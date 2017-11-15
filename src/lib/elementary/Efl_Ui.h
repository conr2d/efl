#ifndef _EFL_CORE_H
#define _EFL_CORE_H

#include <Efl_Config.h>
#include <Elementary_Options.h>

#ifdef _MSC_VER
# include <Evil.h>
#endif

/* Standard headers for standard system calls etc. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/param.h>
#include <math.h>
#include <fnmatch.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>
#ifdef ELM_DIRENT_H
#include <dirent.h>
#endif
#include <pwd.h>
#include <errno.h>

#ifdef ELM_UNIX
#include <locale.h>
#ifdef ELM_LIBINTL_H
#include <libintl.h>
#endif
#include <signal.h>
#include <grp.h>
#include <glob.h>
#endif

#ifdef ELM_ALLOCA_H
#include <alloca.h>
#endif

#ifdef ELM_WIN32
#include <malloc.h>
#ifndef alloca
#define alloca _alloca
#endif
#endif

#include <Eina.h>
#include <Eo.h>
#include <Efl.h>

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EFL_ECORE_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif /* ! DLL_EXPORT */
# else
#  define EAPI __declspec(dllimport)
# endif /* ! EFL_ECORE_BUILD */
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif /* ! _WIN32 */

#ifdef _WIN32
# define WIN32_LEAN_AND_MEAN
# include <winsock2.h>
# undef WIN32_LEAN_AND_MEAN
#elif defined (__FreeBSD__) || defined (__OpenBSD__)
# include <sys/select.h>
# include <signal.h>
#elif defined (__ANDROID__)
# include <sys/select.h>
#elif defined (__sun)
# include <sys/siginfo.h>
#else
# include <sys/time.h>
# if !defined (EXOTIC_NO_SIGNAL)
#  include <signal.h>
# endif
#endif

#include <sys/types.h>

#define __EFL_UI_IS_REQUIRED

#include <Efl_Net.h>

#include <Evas.h>
#include <Evas_GL.h>
#include <Eio.h>
#include <Edje.h>
#include <Eldbus.h>
#include <Efreet.h>
#include <Efreet_Mime.h>
#include <Efreet_Trash.h>
#include <Ethumb_Client.h>

#ifdef ELM_ELOCATION
#include <Elocation.h>
#endif

#ifdef ELM_EMAP
#include <EMap.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern EAPI double _efl_startup_time;

# include <efl_ui_focus_object.eo.h>
# include <efl_ui_focus_manager.eo.h>
# include <efl_ui_focus_manager_calc.eo.h>
# include <efl_ui_focus_manager_sub.eo.h>
# include <efl_ui_focus_manager_root_focus.eo.h>
# include <efl_ui_focus_user.eo.h>
# include <efl_ui_textpath.eo.h>
# include <efl_ui_translatable.eo.h>
# include <efl_ui_focus_composition.eo.h>
# include <efl_ui_focus_layer.eo.h>
# include <efl_access.h>
# include <efl_access_text.h>
# include <efl_access_window.h>

# include <efl_access_action.eo.h>
# include <efl_access_component.eo.h>
# include <efl_access_editable_text.eo.h>
# include <efl_access_image.eo.h>
# include <efl_access_selection.eo.h>
# include <efl_access_value.eo.h>

# include <efl_config_global.eo.h>
# include <efl_ui_widget_part.eo.h>
# include <efl_ui_layout_part.eo.h>
# include <efl_ui_layout_part_box.eo.h>
# include <efl_ui_layout_part_content.eo.h>
# include <efl_ui_layout_part_text.eo.h>
# include <efl_ui_layout_part_table.eo.h>
# include <efl_ui_win_part.eo.h>
# include <efl_ui_box.eo.h>
# include <efl_ui_box_flow.eo.h>
# include <efl_ui_box_stack.eo.h>
# include <efl_ui_grid.eo.h>
# include <efl_ui_grid_static.eo.h>
# include <efl_ui_image.eo.h>
# include <efl_ui_win.eo.h>
# include <efl_ui_win_inlined.eo.h>
# include <efl_ui_win_socket.eo.h>
# include <efl_ui_text_interactive.eo.h>
# include <efl_ui_text.eo.h>
# include <efl_ui_text_editable.eo.h>
# include <efl_ui_text_async.eo.h>
# include <efl_ui_clock.h>
# include <efl_ui_image_factory.eo.h>
# include <efl_ui_video.h>
# include <efl_ui_nstate.h>
# include <efl_ui_calendar.h>
# include <efl_ui_button_eo.h>
# include <efl_ui_multibuttonentry.h>
# include <efl_ui_slider_eo.h>
# include <efl_ui_flip_eo.h>
# include <efl_ui_frame_eo.h>
# include <efl_ui_check_eo.h>
# include <efl_ui_image_zoomable_eo.h>
# include <efl_ui_progressbar_eo.h>
# include <efl_ui_radio_eo.h>
# include <efl_ui_bg_eo.h>
# include <efl_ui_panes_eo.h>

#ifdef __cplusplus
}
#endif

#undef EAPI
#define EAPI

#endif

// We are including efl_general.h again, just in case Efl_Core.h was already included before this
// and the __EFL_*_IS_REQUIRED changed since then.
#include <efl_general.h>
