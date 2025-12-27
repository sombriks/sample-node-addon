// src/counter-object.hh
#ifndef COUNTER_OBJECT_HH
#define COUNTER_OBJECT_HH

#include <nan.h>

#include "counter.hh"

class CounterObject : public Nan::ObjectWrap
{
public:
  static void Init(v8::Local<v8::Object> exports);

private:
  static void New(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void Increment(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void Decrement(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void GetCount(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static Nan::Persistent<v8::Function> constructor;

  ~CounterObject();
  explicit CounterObject();
  Counter *counter;
};

#endif // COUNTER_OBJECT_HH