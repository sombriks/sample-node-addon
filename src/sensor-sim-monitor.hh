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

  SensorSim *sensorSim;
};

/**
 * @brief async worker to handle the data produced by the sensor simulator.
 * must be created and discarded correctly during the lifetime of the javascript
 * object.
 */
class SensorSimWorker : public Napi::AsyncWorker
{
public:
  SensorSimWorker(Napi::Env env, Napi::Function &, SensorSim *);
  ~SensorSimWorker();
  void Execute() override;

private:
  SensorSim *sensorSim;
  Napi::ThreadSafeFunction mon;
};

#endif // SENSOR_SIM_MONITOR_HH