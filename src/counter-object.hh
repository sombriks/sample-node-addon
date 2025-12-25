// src/counter-object.hh
#ifndef COUNTER_OBJECT_HH
#define COUNTER_OBJECT_HH

#include <node.h>
#include <node_object_wrap.h>

#include "counter.hh"

class CounterObject : public node::ObjectWrap
{
public:
  static void Init(v8::Local<v8::Object> exports);

private:
  Counter *counter;
  explicit CounterObject();
  ~CounterObject();

  static void New(const v8::FunctionCallbackInfo<v8::Value> &args);
  static void Increment(const v8::FunctionCallbackInfo<v8::Value> &args);
  static void Decrement(const v8::FunctionCallbackInfo<v8::Value> &args);
  static void GetCount(const v8::FunctionCallbackInfo<v8::Value> &args);
};

#endif // COUNTER_OBJECT_HH