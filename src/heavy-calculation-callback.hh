// src/heavy-calculation-callback.hh

#ifndef HEAVY_CALCULATION_CALLBACK_HH
#define HEAVY_CALCULATION_CALLBACK_HH

#include <napi.h>

class HeavyCalculationWorker : public Napi::AsyncWorker
{
public:
  HeavyCalculationWorker(Napi::Function &callback, int n);
  void Execute() override;
  void OnOK() override;

private:
  int n;
  int result;
};

Napi::Value HeavyCalculationCallback(const Napi::CallbackInfo &);

#endif // HEAVY_CALCULATION_CALLBACK_HH