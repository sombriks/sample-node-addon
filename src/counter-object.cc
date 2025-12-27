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

void CounterObject::GetCount(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
  CounterObject *obj = ObjectWrap::Unwrap<CounterObject>(info.Holder());
  int count = obj->counter->getCount();
  info.GetReturnValue().Set(Nan::New(count));
}

void CounterObject::Decrement(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
  CounterObject *obj = ObjectWrap::Unwrap<CounterObject>(info.Holder());
  obj->counter->decrement();
}

void CounterObject::Increment(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
  CounterObject *obj = ObjectWrap::Unwrap<CounterObject>(info.Holder());
  obj->counter->increment();
}

void CounterObject::New(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
  if (info.IsConstructCall())
  {
    CounterObject *obj = new CounterObject();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }
  else
  {
    // this time, let's throw an error if not called as constructor
    Nan::ThrowTypeError("Must be called as a constructor with 'new'");
  }
}

void CounterObject::Init(v8::Local<v8::Object> exports)
{
  v8::Local<v8::Context> context = //
      exports->GetCreationContext().ToLocalChecked();
  Nan::HandleScope scope;

  auto name = Nan::New("Counter").ToLocalChecked();

  v8::Local<v8::FunctionTemplate> tpl = //
      Nan::New<v8::FunctionTemplate>(CounterObject::New);
  tpl->SetClassName(name);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "getCount", CounterObject::GetCount);
  Nan::SetPrototypeMethod(tpl, "increment", CounterObject::Increment);
  Nan::SetPrototypeMethod(tpl, "decrement", CounterObject::Decrement);

  exports->Set(context, name, tpl->GetFunction(context).ToLocalChecked());
}