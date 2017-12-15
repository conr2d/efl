#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define ELM_INTERFACE_ATSPI_ACCESSIBLE_PROTECTED
#define ELM_INTERFACE_ATSPI_TEXT_PROTECTED
#define ELM_INTERFACE_ATSPI_TEXT_EDITABLE_PROTECTED
#define ELM_LAYOUT_PROTECTED

#define EFL_SELECTION_MANAGER_BETA

#include <Elementary.h>
#include <Elementary_Cursor.h>
#include "elm_priv.h"

//#define MY_CLASS_DRAG EFL_UI_DND_DRAG_MIXIN
//#define MY_CLASS_DROP EFL_UI_DND_DROP_MIXIN

//FIXME: Efl_Selection_Format should be changed to Efl_Data_Format

static inline Eo*
_selection_manager_get(Eo *obj)
{
   Eo *top = elm_widget_top_get(obj);
   if (!top)
     {
        top = obj;
     }
   Eo *sel_man = efl_key_data_get(top, "__selection_manager");
   if (!sel_man)
     {
        sel_man = efl_add(EFL_SELECTION_MANAGER_CLASS, top);
        efl_key_data_set(top, "__selection_manager", sel_man);
     }
   return sel_man;
}

EOLIAN static void
_efl_ui_dnd_drag_start(Eo *obj, void *pd, Efl_Selection_Format format, Eina_Slice data,
                               Efl_Selection_Action action, void *icon_func_data, Efl_Dnd_Drag_Icon_Create icon_func, Eina_Free_Cb icon_func_free_cb,
                               unsigned int seat)
{
   ERR("In");
   //pd->icon_create = icon_func;
   //pd->icon_create_data = icon_func_data;
   //pd->icon_create_free_cb = icon_func_free_cb;

   //TODO: check seat
   Eo *sel_man = _selection_manager_get(obj);
   efl_selection_manager_drag_start(sel_man, obj, format, data, action, icon_func_data, icon_func, icon_func_free_cb, seat);
}

EOLIAN static void
_efl_ui_dnd_drag_cancel(Eo *obj, void *pd, unsigned int seat)
{
   ERR("In");
   Eo *sel_man = _selection_manager_get(obj);
   efl_selection_manager_drag_cancel(sel_man, obj, seat);
}

EOLIAN static void
_efl_ui_dnd_drag_action_set(Eo *obj, void *pd , Efl_Selection_Action action, unsigned int seat)
{
   ERR("In");
   Eo *sel_man = _selection_manager_get(obj);
   efl_selection_manager_drag_action_set(sel_man, action, seat);
}


EOLIAN static void
_efl_ui_dnd_drop_target_add(Eo *obj, void *pd, Efl_Selection_Format format, unsigned int seat)
{
   ERR("In");
   //pd->format = format;

   Eo *sel_man = _selection_manager_get(obj);
   efl_selection_manager_drop_target_add(sel_man, obj, format, seat);
}

EOLIAN static void
_efl_ui_dnd_drop_target_del(Eo *obj, void *pd, Efl_Selection_Format format, unsigned int seat)
{
   ERR("In");
   Eo *sel_man = _selection_manager_get(obj);
   efl_selection_manager_drop_target_del(sel_man, obj, format, seat);
}

EOLIAN static void
_efl_ui_dnd_container_drag_item_add(Eo *obj, void *pd, double time_to_drag, double anim_time,
                                 void *data_func_data, Efl_Dnd_Drag_Data_Get data_func, Eina_Free_Cb data_func_free_cb,
                                 void *item_func_data, Efl_Dnd_Item_Get item_func, Eina_Free_Cb item_func_free_cb,
                                 void *icon_func_data, Efl_Dnd_Drag_Icon_Create icon_func, Eina_Free_Cb icon_func_free_cb,
                                 void *icon_list_func_data, Efl_Dnd_Drag_Icon_List_Create icon_list_func, Eina_Free_Cb icon_list_func_free_cb,
                                 unsigned int seat)
{
   ERR("In");
   Eo *sel_man = _selection_manager_get(obj);
   efl_selection_manager_container_drag_item_add(sel_man, obj, time_to_drag, anim_time,
                                                 data_func_data, data_func, data_func_free_cb,
                                                 item_func_data, item_func, item_func_free_cb,
                                                 icon_func_data, icon_func, icon_func_free_cb,
                                                 icon_list_func_data, icon_list_func, icon_list_func_free_cb,
                                                 seat);
}

static void
_efl_ui_dnd_container_drag_item_del(Eo *obj, void *pd, unsigned int seat)
{
   ERR("In");
   Eo *sel_man = _selection_manager_get(obj);
   efl_selection_manager_container_drag_item_del(sel_man, obj, seat);
}
EOLIAN static void
_efl_ui_dnd_container_drop_item_add(Eo *obj, void *pd, Efl_Selection_Format format, void *item_func_data, Efl_Dnd_Item_Get item_func, Eina_Free_Cb item_func_free_cb, unsigned int seat)
{
   ERR("In");
   Eo *sel_man = _selection_manager_get(obj);
   efl_selection_manager_container_drop_item_add(sel_man, obj, format, item_func_data, item_func, item_func_free_cb, seat);
}

EOLIAN static void
_efl_ui_dnd_container_drop_item_del(Eo *obj, void *pd, unsigned int seat)
{
   ERR("In");
   Eo *sel_man = _selection_manager_get(obj);
   efl_selection_manager_container_drop_item_del(sel_man, obj, seat);
}


#include "efl_ui_dnd.eo.c"
#include "efl_ui_dnd_container.eo.c"
