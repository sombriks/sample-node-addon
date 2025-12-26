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
touch src/sensor-sim.cc 
touch src/sensor-sim-callback.cc
touch src/sensor-sim-callback.hh
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
                "src/main.cc",
                "src/sensor-sim-callback.cc",
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

## Threads and Callbacks

Asynchronous operations are one of the biggest powers of node. But when native
code is asynchronous already, running in its own thread, you need extra steps.

Take this sensor simulator as example:

```cpp
// src/sensor-sim.cc
#include <thread>
#include <chrono>
#include <random>

void genData(void consumer(int data))
{
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  std::uniform_int_distribution<int> dist(1, 100);

  int i = 12;
  while (i-- > 0)
  {
    // Simulate data generation
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    consumer(dist(gen)); // Generate data in range [0, 99]
  }
}

void sensorWatch(void consumer(int data))
{
  std::thread dataThread(genData, consumer);
  dataThread.join();
}
```

When called, `sensorWatch` will report some random values through the `consumer`
callback and then die.

But all this is happening on another thread, so the wrapper will take the
following shape:

```cpp
```

## Further reading

- [Node Addons API](https://nodejs.org/api/addons.html)
- [Modern C++](https://github.com/federico-busato/Modern-CPP-Programming)
- [Mode module system](https://nodejs.org/api/packages.html)
- [V8 guide](https://v8docs.nodesource.com/node-22.4/)
- [C++ threads](https://en.cppreference.com/w/cpp/thread/thread.html)
-
