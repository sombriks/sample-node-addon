// src/sensor-sim-monitor.cc

#include "sensor-sim-monitor.hh"

/**
 * @brief initialization of the worker class
 * @param env node environment runtime
 * @param callback
 * @param sensorSim our data simulator
 */
SensorSimWorker::SensorSimWorker(Napi::Env env, Napi::Function &callback, SensorSim *sensorSim)
    : Napi::AsyncWorker(env),
      sensorSim(sensorSim),
      mon{Napi::ThreadSafeFunction::New(env, callback, "SensorSimMonitor", 0, 1)}
{
}

SensorSimWorker::~SensorSimWorker()
{
  this->mon.Release();
}

void SensorSimWorker::Execute()
{
  auto dataCallback = [this](const int data)
  {
    this->mon.BlockingCall(
        new int(data),
        [](Napi::Env env, Napi::Function jsCallback, int *data)
        {
          jsCallback.Call({Napi::Number::New(env, *data)});
          delete data;
        });
  };
  this->sensorSim->start(dataCallback);
}

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
}

SensorSimMonitor::~SensorSimMonitor()
{
  if (this->sensorSim != nullptr)
  {
    this->sensorSim->stop();
    delete this->sensorSim;
    this->sensorSim = nullptr;
  }
}

void SensorSimMonitor::StartMonitoring(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::Function callback = info[0].As<Napi::Function>();

  if (this->sensorSim == nullptr)
  {
    this->sensorSim = new SensorSim();
    // create the worker
    SensorSimWorker *worker = new SensorSimWorker(env, callback, this->sensorSim);
    worker->Queue();
  }
  else
  {
    Napi::TypeError::New(env, "SensorSim is already running").ThrowAsJavaScriptException();
  }
}
void SensorSimMonitor::StopMonitoring(const Napi::CallbackInfo &info)
{
  if (this->sensorSim != nullptr)
  {
    this->sensorSim->stop();
    delete this->sensorSim;
    this->sensorSim = nullptr;
  }
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