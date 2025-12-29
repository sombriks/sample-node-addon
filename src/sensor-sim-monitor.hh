// src/sensor-sim-monitor.hh
#ifndef SENSOR_SIM_MONITOR_HH
#define SENSOR_SIM_MONITOR_HH

#include <napi.h>

#include "sensor-sim.hh"

/**
 * @brief basic wrapper class for the sensor simulator
 */
class SensorSimMonitor : public Napi::ObjectWrap<SensorSimMonitor>
{
public:
  static void Init(Napi::Env env, Napi::Object);
  SensorSimMonitor(const Napi::CallbackInfo &);
  ~SensorSimMonitor();

private:
  void StartMonitoring(const Napi::CallbackInfo &);
  void StopMonitoring(const Napi::CallbackInfo &);
  Napi::Value IsMonitoring(const Napi::CallbackInfo &);

  void stop();
  SensorSim *sensorSim;
  Napi::ThreadSafeFunction *tsfn;
};
#endif // SENSOR_SIM_MONITOR_HH
