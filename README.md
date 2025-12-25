# [sample-node-addon](https://github.com/sombriks/sample-node-addon)

Sample code exploring distinct scenarios of the C++ / javascript bridge in the
node runtime.

## Requirements

- Node 22
- C++ compiler compatible with modern (C++20) standard, like gcc or clang
- Make
- Python 3.12 or newer

## Initial setup

```bash
npm init -y
npm i bindings
npm i -D node-gyp ava
mkdir src lib test
touch src/main.cc
touch lib/main.js
touch test/main.spec.js
touch binding.gyp
```

Set the content of the `binding.gyp` file:

```json
{
  "targets": [
    {
      "target_name": "sample_node_addon",
      "sources": [ "src/main.cc" ]
    }
  ]
}
```

_the target name must not contain '-'._

Once node dependencies are set, you're good to `configure`:

```bash
npx node-gyp configure
```

## Functions

Once the entrypoint for the module is configured, we can add our functions into it.

Let's start with this simple, C++20 compatible  _hello world_:

```cpp
// hello.cc
#include <string>

std::string hello() 
{
  return std::string("hello world!");
}
```

In order to consume it, you must wrap the call into a node method:

```cpp
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
```

And finally, register the method into the module:

```cpp
// main.cc
#include <node.h>

// function prototype, can reside into a header file
void HelloMethod(const v8::FunctionCallbackInfo<v8::Value> &);

void Initialize(v8::Local<v8::Object> exports)
{  
  NODE_SET_METHOD(exports, "hello", HelloMethod);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
```

## Further reading

- [Node Addons API](https://nodejs.org/api/addons.html)
- [Modern C++](https://github.com/federico-busato/Modern-CPP-Programming)
-
