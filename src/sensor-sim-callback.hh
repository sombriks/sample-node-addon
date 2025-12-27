// src/sensor-sim-callback.hh
#ifndef SENSOR_SIM_CALLBACK_HH
#define SENSOR_SIM_CALLBACK_HH

#include <iostream>
#include <future>
#include <node.h>

void sensorWatch(void(int, void *), void *);

void WatchSensor(const v8::FunctionCallbackInfo<v8::Value> &);

#endif // SENSOR_SIM_CALLBACK_HH
