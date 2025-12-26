// src/main.cc

#include <node.h>
#include "counter-object.hh"
#include "sensor-sim-callback.hh"

// function prototype, can reside into a header file
void HelloMethod(const v8::FunctionCallbackInfo<v8::Value> &);

void Initialize(v8::Local<v8::Object> exports)
{
  NODE_SET_METHOD(exports, "hello", HelloMethod);
  CounterObject::Init(exports);
  NODE_SET_METHOD(exports, "watchSensor", WatchSensor);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
