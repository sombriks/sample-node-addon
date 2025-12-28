// src/counter-object.hh
#ifndef COUNTER_OBJECT_HH
#define COUNTER_OBJECT_HH

#include <napi.h>

#include "counter.hh"

class CounterObject : public Napi::ObjectWrap<CounterObject>
{
public:
  static void Init(Napi::Env env, Napi::Object exports);
  CounterObject(const Napi::CallbackInfo &info);
  ~CounterObject();

private:
  void Increment(const Napi::CallbackInfo &);
  void Decrement(const Napi::CallbackInfo &);
  Napi::Value GetCount(const Napi::CallbackInfo &);

  Counter *counter;
};

#endif // COUNTER_OBJECT_HH