// src/main.cc

#include <napi.h>

// function prototypes here for didatic purposes
Napi::Value HelloMethod(const Napi::CallbackInfo &);

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "hello"), Napi::Function::New(env, HelloMethod));
  return exports;
}

NODE_API_MODULE(addon, Init)