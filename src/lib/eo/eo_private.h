#ifndef _EO_PRIVATE_H
#define _EO_PRIVATE_H

#define EO_EINA_MAGIC 0xa186bc32
#define EO_EINA_MAGIC_STR "Eo"
#define EO_FREED_EINA_MAGIC 0xa186bb32
#define EO_FREED_EINA_MAGIC_STR "Eo - Freed object"
#define EO_CLASS_EINA_MAGIC 0xa186ba32
#define EO_CLASS_EINA_MAGIC_STR "Eo Class"

#define EO_MAGIC_RETURN_VAL(d, magic, ret) \
   do { \
        if (!EINA_MAGIC_CHECK(d, magic)) \
          { \
             EINA_MAGIC_FAIL(d, magic); \
             return ret; \
          } \
   } while (0)

#define EO_MAGIC_RETURN(d, magic) \
   do { \
        if (!EINA_MAGIC_CHECK(d, magic)) \
          { \
             EINA_MAGIC_FAIL(d, magic); \
             return; \
          } \
   } while (0)


extern int _eo_log_dom;

#ifdef CRITICAL
#undef CRITICAL
#endif
#define CRITICAL(...) EINA_LOG_DOM_CRIT(_eo_log_dom, __VA_ARGS__)

#ifdef ERR
#undef ERR
#endif
#define ERR(...) EINA_LOG_DOM_ERR(_eo_log_dom, __VA_ARGS__)

#ifdef WRN
#undef WRN
#endif
#define WRN(...) EINA_LOG_DOM_WARN(_eo_log_dom, __VA_ARGS__)

#ifdef INF
#undef INF
#endif
#define INF(...) EINA_LOG_DOM_INFO(_eo_log_dom, __VA_ARGS__)

#ifdef DBG
#undef DBG
#endif
#define DBG(...) EINA_LOG_DOM_DBG(_eo_log_dom, __VA_ARGS__)

typedef size_t  Eo_Id;
typedef struct _Eo_Class _Eo_Class;

/* Retrieves the pointer to the object from the id */
static inline _Eo *_eo_obj_pointer_get(const Eo_Id obj_id);

/* Allocates an entry for the given object */
static inline Eo_Id _eo_id_allocate(const _Eo *obj);

/* Releases an entry by the object id */
static inline void _eo_id_release(const Eo_Id obj_id);

/* Free all the entries and the tables */
static inline void _eo_free_ids_tables(void);

void _eo_condtor_done(Eo *obj);

struct _Eo_Internal {
#ifndef HAVE_EO_ID
     EINA_MAGIC
#endif
     Eo *parent;
     Eina_List *children;
     const _Eo_Class *klass;
#ifdef EO_DEBUG
     Eina_Inlist *xrefs;
     Eina_Inlist *data_xrefs;
#endif

     Eina_List *composite_objects;

     Eo_Id obj_id;

     int refcount;
     int datarefcount;

     Eina_Bool do_error:1;
     Eina_Bool condtor_done:1;

     Eina_Bool composite:1;
     Eina_Bool del:1;
     Eina_Bool manual_free:1;
};

/**
 * @typedef Eo_Class_Id
 * An Id of a class.
 * @ingroup Eo_Class
 */
typedef uintptr_t Eo_Class_Id;

typedef struct _Dich_Chain1 Dich_Chain1;

typedef struct
{
   eo_op_func_type func;
   const _Eo_Class *src;
} op_type_funcs;

struct _Dich_Chain1
{
   op_type_funcs *funcs;
};

typedef struct
{
   const _Eo_Class *klass;
   size_t offset;
} Eo_Extension_Data_Offset;

struct _Eo_Class
{
   EINA_MAGIC
   Eo_Class_Id class_id;
   const _Eo_Class *parent;
   const Eo_Class_Description *desc;
   Dich_Chain1 *chain; /**< The size is chain size */

   const _Eo_Class **extensions;

   Eo_Extension_Data_Offset *extn_data_off;

   const _Eo_Class **mro;

   /* cached object for faster allocation */
   struct {
      Eina_Trash  *trash;
      Eina_Lock    trash_lock;
      unsigned int trash_count;
   } objects;

   /* cached iterator for faster allocation cycle */
   struct {
      Eina_Trash  *trash;
      Eina_Lock    trash_lock;
      unsigned int trash_count;
   } iterators;

   unsigned int obj_size; /**< size of an object of this class */
   unsigned int chain_size;
   unsigned int base_id;
   unsigned int data_offset; /* < Offset of the data within object data. */

   Eina_Bool constructed : 1;
   /* [extensions*] + NULL */
   /* [mro*] + NULL */
   /* [extensions data offset] + NULL */
};

static inline void
_eo_condtor_reset(_Eo *obj)
{
   obj->condtor_done = EINA_FALSE;
}

static inline void
_eo_del_internal(const char *file, int line, _Eo *obj)
{
   Eina_Bool do_err;
   /* We need that for the event callbacks that may ref/unref. */
   obj->refcount++;

   eo_do((Eo *) obj->obj_id, eo_event_callback_call(EO_EV_DEL, NULL, NULL));

   const _Eo_Class *klass = obj->klass;

   _eo_condtor_reset(obj);

   do_err = eo_do((Eo *)obj->obj_id, eo_destructor());
   if (EINA_UNLIKELY(!do_err))
     {
        ERR("in %s:%d: Object of class '%s' - One of the object destructors have failed.",
            file, line, klass->desc->name);
     }

   if (!obj->condtor_done)
     {
        ERR("in %s:%d: Object of class '%s' - Not all of the object destructors have been executed.",
            file, line, klass->desc->name);
     }
   /*FIXME: add eo_class_unref(klass) ? - just to clear the caches. */

     {
        Eina_List *itr, *itr_n;
        Eo *emb_obj;
        EINA_LIST_FOREACH_SAFE(obj->composite_objects, itr, itr_n, emb_obj)
          {
             eo_composite_detach(emb_obj, (Eo *)obj->obj_id);
          }
     }

   while (obj->children)
     {
        eo_parent_set(eina_list_data_get(obj->children), NULL); // ZZZ
     }

   obj->del = EINA_TRUE;
   obj->refcount--;
}

static inline void
_eo_free(_Eo *obj)
{
   _Eo_Class *klass = (_Eo_Class*) obj->klass;

#ifdef EO_DEBUG
   if (obj->datarefcount)
     {
        ERR("Object %p data still referenced %d time(s).", obj, obj->datarefcount);
     }
#endif
   _eo_id_release(obj->obj_id);

   eina_lock_take(&klass->objects.trash_lock);
   if (klass->objects.trash_count <= 8)
     {
        eina_trash_push(&klass->objects.trash, obj);
        klass->objects.trash_count++;
     }
   else
     {
        free(obj);
     }
   eina_lock_release(&klass->objects.trash_lock);
}

static inline _Eo *
_eo_ref(_Eo *obj)
{
   obj->refcount++;
   return obj;
}

static inline void
_eo_unref(_Eo *obj)
{
   --(obj->refcount);
   if (obj->refcount == 0)
     {
        if (obj->del)
          {
             ERR("Object %p already deleted.", obj);
             return;
          }

        _eo_del_internal(__FILE__, __LINE__, obj);

#ifdef EO_DEBUG
        /* If for some reason it's not empty, clear it. */
        while (obj->xrefs)
          {
             ERR("obj->xrefs is not empty, possibly a bug, please report. - An error will be reported for each xref in the stack.");
             Eina_Inlist *nitr = obj->xrefs->next;
             free(EINA_INLIST_CONTAINER_GET(obj->xrefs, Eo_Xref_Node));
             obj->xrefs = nitr;
          }
        while (obj->data_xrefs)
          {
             Eina_Inlist *nitr = obj->data_xrefs->next;
             Eo_Xref_Node *xref = EINA_INLIST_CONTAINER_GET(obj->data_xrefs, Eo_Xref_Node);
             ERR("Data of object 0x%lx is still referenced by object %p", (unsigned long)obj->obj_id, xref->ref_obj);

             free(xref);
             obj->data_xrefs = nitr;
          }
#endif

        if (!obj->manual_free)
          _eo_free(obj);
        else
          _eo_ref(obj); /* If we manual free, we keep a phantom ref. */
     }
   else if (obj->refcount < 0)
     {
        ERR("Obj:%p. Refcount (%d) < 0. Too many unrefs.", obj, obj->refcount);
        return;
     }
}

#endif
