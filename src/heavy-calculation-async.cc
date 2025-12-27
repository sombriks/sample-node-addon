// src/heavy-calculation-async.cc
#include <node.h>

#include "heavy-calculation.hh"

void HeavyCalculationAsync(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();

  if (args.Length() < 1 || !args[0]->IsNumber())
  {
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Argument must be a number").ToLocalChecked());
    return;
  }
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  // get the param
  int n = args[0]->Int32Value(context).FromJust();
  // prepare a promise
  v8::Local<v8::Promise::Resolver> resolver = v8::Promise::Resolver::New(context).ToLocalChecked();
  args.GetReturnValue().Set(resolver->GetPromise());
  // do the heavy calculation
  int result = heavyCalculation(n);
  // resolve the promise
  resolver
      ->Resolve(context, v8::Integer::New(isolate, result))
      .ToChecked();
}