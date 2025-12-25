// src/counter-object.cc

#include "counter-object.hh"

CounterObject::CounterObject()
{
  counter = new Counter();
}

CounterObject::~CounterObject()
{
  delete counter;
}

void CounterObject::GetCount(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();
  CounterObject *obj = ObjectWrap::Unwrap<CounterObject>(args.Holder());
  int count = obj->counter->getCount();
  args.GetReturnValue().Set(v8::Number::New(isolate, count));
}

void CounterObject::Decrement(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  CounterObject *obj = ObjectWrap::Unwrap<CounterObject>(args.Holder());
  obj->counter->decrement();
}

void CounterObject::Increment(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  CounterObject *obj = ObjectWrap::Unwrap<CounterObject>(args.Holder());
  obj->counter->increment();
}

void CounterObject::New(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  if (args.IsConstructCall())
  {
    CounterObject *obj = new CounterObject();
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  }
  else
  {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    // we could just throw an error, but let's be more friendly.
    //
    // grab a context
    v8::Local<v8::Context> context = args.GetIsolate()->GetCurrentContext();
    // create a new arguments array
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {args[0]};
    // lots of casts to get the the function call
    v8::Local<v8::Function> cons =
        args.Data()
            .As<v8::Object>()
            ->GetInternalField(0)
            .As<v8::Value>()
            .As<v8::Function>();
    // invoke as constructor
    v8::Local<v8::Object> result =
        cons->NewInstance(context, argc, argv).ToLocalChecked();
    args.GetReturnValue().Set(result);
  }
}

void CounterObject::Init(v8::Local<v8::Object> exports)
{
  v8::Isolate *isolate = exports->GetIsolate();

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl =
      v8::FunctionTemplate::New(isolate, CounterObject::New, exports);
  tpl->SetClassName(v8::String::NewFromUtf8(isolate, "Counter").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "increment", CounterObject::Increment);
  NODE_SET_PROTOTYPE_METHOD(tpl, "decrement", CounterObject::Decrement);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getCount", CounterObject::GetCount);

  v8::Local<v8::Function> constructor =
      tpl->GetFunction(isolate->GetCurrentContext()).ToLocalChecked();
  // Store the constructor in the exports object
  exports
      ->Set(isolate->GetCurrentContext(),
            v8::String::NewFromUtf8(isolate, "Counter").ToLocalChecked(),
            constructor)
      .Check();
}