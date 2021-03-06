#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>
#include "elm_priv.h"

#define MY_CLASS EFL_UI_RADIO_GROUP_IMPL_CLASS

static Eina_Hash *radio_group_map;

typedef struct {
   Efl_Ui_Radio *selected;
   Eina_List *registered_set;
} Efl_Ui_Radio_Group_Impl_Data;

EOLIAN static void
_efl_ui_radio_group_impl_efl_ui_radio_group_selected_object_set(Eo *obj EINA_UNUSED, Efl_Ui_Radio_Group_Impl_Data *pd, Efl_Ui_Radio *selected_object)
{
   int new_value = -1;
   Eo *old_selected;

   if (pd->selected == selected_object) return;

   old_selected = pd->selected;
   pd->selected = selected_object;

   //it is essential to *first* set pd->selected to the new state before calling this
   //otherwise this here will be called again, and the event will get emitted twice
   if (old_selected && efl_alive_get(old_selected))
    efl_ui_check_selected_set(old_selected, EINA_FALSE);

   if (pd->selected)
     {
        efl_ui_check_selected_set(pd->selected, EINA_TRUE);
        new_value = efl_ui_radio_state_value_get(pd->selected);
     }
   efl_event_callback_call(obj, EFL_UI_RADIO_GROUP_EVENT_VALUE_CHANGED, &new_value);
}

EOLIAN static Efl_Ui_Radio*
_efl_ui_radio_group_impl_efl_ui_radio_group_selected_object_get(const Eo *obj EINA_UNUSED, Efl_Ui_Radio_Group_Impl_Data *pd)
{
   return pd->selected;
}

EOLIAN static void
_efl_ui_radio_group_impl_efl_ui_radio_group_selected_value_set(Eo *obj, Efl_Ui_Radio_Group_Impl_Data *pd, int selected_value)
{
   Efl_Ui_Radio *reged;
   Eina_List *n;

   EINA_LIST_FOREACH(pd->registered_set, n, reged)
     {
        if (efl_ui_radio_state_value_get(reged) == selected_value)
          {
             efl_ui_radio_group_selected_object_set(obj, reged);
             return;
          }
     }
   ERR("Value %d not assosiated with any radio button", selected_value);
}

EOLIAN static int
_efl_ui_radio_group_impl_efl_ui_radio_group_selected_value_get(const Eo *obj EINA_UNUSED, Efl_Ui_Radio_Group_Impl_Data *pd)
{
   return pd->selected ? efl_ui_radio_state_value_get(pd->selected) : -1;
}

static void
_selected_cb(void *data, const Efl_Event *ev)
{
   if (efl_ui_check_selected_get(ev->object))
     {
        efl_ui_radio_group_selected_object_set(data, ev->object);
     }
   else
     {
        //if something was unselected, we need to make sure that we are unsetting the internal pointer to NULL
        if (efl_ui_radio_group_selected_object_get(data) == ev->object)
          {
             efl_ui_radio_group_selected_object_set(data, NULL);
          }
     }
}

static void
_invalidate_cb(void *data, const Efl_Event *ev)
{
   efl_ui_radio_group_unregister(data, ev->object);
}

EFL_CALLBACKS_ARRAY_DEFINE(radio_btn_cb,
  {EFL_UI_CHECK_EVENT_SELECTED_CHANGED, _selected_cb},
  {EFL_EVENT_INVALIDATE, _invalidate_cb},
)

EOLIAN static void
_efl_ui_radio_group_impl_efl_ui_radio_group_register(Eo *obj, Efl_Ui_Radio_Group_Impl_Data *pd, Efl_Ui_Radio *radio)
{
   Efl_Ui_Radio *reged;
   Eina_List *n;

   if (eina_hash_find(radio_group_map, &radio))
     {
        ERR("Radio button %p is already part of another group", radio);
        return;
     }

   EINA_LIST_FOREACH(pd->registered_set, n, reged)
     {
        EINA_SAFETY_ON_TRUE_RETURN(radio == reged);
        EINA_SAFETY_ON_TRUE_RETURN(efl_ui_radio_state_value_get(radio) == efl_ui_radio_state_value_get(reged));
     }
   EINA_SAFETY_ON_TRUE_RETURN(efl_ui_radio_state_value_get(radio) == -1);

   pd->registered_set = eina_list_append(pd->registered_set, radio);
   eina_hash_add(radio_group_map, &radio, obj);
   efl_event_callback_array_add(radio, radio_btn_cb(), obj);
}

EOLIAN static void
_efl_ui_radio_group_impl_efl_ui_radio_group_unregister(Eo *obj, Efl_Ui_Radio_Group_Impl_Data *pd, Efl_Ui_Radio *radio)
{
   if (pd->selected == radio)
     efl_ui_radio_group_selected_object_set(obj, NULL);

   efl_event_callback_array_del(radio, radio_btn_cb(), obj);
   pd->registered_set = eina_list_remove(pd->registered_set, radio);
   eina_hash_del(radio_group_map, &radio, obj);
}

EOLIAN static void
_efl_ui_radio_group_impl_efl_object_destructor(Eo *obj, Efl_Ui_Radio_Group_Impl_Data *pd)
{
   Eo *radio;

   EINA_LIST_FREE(pd->registered_set, radio)
     {
        efl_event_callback_array_del(radio, radio_btn_cb(), obj);
        eina_hash_del(radio_group_map, &radio, obj);
     }
   efl_destructor(efl_super(obj, MY_CLASS));
}

void
_efl_ui_radio_group_impl_class_constructor(Efl_Class *klass EINA_UNUSED)
{
   radio_group_map = eina_hash_pointer_new(NULL);
}

#include "efl_ui_radio_group_impl.eo.c"
