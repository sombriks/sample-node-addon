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

Like you did with the function, you must wrap your counter using the node addon
API:

```cpp
// src/counter-object.cc

```

Then registry the wrapper in the module:

```cpp
//src/main.cc

```

## Further reading

- [Node Addons API](https://nodejs.org/api/addons.html)
- [Modern C++](https://github.com/federico-busato/Modern-CPP-Programming)
- [Mode module system](https://nodejs.org/api/packages.html)
-
