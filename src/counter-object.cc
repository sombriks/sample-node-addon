// src/counter-object.cc

#include "counter-object.hh"

CounterObject::CounterObject(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<CounterObject>(info)
{
  counter = new Counter();
}

CounterObject::~CounterObject()
{
  delete counter;
}

Napi::Value CounterObject::GetCount(const Napi::CallbackInfo &info)
{
  return Napi::Number::New(info.Env(), counter->getCount());
}

void CounterObject::Decrement(const Napi::CallbackInfo &info)
{
  counter->decrement();
}

void CounterObject::Increment(const Napi::CallbackInfo &info)
{
  counter->increment();
}

void CounterObject::Init(Napi::Env env, Napi::Object exports)
{
  Napi::Function func = DefineClass(
      env, "CounterObject",
      {
          CounterObject::InstanceMethod("increment", &CounterObject::Increment),
          CounterObject::InstanceMethod("decrement", &CounterObject::Decrement),
          CounterObject::InstanceMethod("getCount", &CounterObject::GetCount),
      });

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("Counter", func);
}