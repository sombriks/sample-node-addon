// src/main.cc

#include <nan.h>

#include "counter-object.hh"

// function prototypes here for didatic purposes
void HelloMethod(const Nan::FunctionCallbackInfo<v8::Value> &info);
void HeavyCalculationSync(const v8::FunctionCallbackInfo<v8::Value> &);
void HeavyCalculationAsync(const v8::FunctionCallbackInfo<v8::Value> &);

void Initialize(v8::Local<v8::Object> exports)
{
  v8::Local<v8::Context> context =
      exports->GetCreationContext().ToLocalChecked();

  exports->Set(context,
               Nan::New("hello").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(HelloMethod)
                   ->GetFunction(context)
                   .ToLocalChecked());
  CounterObject::Init(exports);
  NODE_SET_METHOD(exports, "heavyCalculationSync", HeavyCalculationSync);
  NODE_SET_METHOD(exports, "heavyCalculationAsync", HeavyCalculationAsync);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
