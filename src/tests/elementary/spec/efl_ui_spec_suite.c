#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>
#include <Efl_Ui.h>
#include "efl_ui_spec_suite.h"
#include "suite_helpers.h"
#include "eo_internal.h"

Evas_Object *win = NULL;
Evas_Object *widget = NULL;
const Efl_Class *test_content_klass = NULL;
EFL_CLASS_SIMPLE_CLASS(efl_ui_widget, "efl_ui_widget", EFL_UI_WIDGET_CLASS);

static void
_setup_window_and_widget(const Efl_Class *klass, const Efl_Class *content_klass)
{
   ck_assert(!win);
   ck_assert(!widget);

   test_content_klass = content_klass;
   win = win_add();
   widget = efl_add(klass, win);
   efl_content_set(win, widget);

   ck_assert(win);
   ck_assert(widget);
}

static void
_shutdown_fixture(void)
{
   //We just NULL them out, shutdown will delete them
   win = NULL;
   widget = NULL;
}

Eo*
create_test_widget(void)
{
   Eo *ret = efl_add(test_content_klass, win);

   return ret;
}

#include "efl_ui_spec_suite_gen.x"
int
main(int argc, char **argv)
{
   int failed_count;

#define LIST_DECL
#include "efl_ui_spec_suite_gen.x"

   if (!_efl_test_option_disp(argc, argv, etc))
     return 0;

   failed_count = suite_setup(EINA_FALSE);

   failed_count += _efl_suite_build_and_run(argc - 1, (const char **)argv + 1,
                                           "Efl_Ui_Spec", etc, SUITE_INIT_FN(elm2), SUITE_SHUTDOWN_FN(elm));

   return (failed_count == 0) ? 0 : 255;
}
