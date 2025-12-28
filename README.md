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
touch src/heavy-calculation-callback.cc
touch src/sensor-sim.cc
touch src/sensor-sim-monitor.cc
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
                "src/heavy-calculation-callback.cc",
                "src/heavy-calculation.cc",
                "src/hello-method.cc",
                "src/hello.cc",
                "src/main.cc",
                "src/sensor-sim-monitor.cc",
                "src/sensor-sim.cc"
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
                "src/heavy-calculation-callback.cc",
                "src/heavy-calculation.cc",
                "src/hello-method.cc",
                "src/hello.cc",
                "src/main.cc",
                "src/sensor-sim-monitor.cc",
                "src/sensor-sim.cc"
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

### Classes, callbacks and more

The way of defining classes also got simpler. First the header:

```cpp
// src/counter-object.hh
#ifndef COUNTER_OBJECT_HH
#define COUNTER_OBJECT_HH

#include <napi.h>

#include "counter.hh"

class CounterObject : public Napi::ObjectWrap<CounterObject>
{
public:
  static void Init(Napi::Env env, Napi::Object exports);
  CounterObject(const Napi::CallbackInfo &info);
  ~CounterObject();

private:
  void Increment(const Napi::CallbackInfo &);
  void Decrement(const Napi::CallbackInfo &);
  Napi::Value GetCount(const Napi::CallbackInfo &);

  Counter *counter;
};

#endif // COUNTER_OBJECT_HH
```

The implementation goes like this:

```cpp
// src/counter-object.cc

#include "counter-object.hh"

CounterObject::CounterObject(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<CounterObject>(info)
{
  counter = new Counter();
}

CounterObject::~CounterObject()
{
  delete counter;
}

Napi::Value CounterObject::GetCount(const Napi::CallbackInfo &info)
{
  return Napi::Number::New(info.Env(), counter->getCount());
}

void CounterObject::Decrement(const Napi::CallbackInfo &info)
{
  counter->decrement();
}

void CounterObject::Increment(const Napi::CallbackInfo &info)
{
  counter->increment();
}

void CounterObject::Init(Napi::Env env, Napi::Object exports)
{
  Napi::Function func = DefineClass(
      env, "CounterObject",
      {
          CounterObject::InstanceMethod("increment", &CounterObject::Increment),
          CounterObject::InstanceMethod("decrement", &CounterObject::Decrement),
          CounterObject::InstanceMethod("getCount", &CounterObject::GetCount),
      });

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("Counter", func);
}
```

And it can be registered on the entry point pretty much like this:

```cpp
// src/main.cc

#include <napi.h>

#include "counter-object.hh"

// function prototypes here for didatic purposes
Napi::Value HelloMethod(const Napi::CallbackInfo &);

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "hello"), Napi::Function::New(env, HelloMethod));
  CounterObject::Init(env, exports);
  return exports;
}

NODE_API_MODULE(addon, Init)
```

Finally, our callback version for the heavy operation goes like this:

```cpp
// src/heavy-calculation-callback.hh

#ifndef HEAVY_CALCULATION_CALLBACK_HH
#define HEAVY_CALCULATION_CALLBACK_HH

#include <napi.h>

class HeavyCalculationWorker : public Napi::AsyncWorker
{
public:
  HeavyCalculationWorker(Napi::Function &callback, int n);
  void Execute() override;
  void OnOK() override;

private:
  int n;
  int result;
};

// function signature
Napi::Value HeavyCalculationCallback(const Napi::CallbackInfo &);

#endif // HEAVY_CALCULATION_CALLBACK_HH
```

Implementation goes like this:

```cpp
// src/heavy-calculation-callback.cc

#include "heavy-calculation.hh"

#include "heavy-calculation-callback.hh"

HeavyCalculationWorker::HeavyCalculationWorker(Napi::Function &callback, int n)
    : Napi::AsyncWorker(callback)
{
  this->n = n;
  this->result = 0;
}

void HeavyCalculationWorker::Execute()
{
  this->result = heavyCalculation(this->n);
}

void HeavyCalculationWorker::OnOK()
{
  Napi::HandleScope scope(Env());
  Callback().Call({Napi::Number::New(Env(), this->result)});
}

Napi::Value HeavyCalculationCallback(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  int n = info[0].As<Napi::Number>().Int32Value();
  Napi::Function callback = info[1].As<Napi::Function>();

  HeavyCalculationWorker *worker = new HeavyCalculationWorker(callback, n);
  worker->Queue();

  return env.Undefined();
}
```

Function can be registered in the entry point pretty much like how you already
did with previous examples and _voilá_, heavywork offloaded from main thread.

To test if it is really off the main thread, test it like this:

```javascript
// test/main.spec.js
import test from "ava"

import { hello, Counter, heavyCalculation } from "../lib/main.js"

// other testcases

test("Should perform a heavy calculation, native callback + promise", async t => {
  const value = await Promise.all([
    new Promise(resolve => heavyCalculation(15, resolve)), 
    new Promise(resolve => heavyCalculation(21, resolve))
  ])
  t.like(value, [30, 42])
})
```

since the heavy calculations will occur concurrently, execution time will not
add up:

```
  ✔ Should get hello world
  ✔ Should create and use Counter
  ✔ Should perform a heavy calculation, native callback + promise (3s)
```

### The inverse path

One cool feature of napi is the mechanism available to allow native threads to
communicate with node event loop.

our previous example offloads work, but still got all control over results.

Napi has this neat api which allows to receive data from outside.

Consider this sensor simulator:

```cpp
#ifndef SENSOR_SIM_HH
#define SENSOR_SIM_HH

#include <string>
#include <thread>
#include <chrono>
#include <random>
#include <iostream>
#include <functional>

class SensorSim
{
  public:
    SensorSim(std::function<void(const int)> &);
    ~SensorSim();
    void start();
    void stop();
  private:
    bool running;
    std::function<void(const int)> &dataCallback;
};

#endif // SENSOR_SIM_HH
```

This one is different. It has its own thread, controls its own execution:

```cpp
// src/sensor-sim.cc

#include "sensor-sim.hh"

SensorSim::SensorSim(std::function<void(const int)> &dataCallback)
    : dataCallback(dataCallback), running(false)
{
  std::cout << "SensorSim created" << std::endl;
}

SensorSim::~SensorSim()
{
  stop();
  std::cout << "SensorSim destroyed" << std::endl;
}

void SensorSim::start()
{
  std::cout << "SensorSim starting..." << std::endl;
  this->running = true;
  unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 engine(seed);
  std::uniform_int_distribution<int> dist(50, 250);

  auto sim = [this, &dist, &engine]()
  {
    while (this->running)
    {
      int random_num = dist(engine);
      std::this_thread::sleep_for(std::chrono::milliseconds(random_num));
      std::cout << "SensorSim generated data: " << random_num << std::endl;
      this->dataCallback(random_num);
    }
  };

  std::thread(sim).detach();
}

void SensorSim::stop()
{
  std::cout << "SensorSim stopping..." << std::endl;
  this->running = false;
}
```

So, the challenge is clear now. how to **monitor** values coming from
**outside** the main event loop?

Napi offers `Napi::ThreadSafeFunction` for such situations. Let's check the glue
code to make it happen:

```cpp

```

### When use napi over vanilla v8 or nan?

Short answer is: whenever possible.

In a greenfield project, go strait with NAPI.

Only rely on nan or bare v8 if you're dealing with node versions older than 8 or
if the already existing codebase is already too much tied to v8.

## Further reading

- [Node Addons API](https://nodejs.org/api/addons.html)
- [Modern C++](https://github.com/federico-busato/Modern-CPP-Programming)
- [Mode module system](https://nodejs.org/api/packages.html)
- [V8 guide](https://v8docs.nodesource.com/node-22.4/)
- [C++ threads](https://en.cppreference.com/w/cpp/thread/thread.html)
- [NAPI thread-safe functions](https://github.com/sombriks/node-addon-examples/blob/main/src/5-async-work/call-js-from-async-worker-execute/node-addon-api/src/binding.cc)
-
