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
touch src/counter.hh
touch src/counter.cc
touch src/counter-object.hh
touch src/counter-object.cc 
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

## Objects

In a similar way to native functions, you cn define native objects, wrap them
using the node addon api and do stuff in the javascript side.

This simple example defines a counter object, featuring private state and public
methods:

```cpp
// src/counter.hh
#ifndef COUNTER_HH
#define COUNTER_HH

class Counter
{
private:
  int count;

public:
  Counter();
  Counter(int);
  ~Counter();
  void increment();
  void decrement();
  int getCount();
};
#endif // COUNTER_HH
```

And the implementation:

```cpp
// src/counter.cc

#include <iostream>
#include "counter.hh"

Counter::Counter() : count(0)
{
  std::cout << "Counter initialized to zero." << std::endl;
}

Counter::Counter(int initial) : count(initial)
{
  std::cout << "Counter initialized to " << initial << "." << std::endl;
}

Counter::~Counter()
{
  std::cout << "Counter destroyed." << std::endl;
}

void Counter::increment()
{
  ++count;
  std::cout << "Counter incremented to " << count << "." << std::endl;
}

void Counter::decrement()
{
  --count;
  std::cout << "Counter decremented to " << count << "." << std::endl;
}

int Counter::getCount()
{
  std::cout << "Current counter value: " << count << "." << std::endl;
  return count;
}
```

Like as you did with the function, you must wrap your counter using the node
addon API:

```cpp
// src/counter-object.hh
#ifndef COUNTER_OBJECT_HH
#define COUNTER_OBJECT_HH

#include <node.h>
#include <node_object_wrap.h>

#include "counter.hh"

class CounterObject : public node::ObjectWrap
{
public:
  static void Init(v8::Local<v8::Object> exports);

private:
  static void New(const v8::FunctionCallbackInfo<v8::Value> &args);
  static void Increment(const v8::FunctionCallbackInfo<v8::Value> &args);
  static void Decrement(const v8::FunctionCallbackInfo<v8::Value> &args);
  static void GetCount(const v8::FunctionCallbackInfo<v8::Value> &args);

  ~CounterObject();
  explicit CounterObject();
  Counter *counter;
};

#endif // COUNTER_OBJECT_HH
```

This will be the default structure for wrappers:

- extra `<node_object_wrap.h>` header file
- the `static void Init(v8::Local<v8::Object> exports);` as sole public native
  interface
- actual wrapped interface as private, static methods
- The `static void New(const v8::FunctionCallbackInfo<v8::Value> &args);` to
  define a proper constructor for your wrapped object

The implementation follows:

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

void CounterObject::GetCount(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();
  CounterObject *obj = ObjectWrap::Unwrap<CounterObject>(args.Holder());
  int count = obj->counter->getCount();
  args.GetReturnValue().Set(v8::Number::New(isolate, count));
}

void CounterObject::Decrement(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  CounterObject *obj = ObjectWrap::Unwrap<CounterObject>(args.Holder());
  obj->counter->decrement();
}

void CounterObject::Increment(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  CounterObject *obj = ObjectWrap::Unwrap<CounterObject>(args.Holder());
  obj->counter->increment();
}

void CounterObject::New(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  if (args.IsConstructCall())
  {
    CounterObject *obj = new CounterObject();
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  }
  else
  {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    // we could just throw an error, but let's be more friendly.
    //
    // grab a context
    v8::Local<v8::Context> context = args.GetIsolate()->GetCurrentContext();
    // create a new arguments array
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {args[0]};
    // lots of casts to get the the function call
    v8::Local<v8::Function> cons =
        args.Data()
            .As<v8::Object>()
            ->GetInternalField(0)
            .As<v8::Value>()
            .As<v8::Function>();
    // invoke as constructor
    v8::Local<v8::Object> result =
        cons->NewInstance(context, argc, argv).ToLocalChecked();
    args.GetReturnValue().Set(result);
  }
}

void CounterObject::Init(v8::Local<v8::Object> exports)
{
  v8::Isolate *isolate = exports->GetIsolate();

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl =
      v8::FunctionTemplate::New(isolate, CounterObject::New, exports);
  tpl->SetClassName(v8::String::NewFromUtf8(isolate, "Counter").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "increment", CounterObject::Increment);
  NODE_SET_PROTOTYPE_METHOD(tpl, "decrement", CounterObject::Decrement);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getCount", CounterObject::GetCount);

  v8::Local<v8::Function> constructor =
      tpl->GetFunction(isolate->GetCurrentContext()).ToLocalChecked();
  // Store the constructor in the exports object
  exports
      ->Set(isolate->GetCurrentContext(),
            v8::String::NewFromUtf8(isolate, "Counter").ToLocalChecked(),
            constructor)
      .Check();
}
```

Verbose, but not hard.

Then registry the wrapper in the module:

```cpp
// src/main.cc

#include <node.h>
#include "counter-object.hh"

// function prototype, can reside into a header file
void HelloMethod(const v8::FunctionCallbackInfo<v8::Value> &);

void Initialize(v8::Local<v8::Object> exports)
{
  NODE_SET_METHOD(exports, "hello", HelloMethod);
  CounterObject::Init(exports);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
```

## Running the sample

Modify the `lib/main.js` to see the magic happening:

```javascript
// lib/main.js
import bindings from "bindings";

const addon = bindings("sample_node_addon");

let counter = new addon.Counter();
console.log("Initial count:", counter.getCount());

counter.increment();
console.log("After increment:", counter.getCount());

counter.increment();
console.log("After another increment:", counter.getCount());

counter.decrement();
console.log("After decrement:", counter.getCount());

if(global.gc) {
  counter = null;
  console.log("Triggering garbage collection...");
  global.gc();
  console.log("Counter object deleted.");
}

console.log("addon.hello():", addon.hello());
```

To check if the native object is properly destroyed, run the script using the
`--expose-gc` flag:

```bash
node --expose-gc lib/main.js
```

## Further reading

- [Node Addons API](https://nodejs.org/api/addons.html)
- [Modern C++](https://github.com/federico-busato/Modern-CPP-Programming)
- [Mode module system](https://nodejs.org/api/packages.html)
- [V8 guide](https://v8docs.nodesource.com/node-22.4/)
-
