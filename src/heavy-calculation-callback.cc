// src/heavy-calculation-callback.cc

#include "heavy-calculation.hh"

#include "heavy-calculation-callback.hh"

void HeavyCalculationCallback(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
  int n = info[0].As<v8::Number>()->Value();
  v8::Local<v8::Function> cb = info[1].As<v8::Function>();

  Nan::Callback *callback = new Nan::Callback(cb);
  HeavyCalculationWorker *worker = new HeavyCalculationWorker(n, callback);
  Nan::AsyncQueueWorker(worker);
}

void HeavyCalculationWorker::Execute()
{
  result = heavyCalculation(n);
}

void HeavyCalculationWorker::HandleOKCallback()
{
  Nan::HandleScope scope;

  v8::Local<v8::Value> argv[] = {
      Nan::New(result)};

  callback->Call(1, argv, async_resource);
}
