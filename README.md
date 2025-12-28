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
npm i bindings node-addon-api
npm i -D node-gyp ava
mkdir src lib test
touch src/main.cc
touch src/hello.cc
touch src/hello-method.cc
touch src/counter.hh
touch src/counter.cc
touch src/counter-object.hh
touch src/counter-object.cc 
touch src/heavy-calculation.cc
touch src/heavy-calculation-sync.cc
touch src/heavy-calculation-async.cc
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
            "cflags": ["-std=c++20"],
            "sources": [
                "src/counter-object.cc",
                "src/counter.cc",
                "src/heavy-calculation-async.cc",
                "src/heavy-calculation-sync.cc",
                "src/heavy-calculation.cc",
                "src/hello-method.cc",
                "src/hello.cc",
                "src/main.cc"
            ]
        }
    ]
}
```

_the target name must not contain '-'._

Once node dependencies are set, you're good to `configure`:

```bash
npx node-gyp configure
```

## NAPI

So far, we dealt with classic, v8-aware, addons and nan-powered ones.

The drawback of these options is that by going with them, your addon is bound to
v8 api and its changes.

NAPI was created to effectively detach node addons from that dependency, and
also offer more flexibility.

So, how napi differs from plain vi8 and nan?

First, install napi dependency:

```bash
npm i node-addon-api
```

Next, modify the `binding.gyp`:

```json
{
    "targets": [
        {
            "target_name": "sample_node_addon",
            "cflags": ["-std=c++20"],
            "cflags!": [ "-fno-exceptions" ],
            "cflags_cc!": [ "-fno-exceptions" ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")"
            ],
            "sources": [
                "src/counter-object.cc",
                "src/counter.cc",
                "src/heavy-calculation-async.cc",
                "src/heavy-calculation-sync.cc",
                "src/heavy-calculation.cc",
                "src/hello-method.cc",
                "src/hello.cc",
                "src/main.cc"
            ]
        }
    ]
}
```

Check the hello world now:

```cpp
// hello-method.cc

#include <napi.h>

// function prototype, could be a header file
std::string hello();

Napi::Value HelloMethod(const Napi::CallbackInfo &info)
{
  return Napi::String::New(info.Env(), hello());
}
```

The entrypoint also changes:

```cpp
// src/main.cc

#include <napi.h>

// function prototypes here for didatic purposes
Napi::Value HelloMethod(const Napi::CallbackInfo &);

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "hello"), Napi::Function::New(env, HelloMethod));
  return exports;
}

NODE_API_MODULE(addon, Init)
```

### Now, something completely different

Unlike plain v8 and nan, napi is a distinct api.

A better one, by the way.

It is not only stable since node 8, but also a C api, making it compatible with
any other language capable of C bindings, not only C++.

You can also use [CMake.js](https://napi.inspiredware.com/build-tools/cmake-js.html)
instead node-gyp, but we'll take this journey on another day.

### Objects, callbacks and more

## Further reading

- [Node Addons API](https://nodejs.org/api/addons.html)
- [Modern C++](https://github.com/federico-busato/Modern-CPP-Programming)
- [Mode module system](https://nodejs.org/api/packages.html)
- [V8 guide](https://v8docs.nodesource.com/node-22.4/)
- [C++ threads](https://en.cppreference.com/w/cpp/thread/thread.html)
-
