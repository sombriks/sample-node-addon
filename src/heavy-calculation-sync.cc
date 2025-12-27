#include <node.h>
#include "heavy-calculation.hh"

void HeavyCalculationSync(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();

  if (args.Length() < 1 || !args[0]->IsNumber())
  {
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Argument must be a number").ToLocalChecked());
    return;
  }

  int n = args[0]->Int32Value(isolate->GetCurrentContext()).FromJust();

  int result = heavyCalculation(n);

  args.GetReturnValue().Set(v8::Integer::New(isolate, result));
}