// src/sensor-sim-callback.hh
#ifndef SENSOR_SIM_CALLBACK_HH
#define SENSOR_SIM_CALLBACK_HH

#include <iostream>
#include <functional>
#include <node.h>
#include <uv.h>

struct AsyncData
{
  uv_async_t async;
  v8::Persistent<v8::Function> callback;
  v8::Persistent<v8::Context> context;
  v8::Isolate* isolate_ptr;
  int value;
};

void sensorWatch(void(int, void *), void *);

void WatchSensor(const v8::FunctionCallbackInfo<v8::Value> &);

#endif // SENSOR_SIM_CALLBACK_HH
