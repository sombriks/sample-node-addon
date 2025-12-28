// src/heavy-calculation-callback.hh
#ifndef HEAVY_CALCULATION_CALLBACK_HH
#define HEAVY_CALCULATION_CALLBACK_HH

#include <nan.h>

void HeavyCalculationCallback(const Nan::FunctionCallbackInfo<v8::Value> &info);

class HeavyCalculationWorker : public Nan::AsyncWorker
{
public:
  HeavyCalculationWorker(int n, Nan::Callback *callback)
      : Nan::AsyncWorker(callback), n(n), result(0) {}

  void Execute() override;
  void HandleOKCallback() override;

private:
  int n;
  int result;
};

#endif // HEAVY_CALCULATION_CALLBACK_HH
