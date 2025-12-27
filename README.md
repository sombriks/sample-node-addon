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
npm i -D node-gyp ava nan
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
touch src/heavy-calculation-callback.cc
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
                "src/heavy-calculation-callback.cc",
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

## NAN addon

The [Native Abstractions for Node.js](https://github.com/nodejs/nan) is the
first attempt to solve the issue of breaking changes on v8 api that caused some
issues in the early days of node.

Though write an add using directly the v8 API would be simpler, node is v8 after
all, old node versions suffered with unstable api.

Nan wraps v8 calls with macros and wrapper types whenever possible.

To use nan, first install it as dependency:

```bash
npm i -D nan
```

Then modify your `bindings.byp` to _include_ it:

```json
{
    "targets": [
        {
            "target_name": "sample_node_addon",
            "cflags": ["-std=c++20"],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")"
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

Now you're good to go with nan.

### Changes from pure v8 addon to nan addon

A few changes can be done when writing addons with nan. Let's rewrite our hello
world addon:

```cpp
// src/hello-method.cc
// hello-method.cc
#include <nan.h>

// function prototype, could be a header file
std::string hello();

void HelloMethod(const Nan::FunctionCallbackInfo<v8::Value> &info)
{  
  info.GetReturnValue()
    .Set(Nan::New(hello().c_str())
    .ToLocalChecked());
}
```

Next, change the entrypoint of your addon as well:

```cpp
// src/main.cc
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
```

In a similar fashion, let's rewrite our counter object. First the header:

```cpp
// src/counter-object.hh
#ifndef COUNTER_OBJECT_HH
#define COUNTER_OBJECT_HH

#include <nan.h>

#include "counter.hh"

class CounterObject : public Nan::ObjectWrap
{
public:
  static void Init(v8::Local<v8::Object> exports);

private:
  static void New(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void Increment(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void Decrement(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void GetCount(const Nan::FunctionCallbackInfo<v8::Value> &info);

  ~CounterObject();
  explicit CounterObject();
  Counter *counter;
};

#endif // COUNTER_OBJECT_HH
```

Now the implementation:

```cpp
// src/counter-object.cc

#include "counter-object.hh"

CounterObject::CounterObject()
{
  counter = new Counter();
}

CounterObject::~CounterObject()
{
  delete counter;
}

void CounterObject::GetCount(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
  CounterObject *obj = ObjectWrap::Unwrap<CounterObject>(info.Holder());
  int count = obj->counter->getCount();
  info.GetReturnValue().Set(Nan::New(count));
}

void CounterObject::Decrement(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
  CounterObject *obj = ObjectWrap::Unwrap<CounterObject>(info.Holder());
  obj->counter->decrement();
}

void CounterObject::Increment(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
  CounterObject *obj = ObjectWrap::Unwrap<CounterObject>(info.Holder());
  obj->counter->increment();
}

void CounterObject::New(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
  if (info.IsConstructCall())
  {
    CounterObject *obj = new CounterObject();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }
  else
  {
    // this time, let's throw an error if not called as constructor
    Nan::ThrowTypeError("Must be called as a constructor with 'new'");
  }
}

void CounterObject::Init(v8::Local<v8::Object> exports)
{
  v8::Local<v8::Context> context = //
      exports->GetCreationContext().ToLocalChecked();
  Nan::HandleScope scope;

  auto name = Nan::New("Counter").ToLocalChecked();

  v8::Local<v8::FunctionTemplate> tpl = //
      Nan::New<v8::FunctionTemplate>(CounterObject::New);
  tpl->SetClassName(name);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "getCount", CounterObject::GetCount);
  Nan::SetPrototypeMethod(tpl, "increment", CounterObject::Increment);
  Nan::SetPrototypeMethod(tpl, "decrement", CounterObject::Decrement);

  exports->Set(context, name, tpl->GetFunction(context).ToLocalChecked());
}
```

So far, adopt nan seems quite the same than use v8 api directly, apart from
small simplifications, like the absence of `v8::Isolate` all around the code.

But for our heavy calculation example there is a few advantages. For example, it
is possible now to declare a callback version of the heavy calculation:

```cpp
```

## Further reading

- [Node Addons API](https://nodejs.org/api/addons.html)
- [Modern C++](https://github.com/federico-busato/Modern-CPP-Programming)
- [Mode module system](https://nodejs.org/api/packages.html)
- [V8 guide](https://v8docs.nodesource.com/node-22.4/)
- [C++ threads](https://en.cppreference.com/w/cpp/thread/thread.html)
-
