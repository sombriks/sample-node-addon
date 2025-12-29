// src/sensor-sim-monitor.cc

#include "sensor-sim-monitor.hh"

void SensorSimMonitor::Init(Napi::Env env, Napi::Object exports)
{
  Napi::Function func = DefineClass(
      env,
      "SensorSimMonitor",
      {
          InstanceMethod("startMonitoring", &SensorSimMonitor::StartMonitoring),
          InstanceMethod("stopMonitoring", &SensorSimMonitor::StopMonitoring),
          InstanceMethod("isMonitoring", &SensorSimMonitor::IsMonitoring),
      });

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("SensorSimMonitor", func);
}

SensorSimMonitor::SensorSimMonitor(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<SensorSimMonitor>(info)
{
  this->sensorSim = nullptr;
  this->tsfn = nullptr;
}

SensorSimMonitor::~SensorSimMonitor()
{
  this->stop();
}

Napi::Value SensorSimMonitor::IsMonitoring(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  bool monitoring = false;
  if (this->sensorSim != nullptr)
  {
    monitoring = this->sensorSim->isRunning();
  }
  return Napi::Boolean::New(env, monitoring);
}

void SensorSimMonitor::StartMonitoring(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::Function jsCallback = info[0].As<Napi::Function>();

  if (this->sensorSim == nullptr)
  {
    this->sensorSim = new SensorSim();
    this->tsfn = new Napi::ThreadSafeFunction(Napi::ThreadSafeFunction::New(env, jsCallback, "SensorSimMonitor", 0, 1));
    auto dataCallback = [this](const int data)
    {
      this->tsfn->BlockingCall(
          new int(data),
          [](Napi::Env env, Napi::Function jsCallback, int *data)
          {
            jsCallback.Call({Napi::Number::New(env, *data)});
            delete data;
          });
    };
    this->sensorSim->start(dataCallback);
  }
  else
  {
    Napi::TypeError::New(env, "SensorSim is already running")
        .ThrowAsJavaScriptException();
  }
}

void SensorSimMonitor::StopMonitoring(const Napi::CallbackInfo &info)
{
  this->stop();
}

void SensorSimMonitor::stop()
{
  if (this->sensorSim != nullptr)
  {
    this->sensorSim->stop();
    delete this->sensorSim;
    this->sensorSim = nullptr;
  }
  if (this->tsfn != nullptr)
  {
    this->tsfn->Release();
    delete this->tsfn;
    this->tsfn = nullptr;
  }
}