#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>
#include "elm_priv.h"

Evas_Object*
legacy_elm_widget_next_targer(Efl_Ui_Widget *obj, Elm_Focus_Direction dir)
{
   Efl_Ui_Widget *target = NULL;
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, pd, NULL);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(elm_widget_is_legacy(obj), NULL);

   if (dir == EFL_UI_FOCUS_DIRECTION_NEXT || dir == EFL_UI_FOCUS_DIRECTION_PREVIOUS)
     {
        Efl_Ui_Widget *parent;
        //consider custom chain
        parent = elm_widget_parent_get(obj);
        ELM_WIDGET_DATA_GET_OR_RETURN(parent, ppd, NULL)

        if (ppd->legacy_focus.custom_chain)
          {
             Eina_List *node;

             node = eina_list_data_find_list(ppd->legacy_focus.custom_chain, obj);
             if (!node)
               {
                  ERR("Node is not in the custom chain of the parent");
                  return NULL;
               }
             if (dir == EFL_UI_FOCUS_DIRECTION_NEXT) target = eina_list_data_get(eina_list_next(node));
             if (dir == EFL_UI_FOCUS_DIRECTION_PREVIOUS) target = eina_list_data_get(eina_list_prev(node));
          }

     }
   if (!target)
     return elm_object_focus_next_object_get(obj, dir);
   return NULL;
}
