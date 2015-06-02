#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ecore_js_init.hh>
#include <Ecore.h>

namespace efl { namespace ecore { namespace js {

struct persistent_with_isolate_t
{
    template<class S>
    persistent_with_isolate_t(v8::Isolate *isolate, v8::Handle<S> that)
        : isolate(isolate)
        , persistent(isolate, that)
    {}

    v8::Isolate *isolate;
    v8::Persistent<v8::Value> persistent;
};

static Ecore_Event *extract_event(v8::Local<v8::Object> object)
{
    return compatibility_get_pointer_internal_field<Ecore_Event*>(object, 0);
}

static v8::Local<v8::Object> wrap_event(Ecore_Event *event,
                                        v8::Isolate *isolate)
{
    using v8::String;
    using v8::ObjectTemplate;
    using v8::FunctionTemplate;

    auto obj_tpl = compatibility_new<ObjectTemplate>(isolate);
    obj_tpl->SetInternalFieldCount(1);
    auto ret = obj_tpl->NewInstance();

    auto del = [](compatibility_callback_info_type info)
        -> compatibility_return_type {
        if (info.Length() != 0)
            return compatibility_return();

        ecore_event_del(extract_event(info.This()));
    };

    ret->Set(compatibility_new<String>(isolate, "del"),
             compatibility_new<FunctionTemplate>(isolate, del)->GetFunction());

    compatibility_set_pointer_internal_field(ret, 0, event);

    return ret;
}

static Ecore_Event_Handler *extract_event_handler(v8::Local<v8::Object> object)
{
    return compatibility_get_pointer_internal_field<Ecore_Event_Handler*>
        (object, 0);
}

static v8::Local<v8::Object> wrap_event_handler(Ecore_Event_Handler *handler,
                                                v8::Isolate *isolate)
{
    using v8::String;
    using v8::ObjectTemplate;
    using v8::FunctionTemplate;

    auto obj_tpl = compatibility_new<ObjectTemplate>(isolate);
    obj_tpl->SetInternalFieldCount(1);
    auto ret = obj_tpl->NewInstance();

    auto del = [](compatibility_callback_info_type info)
        -> compatibility_return_type {
        if (info.Length() != 0)
            return compatibility_return();

        auto p = ecore_event_handler_del(extract_event_handler(info.This()));

        delete reinterpret_cast<persistent_with_isolate_t*>(p);
    };

    ret->Set(compatibility_new<String>(isolate, "del"),
             compatibility_new<FunctionTemplate>(isolate, del)->GetFunction());

    compatibility_set_pointer_internal_field(ret, 0, handler);

    return ret;
}

static Ecore_Event_Filter *extract_event_filter(v8::Local<v8::Object> object)
{
    return compatibility_get_pointer_internal_field<Ecore_Event_Filter*>
        (object, 0);
}

static v8::Local<v8::Object> wrap_event_filter(Ecore_Event_Filter *filter,
                                               v8::Isolate *isolate)
{
    using v8::String;
    using v8::ObjectTemplate;
    using v8::FunctionTemplate;

    auto obj_tpl = compatibility_new<ObjectTemplate>(isolate);
    obj_tpl->SetInternalFieldCount(1);
    auto ret = obj_tpl->NewInstance();

    auto del = [](compatibility_callback_info_type info)
        -> compatibility_return_type {
        if (info.Length() != 0)
            return compatibility_return();

        auto p = ecore_event_filter_del(extract_event_filter(info.This()));
        delete[] reinterpret_cast<persistent_with_isolate_t*>(p);
    };

    ret->Set(compatibility_new<String>(isolate, "del"),
             compatibility_new<FunctionTemplate>(isolate, del)->GetFunction());

    compatibility_set_pointer_internal_field(ret, 0, filter);

    return ret;
}

EAPI
void register_event_none(v8::Isolate *isolate, v8::Handle<v8::Object> global,
                         v8::Handle<v8::String> name)
{
    using v8::Integer;
    global->Set(name,
                compatibility_new<Integer>(isolate, ECORE_EVENT_NONE));
}

EAPI
void register_event_signal_user(v8::Isolate *isolate,
                                v8::Handle<v8::Object> global,
                                v8::Handle<v8::String> name)
{
    using v8::Integer;
    global->Set(name,
                compatibility_new<Integer>(isolate, ECORE_EVENT_SIGNAL_USER));
}

EAPI
void register_event_signal_hup(v8::Isolate *isolate,
                               v8::Handle<v8::Object> global,
                               v8::Handle<v8::String> name)
{
    using v8::Integer;
    global->Set(name,
                compatibility_new<Integer>(isolate, ECORE_EVENT_SIGNAL_HUP));
}

EAPI
void register_event_signal_exit(v8::Isolate *isolate,
                                v8::Handle<v8::Object> global,
                                v8::Handle<v8::String> name)
{
    using v8::Integer;
    global->Set(name,
                compatibility_new<Integer>(isolate, ECORE_EVENT_SIGNAL_EXIT));
}

EAPI
void register_event_signal_power(v8::Isolate *isolate,
                                 v8::Handle<v8::Object> global,
                                 v8::Handle<v8::String> name)
{
    using v8::Integer;
    global->Set(name,
                compatibility_new<Integer>(isolate, ECORE_EVENT_SIGNAL_POWER));
}

EAPI
void register_event_signal_realtime(v8::Isolate *isolate,
                                    v8::Handle<v8::Object> global,
                                    v8::Handle<v8::String> name)
{
    using v8::Integer;
    global->Set(name,
                compatibility_new<Integer>(isolate,
                                           ECORE_EVENT_SIGNAL_REALTIME));
}

EAPI
void register_event_memory_state(v8::Isolate *isolate,
                                 v8::Handle<v8::Object> global,
                                 v8::Handle<v8::String> name)
{
    using v8::Integer;
    global->Set(name,
                compatibility_new<Integer>(isolate, ECORE_EVENT_MEMORY_STATE));
}

EAPI
void register_event_power_state(v8::Isolate *isolate,
                                v8::Handle<v8::Object> global,
                                v8::Handle<v8::String> name)
{
    using v8::Integer;
    global->Set(name,
                compatibility_new<Integer>(isolate, ECORE_EVENT_POWER_STATE));
}

EAPI
void register_event_locale_changed(v8::Isolate *isolate,
                                   v8::Handle<v8::Object> global,
                                   v8::Handle<v8::String> name)
{
    using v8::Integer;
    global->Set(name,
                compatibility_new<Integer>(isolate,
                                           ECORE_EVENT_LOCALE_CHANGED));
}

EAPI
void register_event_hostname_changed(v8::Isolate *isolate,
                                     v8::Handle<v8::Object> global,
                                     v8::Handle<v8::String> name)
{
    using v8::Integer;
    global->Set(name,
                compatibility_new<Integer>(isolate,
                                           ECORE_EVENT_HOSTNAME_CHANGED));
}

EAPI
void register_event_system_timedate_changed(v8::Isolate *isolate,
                                            v8::Handle<v8::Object> global,
                                            v8::Handle<v8::String> name)
{
    using v8::Integer;
    global->Set(name,
                compatibility_new<Integer>(isolate,
                                           ECORE_EVENT_SYSTEM_TIMEDATE_CHANGED));
}

EAPI
void register_event_type_new(v8::Isolate *isolate,
                             v8::Handle<v8::Object> global,
                             v8::Handle<v8::String> name)
{
    using v8::Integer;
    using v8::FunctionTemplate;

    auto f = [](compatibility_callback_info_type args)
        -> compatibility_return_type {
        if (args.Length() != 0)
            return compatibility_return();

        auto isolate = args.GetIsolate();
        auto ret = ecore_event_type_new();
        return compatibility_return(compatibility_new<Integer>(isolate, ret),
                                    args);
    };

    global->Set(name,
                compatibility_new<FunctionTemplate>(isolate, f)->GetFunction());
}

EAPI
void register_event_add(v8::Isolate *isolate, v8::Handle<v8::Object> global,
                        v8::Handle<v8::String> name)
{
    using v8::FunctionTemplate;

    auto f = [](compatibility_callback_info_type args)
        -> compatibility_return_type {
        if (args.Length() != 1 || !args[0]->IsNumber())
            return compatibility_return();

        auto isolate = args.GetIsolate();
        auto ret = ecore_event_add(args[0]->NumberValue(), NULL, NULL, NULL);
        return compatibility_return(wrap_event(ret, isolate), args);
    };

    global->Set(name,
                compatibility_new<FunctionTemplate>(isolate, f)->GetFunction());
}

EAPI
void register_event_handler_add(v8::Isolate *isolate,
                                v8::Handle<v8::Object> global,
                                v8::Handle<v8::String> name)
{
    using v8::Integer;
    using v8::Value;
    using v8::Function;
    using v8::Handle;
    using v8::Local;
    using v8::FunctionTemplate;

    auto f = [](compatibility_callback_info_type args)
        -> compatibility_return_type {
        if (args.Length() != 2 || !args[0]->IsNumber()
            || !args[1]->IsFunction()) {
            return compatibility_return();
        }

        auto isolate = args.GetIsolate();

        auto p = new persistent_with_isolate_t(isolate, args[1]);

        auto cb = [](void *d, int type, void */*event*/) -> Eina_Bool {
            auto persistent = reinterpret_cast<persistent_with_isolate_t*>(d);
            auto value = Local<Value>::New(persistent->isolate,
                                           persistent->persistent);
            auto closure = Function::Cast(*value);

            Handle<Value> args = compatibility_new<Integer>(persistent->isolate,
                                                            type);

            auto ret = closure->Call(Undefined(persistent->isolate), 1, &args);
            auto bret = ret->IsBoolean() && ret->BooleanValue();

            return bret ? EINA_TRUE : EINA_FALSE;
        };

        auto ret = ecore_event_handler_add(args[0]->Int32Value(), cb, p);
        return compatibility_return(wrap_event_handler(ret, isolate), args);
    };

    global->Set(name,
                compatibility_new<FunctionTemplate>(isolate, f)->GetFunction());
}

EAPI
void register_event_filter_add(v8::Isolate *isolate,
                               v8::Handle<v8::Object> global,
                               v8::Handle<v8::String> name)
{
    using v8::Integer;
    using v8::Value;
    using v8::Function;
    using v8::Handle;
    using v8::Local;
    using v8::FunctionTemplate;

    auto f = [](compatibility_callback_info_type args)
        -> compatibility_return_type {
        if (args.Length() != 3 || !args[0]->IsFunction()
            || !args[1]->IsFunction() || !args[2]->IsFunction()) {
            return compatibility_return();
        }

        auto isolate = args.GetIsolate();

        auto p = new persistent_with_isolate_t[3]{{isolate, args[0]},
                                                  {isolate, args[1]},
                                                  {isolate, args[2]}};

        auto start_cb = [](void *data) -> void* {
            auto p = reinterpret_cast<persistent_with_isolate_t*>(data);
            auto value = Local<Value>::New(p->isolate, p->persistent);
            auto closure = Function::Cast(*value);

            auto ret = closure->Call(Undefined(p->isolate), 0, NULL);
            return new persistent_with_isolate_t{p->isolate, ret};
        };

        auto filter_cb = [](void *data, void *loop_data, int type,
                            void */*event*/) -> Eina_Bool {
            typedef persistent_with_isolate_t p_t;

            auto p = reinterpret_cast<p_t*>(data) + 1;
            auto value = Local<Value>::New(p->isolate, p->persistent);
            auto closure = Function::Cast(*value);

            Handle<Value> args[2]{
                [loop_data]() {
                    auto ret = reinterpret_cast<p_t*>(loop_data);
                    return Local<Value>::New(ret->isolate, ret->persistent);
                }(),
                compatibility_new<Integer>(p->isolate, type)
            };

            auto ret = closure->Call(Undefined(p->isolate), 2, args);
            auto bret = ret->IsBoolean() && ret->BooleanValue();

            return bret ? EINA_TRUE : EINA_FALSE;
        };

        auto end_cb = [](void *user_data, void *func_data) -> void {
            typedef persistent_with_isolate_t p_t;

            auto loop_data = std::unique_ptr<p_t>(reinterpret_cast<p_t*>
                                                  (func_data));
            auto p = reinterpret_cast<p_t*>(user_data) + 2;
            auto value = Local<Value>::New(p->isolate, p->persistent);
            auto closure = Function::Cast(*value);

            Handle<Value> args = Local<Value>::New(loop_data->isolate,
                                                   loop_data->persistent);

            closure->Call(Undefined(p->isolate), 1, &args);
        };

        auto ret = ecore_event_filter_add(start_cb, filter_cb, end_cb, p);
        return compatibility_return(wrap_event_filter(ret, isolate), args);
    };

    global->Set(name,
                compatibility_new<FunctionTemplate>(isolate, f)->GetFunction());
}

EAPI
void register_event_current_type_get(v8::Isolate *isolate,
                                     v8::Handle<v8::Object> global,
                                     v8::Handle<v8::String> name)
{
    using v8::Integer;
    using v8::FunctionTemplate;

    auto f = [](compatibility_callback_info_type args)
        -> compatibility_return_type {
        if (args.Length() != 0)
            return compatibility_return();

        auto isolate = args.GetIsolate();
        auto ret = ecore_event_current_type_get();
        return compatibility_return(compatibility_new<Integer>(isolate, ret),
                                    args);
    };

    global->Set(name,
                compatibility_new<FunctionTemplate>(isolate, f)->GetFunction());
}

EAPI
void register_memory_state_normal(v8::Isolate *isolate,
                                  v8::Handle<v8::Object> global,
                                  v8::Handle<v8::String> name)
{
    using v8::Integer;
    global->Set(name,
                compatibility_new<Integer>(isolate, ECORE_MEMORY_STATE_NORMAL));
}

EAPI
void register_memory_state_low(v8::Isolate *isolate,
                               v8::Handle<v8::Object> global,
                               v8::Handle<v8::String> name)
{
    using v8::Integer;
    global->Set(name,
                compatibility_new<Integer>(isolate, ECORE_MEMORY_STATE_LOW));
}

EAPI
void register_power_state_mains(v8::Isolate *isolate,
                                v8::Handle<v8::Object> global,
                                v8::Handle<v8::String> name)
{
    using v8::Integer;
    global->Set(name,
                compatibility_new<Integer>(isolate, ECORE_POWER_STATE_MAINS));
}

EAPI
void register_power_state_battery(v8::Isolate *isolate,
                                  v8::Handle<v8::Object> global,
                                  v8::Handle<v8::String> name)
{
    using v8::Integer;
    global->Set(name,
                compatibility_new<Integer>(isolate, ECORE_POWER_STATE_BATTERY));
}

EAPI
void register_power_state_low(v8::Isolate *isolate,
                              v8::Handle<v8::Object> global,
                              v8::Handle<v8::String> name)
{
    using v8::Integer;
    global->Set(name,
                compatibility_new<Integer>(isolate, ECORE_POWER_STATE_LOW));
}

EAPI
void register_memory_state_get(v8::Isolate *isolate,
                               v8::Handle<v8::Object> global,
                               v8::Handle<v8::String> name)
{
    using v8::Integer;
    using v8::FunctionTemplate;

    auto f = [](compatibility_callback_info_type args)
        -> compatibility_return_type {
        if (args.Length() != 0)
            return compatibility_return();

        auto isolate = args.GetIsolate();
        auto ret = ecore_memory_state_get();
        return compatibility_return(compatibility_new<Integer>(isolate, ret),
                                    args);
    };

    global->Set(name,
                compatibility_new<FunctionTemplate>(isolate, f)->GetFunction());
}

EAPI
void register_power_state_get(v8::Isolate *isolate,
                                     v8::Handle<v8::Object> global,
                                     v8::Handle<v8::String> name)
{
    using v8::Integer;
    using v8::FunctionTemplate;

    auto f = [](compatibility_callback_info_type args)
        -> compatibility_return_type {
        if (args.Length() != 0)
            return compatibility_return();

        auto isolate = args.GetIsolate();
        auto ret = ecore_power_state_get();
        return compatibility_return(compatibility_new<Integer>(isolate, ret),
                                    args);
    };

    global->Set(name,
                compatibility_new<FunctionTemplate>(isolate, f)->GetFunction());
}

EAPI
void register_event_signal_user_handler_add(v8::Isolate *isolate,
                                            v8::Handle<v8::Object> global,
                                            v8::Handle<v8::String> name)
{
    using v8::Integer;
    using v8::String;
    using v8::Value;
    using v8::Object;
    using v8::Function;
    using v8::Handle;
    using v8::Local;
    using v8::FunctionTemplate;

    auto f = [](compatibility_callback_info_type args)
        -> compatibility_return_type {
        if (args.Length() != 1 || !args[0]->IsFunction())
            return compatibility_return();

        auto isolate = args.GetIsolate();

        auto p = new persistent_with_isolate_t(isolate, args[0]);

        auto cb = [](void *d, int type, void *event) -> Eina_Bool {
            auto p = reinterpret_cast<persistent_with_isolate_t*>(d);
            auto value = Local<Value>::New(p->isolate, p->persistent);
            auto closure = Function::Cast(*value);

            auto wrapped_event = compatibility_new<Object>(p->isolate);

            {
                auto n
                    = reinterpret_cast<Ecore_Event_Signal_User*>(event)->number;
                wrapped_event->Set(compatibility_new<String>(p->isolate,
                                                             "number"),
                                   compatibility_new<Integer>(p->isolate, n));
            }

            Handle<Value> args[2]{
                compatibility_new<Integer>(p->isolate, type),
                wrapped_event
            };

            auto ret = closure->Call(Undefined(p->isolate), 2, args);
            auto bret = ret->IsBoolean() && ret->BooleanValue();

            return bret ? EINA_TRUE : EINA_FALSE;
        };

        auto ret = ecore_event_handler_add(ECORE_EVENT_SIGNAL_USER, cb, p);
        return compatibility_return(wrap_event_handler(ret, isolate), args);
    };

    global->Set(name,
                compatibility_new<FunctionTemplate>(isolate, f)->GetFunction());
}

EAPI
void register_event_signal_exit_handler_add(v8::Isolate *isolate,
                                            v8::Handle<v8::Object> global,
                                            v8::Handle<v8::String> name)
{
    using v8::Integer;
    using v8::Boolean;
    using v8::String;
    using v8::Value;
    using v8::Object;
    using v8::Function;
    using v8::Handle;
    using v8::Local;
    using v8::FunctionTemplate;

    auto f = [](compatibility_callback_info_type args)
        -> compatibility_return_type {
        if (args.Length() != 1 || !args[0]->IsFunction())
            return compatibility_return();

        auto isolate = args.GetIsolate();

        auto p = new persistent_with_isolate_t(isolate, args[0]);

        auto cb = [](void *d, int type, void *ev) -> Eina_Bool {
            auto p = reinterpret_cast<persistent_with_isolate_t*>(d);
            auto value = Local<Value>::New(p->isolate, p->persistent);
            auto closure = Function::Cast(*value);

            auto wrapped_event = compatibility_new<Object>(p->isolate);

            {
                auto event = reinterpret_cast<Ecore_Event_Signal_Exit*>(ev);
                auto interrupt = event->interrupt;
                auto quit = event->quit;
                auto terminate = event->terminate;

                wrapped_event->Set(compatibility_new<String>(p->isolate,
                                                             "interrupt"),
                                   compatibility_new<Boolean>(p->isolate,
                                                              interrupt));
                wrapped_event->Set(compatibility_new<String>(p->isolate,
                                                             "quit"),
                                   compatibility_new<Boolean>(p->isolate,
                                                              quit));
                wrapped_event->Set(compatibility_new<String>(p->isolate,
                                                             "terminate"),
                                   compatibility_new<Boolean>(p->isolate,
                                                              terminate));
            }

            Handle<Value> args[2]{
                compatibility_new<Integer>(p->isolate, type),
                wrapped_event
            };

            auto ret = closure->Call(Undefined(p->isolate), 2, args);
            auto bret = ret->IsBoolean() && ret->BooleanValue();

            return bret ? EINA_TRUE : EINA_FALSE;
        };

        auto ret = ecore_event_handler_add(ECORE_EVENT_SIGNAL_EXIT, cb, p);
        return compatibility_return(wrap_event_handler(ret, isolate), args);
    };

    global->Set(name,
                compatibility_new<FunctionTemplate>(isolate, f)->GetFunction());
}

EAPI
void register_event_signal_realtime_handler_add(v8::Isolate *isolate,
                                                v8::Handle<v8::Object> global,
                                                v8::Handle<v8::String> name)
{
    using v8::Integer;
    using v8::String;
    using v8::Value;
    using v8::Object;
    using v8::Function;
    using v8::Handle;
    using v8::Local;
    using v8::FunctionTemplate;

    auto f = [](compatibility_callback_info_type args)
        -> compatibility_return_type {
        if (args.Length() != 1 || !args[0]->IsFunction())
            return compatibility_return();

        auto isolate = args.GetIsolate();

        auto p = new persistent_with_isolate_t(isolate, args[0]);

        auto cb = [](void *d, int type, void *ev) -> Eina_Bool {
            auto p = reinterpret_cast<persistent_with_isolate_t*>(d);
            auto value = Local<Value>::New(p->isolate, p->persistent);
            auto closure = Function::Cast(*value);

            auto wrapped_event = compatibility_new<Object>(p->isolate);

            {
                auto n
                    = reinterpret_cast<Ecore_Event_Signal_Realtime*>(ev)->num;
                wrapped_event->Set(compatibility_new<String>(p->isolate, "num"),
                                   compatibility_new<Integer>(p->isolate, n));
            }

            Handle<Value> args[2]{
                compatibility_new<Integer>(p->isolate, type),
                wrapped_event
            };

            auto ret = closure->Call(Undefined(p->isolate), 2, args);
            auto bret = ret->IsBoolean() && ret->BooleanValue();

            return bret ? EINA_TRUE : EINA_FALSE;
        };

        auto ret = ecore_event_handler_add(ECORE_EVENT_SIGNAL_REALTIME, cb, p);
        return compatibility_return(wrap_event_handler(ret, isolate), args);
    };

    global->Set(name,
                compatibility_new<FunctionTemplate>(isolate, f)->GetFunction());
}

} } } // namespace efl { namespace js {
