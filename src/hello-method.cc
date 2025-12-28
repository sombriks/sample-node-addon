// hello-method.cc

#include <napi.h>

// function prototype, could be a header file
std::string hello();

Napi::Value HelloMethod(const Napi::CallbackInfo &info)
{
  return Napi::String::New(info.Env(), hello());
}
