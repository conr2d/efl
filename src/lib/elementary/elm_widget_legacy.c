#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>
#include "elm_priv.h"

static Eina_Bool
_elm_widget_is_legacy(Elm_Widget *wid)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(wid, pd, EINA_FALSE);

   return (pd->focus.prev || pd->focus.next || pd->focus.right || pd->focus.left || pd->focus.up || pd->focus.down);
}
