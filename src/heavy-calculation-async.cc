// src/heavy-calculation-async.cc
#include <node.h>
#include <thread>
#include "heavy-calculation.hh"

void HeavyCalculationAsync(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();

  if (args.Length() < 1 || !args[0]->IsNumber())
  {
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Argument must be a number").ToLocalChecked());
    return;
  }
  int n = args[0]->Int32Value(isolate->GetCurrentContext()).FromJust();

  // prepare a promiste
  v8::Local<v8::Promise::Resolver> resolver = //
      v8::Promise::Resolver::New(isolate->GetCurrentContext()).ToLocalChecked();
  args.GetReturnValue()
      .Set(resolver->GetPromise());
  // add a lambda to a thread to resolve the promise after the heavy calculation
  std::thread([isolate, resolver, n]()
              {
    int result = heavyCalculation(n);
    // we need to enter the isolate to set the promise result
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    resolver
        ->Resolve(context, v8::Integer::New(isolate, result))
        .ToChecked(); })
      .detach();
}