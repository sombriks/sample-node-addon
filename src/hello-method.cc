// hello-method.cc
#include <nan.h>

// function prototype, could be a header file
std::string hello();

void HelloMethod(const Nan::FunctionCallbackInfo<v8::Value> &info)
{  
  info.GetReturnValue().Set(Nan::New(hello().c_str()).ToLocalChecked());
}