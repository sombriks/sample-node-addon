// hello-method.cc
#include <node.h>
#include <string>

// function prototype, could be a header file
std::string hello();

void HelloMethod(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();
  args.GetReturnValue()
      .Set(v8::String::NewFromUtf8(isolate, hello().c_str()).ToLocalChecked());
}