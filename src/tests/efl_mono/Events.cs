using System;
using System.Linq;

namespace TestSuite
{

class TestEoEvents
{
    public bool called = false;
    public bool correct_sender = false;
    public Efl.Loop loop { get; set; }
    protected void callback(object sender, EventArgs e) {
        called = true;
        Efl.Object obj = sender as Efl.Object;
        if (obj != null)
        {
            obj.SetName("loop_called");
            correct_sender = true;
        }

        Eina.Value v = new Eina.Value(Eina.ValueType.Int32);
        v.Set(0);
        loop.Quit(v);
    }
    protected void another_callback(object sender, EventArgs e) { }

    public static void idle_event()
    {
        Efl.Loop loop = Efl.App.AppMain;
        loop.SetName("loop");
        TestEoEvents listener = new TestEoEvents();
        listener.loop = loop;
        loop.IdleEvt += listener.callback;

        Test.Assert(!listener.called);
        Test.Assert(!listener.correct_sender);
        Test.AssertEquals("loop", loop.GetName());
        loop.Begin();
        Test.Assert(listener.called);
        Test.Assert(listener.correct_sender);
        Test.AssertEquals("loop_called", loop.GetName());
    }

    public static void event_with_string_payload()
    {
        var obj = new Dummy.TestObject();
        string received_string = null;

        obj.EvtWithStringEvt += (object sender, Dummy.TestObjectEvtWithStringEvt_Args e) => {
            received_string = e.arg;
        };

        obj.EmitEventWithString("Some args");

        Test.AssertEquals("Some args", received_string);
    }

    public static void event_with_int_payload()
    {
        var obj = new Dummy.TestObject();
        int received_int= 0;

        obj.EvtWithIntEvt += (object sender, Dummy.TestObjectEvtWithIntEvt_Args e) => {
            received_int = e.arg;
        };

        obj.EmitEventWithInt(-1984);

        Test.AssertEquals(-1984, received_int);
    }

    public static void event_with_bool_payload()
    {
        var obj = new Dummy.TestObject();
        bool received_bool = false;

        obj.EvtWithBoolEvt += (object sender, Dummy.TestObjectEvtWithBoolEvt_Args e) => {
            received_bool = e.arg;
        };

        obj.EmitEventWithBool(true);

        Test.AssertEquals(true, received_bool);

        obj.EmitEventWithBool(false);

        Test.AssertEquals(false, received_bool);
    }

    public static void event_with_uint_payload()
    {
        var obj = new Dummy.TestObject();
        uint received_uint = 0;
        obj.EvtWithUintEvt += (object sender, Dummy.TestObjectEvtWithUintEvt_Args e) => {
            received_uint = e.arg;
        };

        obj.EmitEventWithUint(0xbeef);

        Test.AssertEquals<uint>(0xbeef, received_uint);
    }

    public static void event_with_float_payload()
    {
        var obj = new Dummy.TestObject();
        float received_float = 0;
        obj.EvtWithFloatEvt += (object sender, Dummy.TestObjectEvtWithFloatEvt_Args e) => {
            received_float = e.arg;
        };

        obj.EmitEventWithFloat(3.14f);

        Test.AssertAlmostEquals(3.14f, received_float);
    }

    public static void event_with_double_payload()
    {
        var obj = new Dummy.TestObject();
        double received_double = 0;
        double reference = float.MaxValue + 42;
        obj.EvtWithDoubleEvt += (object sender, Dummy.TestObjectEvtWithDoubleEvt_Args e) => {
            received_double = e.arg;
        };

        obj.EmitEventWithDouble(reference);

        Test.AssertAlmostEquals(reference, received_double);
    }

    public static void event_with_object_payload()
    {
        var obj = new Dummy.TestObject();
        Dummy.TestObject received_obj = null;

        obj.EvtWithObjEvt += (object sender, Dummy.TestObjectEvtWithObjEvt_Args e) => {
            received_obj = e.arg;
        };

        var sent_obj = new Dummy.TestObject();

        obj.EmitEventWithObj(sent_obj);

        Test.AssertEquals(sent_obj, received_obj);
    }

    public static void event_with_error_payload()
    {
        var obj = new Dummy.TestObject();
        Eina.Error received_error = 0;

        obj.EvtWithErrorEvt += (object sender, Dummy.TestObjectEvtWithErrorEvt_Args e) => {
            received_error = e.arg;
        };

        Eina.Error sent_error = -2001;

        obj.EmitEventWithError(sent_error);

        Test.AssertEquals(sent_error, received_error);
    }

    public static void event_with_struct_payload()
    {
        var obj = new Dummy.TestObject();
        Dummy.StructSimple received_struct = default(Dummy.StructSimple);

        obj.EvtWithStructEvt += (object sender, Dummy.TestObjectEvtWithStructEvt_Args e) => {
            received_struct = e.arg;
        };

        Dummy.StructSimple sent_struct = default(Dummy.StructSimple);
        sent_struct.Fstring = "Struct Event";

        obj.EmitEventWithStruct(sent_struct);

        Test.AssertEquals(sent_struct.Fstring, received_struct.Fstring);
    }

    public static void event_with_struct_complex_payload()
    {
        var obj = new Dummy.TestObject();
        Dummy.StructComplex received_struct = default(Dummy.StructComplex);

        obj.EvtWithStructComplexEvt += (object sender, Dummy.TestObjectEvtWithStructComplexEvt_Args e) => {
            received_struct = e.arg;
        };

        Dummy.StructComplex sent_struct = StructHelpers.structComplexWithValues();

        obj.EmitEventWithStructComplex(sent_struct);

        Test.AssertEquals(sent_struct.Fobj, received_struct.Fobj);
    }

    public static void event_with_array_payload()
    {
        var obj = new Dummy.TestObject();
        Eina.Array<string> received = null;
        Eina.Array<string> sent = new Eina.Array<string>();

        sent.Append("Abc");
        sent.Append("Def");
        sent.Append("Ghi");

        obj.EvtWithArrayEvt += (object sender, Dummy.TestObjectEvtWithArrayEvt_Args e) => {
            received = e.arg;
        };

        obj.EmitEventWithArray(sent);

        Test.AssertEquals(sent.Length, received.Length);
        var pairs = sent.Zip(received, (string sentItem, string receivedItem) => new { Sent = sentItem, Received = receivedItem } );
        foreach (var pair in pairs)
            Test.AssertEquals(pair.Sent, pair.Received);
    }
}

class TestEventAddRemove
{
    public static void test_add_remove_event()
    {
        var obj = new Dummy.TestObject();
        bool called = true;

        EventHandler<Dummy.TestObjectEvtWithIntEvt_Args> evtCb = (object sender, Dummy.TestObjectEvtWithIntEvt_Args e) => {
            called = true;
        };

        obj.EvtWithIntEvt += evtCb;
        obj.EmitEventWithInt(42);
        Test.Assert(called);

        called = false;
        obj.EvtWithIntEvt -= evtCb;
        obj.EmitEventWithInt(42);
        Test.Assert(!called);
    }
}

class TestInterfaceEvents
{
    public static void test_nonconflicting_events()
    {
        var obj = new Dummy.TestObject();
        var called = false;

        EventHandler cb = (object sender, EventArgs e) => {
            called = true;
        };

        obj.NonconflictedEvt += cb;
        obj.EmitNonconflicted();
        Test.Assert(called);
    }
}

class TestEventNaming
{
    // For events named line focus_geometry,changed
    public static void test_event_naming()
    {
        var obj = new Dummy.TestObject();
        var test_called = false;

        EventHandler cb = (object sender, EventArgs e) => {
            test_called = true;
        };

        obj.EvtWithUnderEvt += cb;

        obj.EmitEventWithUnder();

        Test.Assert(test_called);

    }
}

class TestEventWithDeadWrappers
{

    private static WeakReference AttachToManager(Dummy.EventManager manager,
                                          EventHandler<Dummy.TestObjectEvtWithIntEvt_Args> cb)
    {
        var obj = new Dummy.TestObject();
        manager.Emitter = obj;

        obj.EvtWithIntEvt += cb;
        return new WeakReference(obj);
    }

    public static void test_event_from_c_owned_wrapper()
    {
        // Set upon object instantiation
        WeakReference wref = null;

        // Checks in the callback called
        bool callbackCalled = false;
        int received = -1;

        // attach to evt with int
        EventHandler<Dummy.TestObjectEvtWithIntEvt_Args> cb = (object sender, Dummy.TestObjectEvtWithIntEvt_Args args) => {
            callbackCalled = true;
            received = args.arg;
            Test.Assert(Object.ReferenceEquals(sender, wref.Target));
        };

        Dummy.EventManager manager = new Dummy.EventManager();
        wref = AttachToManager(manager, cb);

        Test.CollectAndIterate();

        manager.EmitWithInt(42);

        Test.CollectAndIterate();

        Test.Assert(callbackCalled, "Callback must have been called.");
        Test.AssertEquals(42, received, "Wrong value received.");

        // Cleanup checks
        manager.Release();

        // Make sure the released wrapper is collected and release the Eo object
        Test.CollectAndIterate();

        Test.AssertNull(wref.Target);
    }

}

}
