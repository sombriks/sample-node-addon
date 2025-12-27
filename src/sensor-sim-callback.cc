// src/sensor-sim-callback.cc
#include "sensor-sim-callback.hh"

void WatchSensor(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();

  // sensorWatch(
  //     [](int v, void *userData) {

  //     },
  //     asyncData);
}
