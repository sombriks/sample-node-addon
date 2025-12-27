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

## Threads and Callbacks

Asynchronous operations are one of the biggest powers of node.
Let start simple, let's say we want the result of a heavy calculation:

```cpp
// src/heavy-calculation.cc
#include <chrono>
#include <thread>

int heavyCalculation(int n)
{
  int result = n + n;
  // wait for 3 second
  std::this_thread::sleep_for(std::chrono::seconds(3));
  return result;
}
```

The synchronous approach would look like this:

```cpp
// src/heavy-calculation-sync.cc
#include <node.h>
#include "heavy-calculation.hh"

void HeavyCalculationSync(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();

  if (args.Length() < 1 || !args[0]->IsNumber())
  {
    isolate
      ->ThrowException(v8::String::NewFromUtf8(isolate, "Argument must be a number")
      .ToLocalChecked());
    return;
  }

  int n = args[0]->Int32Value(isolate->GetCurrentContext()).FromJust();

  int result = heavyCalculation(n);

  args.GetReturnValue().Set(v8::Integer::New(isolate, result));
}
```

And could be tested like this:

```javascript
// test/main.spec.js
import test from "ava"

import { hello, Counter, heavyCalculationSync } from "../lib/main.js"

// other test cases...

test("Should perform a heavy calculation, synchronous", t => {
  const value = heavyCalculationSync(5)
  t.is(value, 10)
})
```

Everything looks fine, until you notice something odd when running the test
suite:

```
  ✔ Should get hello world (3s)
  ✔ Should create and use Counter (3s)
  ✔ Should perform a heavy calculation, synchronous (3s)
```

Instead of just the heavy operation, **all operations got delayed**. This is
because everything is running on
[node's main event loop](https://nodejs.org/en/learn/asynchronous-work/event-loop-timers-and-nexttick)..

One first workaround for this is to schedule the execution using timers:

```javascript
// test/main.spec.js
import test from "ava"

import { hello, Counter, heavyCalculationSync } from "../lib/main.js"

// other test cases

test("Should perform a heavy calculation, promise + timeout", async t => {
  const value = await new Promise(resolve => {
    setTimeout(() => {
      const value = heavyCalculationSync(10)
      resolve(value)
    }, 100)
  })
  t.is(value, 20)
})
```

While this approach seems to free the main loop from the big performance hit,
the timers are still paying the price.

Let's return a promise natively and see what happens:

```cpp
// src/heavy-calculation-async.cc
#include <node.h>
#include "heavy-calculation.hh"

void HeavyCalculationAsync(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();

  if (args.Length() < 1 || !args[0]->IsNumber())
  {
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Argument must be a number").ToLocalChecked());
    return;
  }
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  // get the param
  int n = args[0]->Int32Value(context).FromJust();
  // prepare a promise
  v8::Local<v8::Promise::Resolver> resolver = //
    v8::Promise::Resolver::New(context).ToLocalChecked();
  args.GetReturnValue().Set(resolver->GetPromise());
  // do the heavy calculation
  int result = heavyCalculation(n);
  // resolve the promise
  resolver
      ->Resolve(context, v8::Integer::New(isolate, result))
      .ToChecked();
}
```

The test case gets simple again, but check out the times:

```
  ✔ Should get hello world (6s)
  ✔ Should create and use Counter (6s)
  ✔ Should perform a heavy calculation, synchronous (6s)
  ✔ Should perform a heavy calculation, native promise (3s)
  ✔ Should perform a heavy calculation, promise + timeout (6s)
```

## Further reading

- [Node Addons API](https://nodejs.org/api/addons.html)
- [Modern C++](https://github.com/federico-busato/Modern-CPP-Programming)
- [Mode module system](https://nodejs.org/api/packages.html)
- [V8 guide](https://v8docs.nodesource.com/node-22.4/)
- [C++ threads](https://en.cppreference.com/w/cpp/thread/thread.html)
-
