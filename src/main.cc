// src/main.cc

#include <napi.h>

#include "counter-object.hh"

#include "heavy-calculation-callback.hh"

// function prototypes here for didactic purposes
Napi::Value HelloMethod(const Napi::CallbackInfo &);

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "hello"), Napi::Function::New(env, HelloMethod));
  CounterObject::Init(env, exports);
  exports.Set(Napi::String::New(env, "heavyCalculation"), Napi::Function::New(env, HeavyCalculationCallback));
  return exports;
}

NODE_API_MODULE(addon, Init)