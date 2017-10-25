#ifndef EFL_MODEL_COMMON_H__
# define EFL_MODEL_COMMON_H__

#include <Eina.h>

EAPI extern Eina_Error EFL_MODEL_ERROR_UNKNOWN;
EAPI extern Eina_Error EFL_MODEL_ERROR_NOT_SUPPORTED;
EAPI extern Eina_Error EFL_MODEL_ERROR_NOT_FOUND;
EAPI extern Eina_Error EFL_MODEL_ERROR_READ_ONLY;
EAPI extern Eina_Error EFL_MODEL_ERROR_INIT_FAILED;
EAPI extern Eina_Error EFL_MODEL_ERROR_INCORRECT_VALUE;
EAPI extern Eina_Error EFL_MODEL_ERROR_PERMISSION_DENIED;
EAPI extern Eina_Error EFL_MODEL_ERROR_INVALID_OBJECT; /**< @since 1.19 */

/**
 * @struct _Efl_Model_Children_Event
 * Every time a child id added the event
 * EFL_MODEL_EVENT_CHILD_ADDED is dispatched
 * passing along this structure.
 */
struct _Efl_Model_Children_Event
{
   /**
    * index is a hint and is intended
    * to provide a way for applications
    * to control/know children relative
    * positions through listings.
    */
   unsigned int index;
};

/**
 * @struct Efl_Model_Children_Event
 */
typedef struct _Efl_Model_Children_Event Efl_Model_Children_Event;

#include "interfaces/efl_model.eo.h"

EAPI int efl_model_init(void);


/**
 * @brief Notifies a property changed event with an @c EFL_MODEL_EVENT_PROPERTIES_CHANGED
 *
 * @param model The model to be notified
 * @param property The changed property
 *
 * @since 1.17
 */
EAPI void _efl_model_properties_changed_internal(Efl_Model *model, ...);

#define efl_model_properties_changed(Model, ...) _efl_model_properties_changed_internal(Model, ##__VA_ARGS__, NULL)

/**
 * @brief Notifies a property invalidated event with an @c EFL_MODEL_EVENT_PROPERTIES_CHANGED
 *
 * @param model The model to be notified
 * @param property The invalidated property
 *
 * @since 1.17
 */
EAPI void efl_model_property_invalidated_notify(Efl_Model *model, const char *property);

/**
 * @brief Callback to setup a member of @c Eina_Value_Struct
 *
 * @param data The user data
 * @param index The member index
 * @param member The member to fill its name and type. Must use @c Eina_Stringshare for name.
 */
typedef void (*Efl_Model_Value_Struct_Member_Setup_Cb)(void *data, int index, Eina_Value_Struct_Member *member);

/**
 * @brief Creates a new struct description
 *
 * @param member_count The number of struct members
 * @param setup_cb The callback to setup struct members
 * @param data The user data
 * @return Returns the struct description
 *
 * @since 1.17
 */
EAPI Eina_Value_Struct_Desc *efl_model_value_struct_description_new(unsigned int member_count, Efl_Model_Value_Struct_Member_Setup_Cb setup_cb, void *data) EINA_ARG_NONNULL(2);

/**
 * @brief Frees the memory allocated to the struct description.
 *
 * @param desc The struct description. If @c NULL, the function returns immediately.
 *
 * @since 1.17
 */
EAPI void efl_model_value_struct_description_free(Eina_Value_Struct_Desc *desc);


static inline Eina_Value
efl_model_list_value_get(Eina_List *childrens,
                         unsigned int start,
                         unsigned int count)
{
   Eina_Value v = EINA_VALUE_EMPTY;
   Eina_List *l;
   Eo *child;

   eina_value_array_setup(&v, EINA_VALUE_TYPE_OBJECT, eina_list_count(childrens));

   EINA_LIST_FOREACH(childrens, l, child)
     {
        if (start != 0)
          {
             start--;
             continue;
          }
        if (count == 0)
          continue;
        count--;

        eina_value_array_append(&v, child);
     }

   return v;
}


#endif
