// src/main.cc

#include <node.h>
#include "counter-object.hh"

// function prototypes here for didatic purposes
void HelloMethod(const v8::FunctionCallbackInfo<v8::Value> &);
void HeavyCalculationSync(const v8::FunctionCallbackInfo<v8::Value> &);
void HeavyCalculationAsync(const v8::FunctionCallbackInfo<v8::Value> &);

void Initialize(v8::Local<v8::Object> exports)
{
  NODE_SET_METHOD(exports, "hello", HelloMethod);
  CounterObject::Init(exports);
  NODE_SET_METHOD(exports, "heavyCalculationSync", HeavyCalculationSync);
  NODE_SET_METHOD(exports, "heavyCalculationAsync", HeavyCalculationAsync);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
