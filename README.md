# [sample-node-addon](https://github.com/sombriks/sample-node-addon)

Sample code exploring distinct scenarios of the C++ / javascript bridge in the
node runtime.

## Requirements

- Node 22
- C++ compiler compatible with modern (C++20) standard, like gcc or clang
- GNU Make
- Python 3.12 or newer

## Initial setup

```bash
npm init -y
npm i bindings
npm i -D node-gyp ava
mkdir src lib test
touch src/main.cc
touch src/hello.cc
touch src/hello-method.cc
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

And this is all the code needed to call C++ code from node.

But a few more steps remain.

### Update `bindings.gyp`

```json
{
    "targets": [
        {
            "target_name": "sample_node_addon",
            "cflags": ["-std=c++20"],
            "sources": [
              "src/hello-method.cc",
              "src/hello.cc", 
              "src/main.cc"
            ]
        }
    ]
}
```

You must list all your source files.

You can also define some compiler flags.

Next, clean upm configure and build:

```bash
npx node-gyp clean
npx node-gyp configure
npx node-gyp build
```

### Call your hello world

In `lib/main.js` make the magic happen:

```javascript
// lib/main.js
import bindings from "bindings";

const addon = bindings("sample_node_addon");

console.log("addon.hello():", addon.hello());
```

The [bindings](https://github.com/TooTallNate/node-bindings) dependency helps to
locate the correct binary over the platforms.

Before finally run, make sure you have enabled the `import module` style on the
project:

```bash
npm pkg set type="module"
```

Finally, call the hello world:

```bash
node lib/main.js
```

## Further reading

- [Node Addons API](https://nodejs.org/api/addons.html)
- [Modern C++](https://github.com/federico-busato/Modern-CPP-Programming)
- [Mode module system](https://nodejs.org/api/packages.html)
-
