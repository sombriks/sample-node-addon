// src/heavy-calculation-callback.cc

#include "heavy-calculation.hh"

#include "heavy-calculation-callback.hh"

HeavyCalculationWorker::HeavyCalculationWorker(Napi::Function &callback, int n)
    : Napi::AsyncWorker(callback)
{
  this->n = n;
  this->result = 0;
}

void HeavyCalculationWorker::Execute()
{
  this->result = heavyCalculation(this->n);
}

void HeavyCalculationWorker::OnOK()
{
  Napi::HandleScope scope(Env());
  Callback().Call({Napi::Number::New(Env(), this->result)});
}

Napi::Value HeavyCalculationCallback(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  int n = info[0].As<Napi::Number>().Int32Value();
  Napi::Function callback = info[1].As<Napi::Function>();

  HeavyCalculationWorker *worker = new HeavyCalculationWorker(callback, n);
  worker->Queue();

  return env.Undefined();
}