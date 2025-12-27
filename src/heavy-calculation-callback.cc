// src/heavy-calculation-callback.cc

#include <nan.h>

#include "heavy-calculation.hh"

void HeavyCalculationCallback(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
  // check the numer of arguments passed.
  if (info.Length() < 2)
  {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  int n = info[0].As<v8::Number>()->Value();
  v8::Local<v8::Function> cb = info[1].As<v8::Function>();

  Nan::AsyncResource resource("nan:makeCallback");

  int result = heavyCalculation(n);

  v8::Local<v8::Value> argv[] = {Nan::New(result)};
  resource.runInAsyncScope(Nan::GetCurrentContext()->Global(), cb, 1, argv);
}