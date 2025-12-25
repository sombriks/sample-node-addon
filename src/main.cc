#include <node.h>

void Initialize(v8::Local<v8::Object> exports)
{
  // This is where you would add your addon initialization code.
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
