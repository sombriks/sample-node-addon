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
      "target_name": "sample-node-addon",
      "sources": [ "src/main.cc" ]
    }
  ]
}
```

Once node dependencies are set, you're good to `configure`:

```bash
npx node-gyp configure
```

## Basic Boilerplate

In order to expose native calls to the node runtime, yo must define a module:

```cpp
#include <node.h>

void Initialize(v8::Local<v8::Object> exports)
{
  // This is where you would add your addon initialization code.
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
```

Also, whenever deep changes occur in the code or node-gyp configuration,
run the following commands:

```bash
nxp node-gyp clean;
nxp node-gyp configure;
nxp node-gyp build;
```

You can make them scripts inside `package.json` for better ergonomics.

## Further reading

- [Node Addons API](https://nodejs.org/api/addons.html)
- [Modern C++](https://github.com/federico-busato/Modern-CPP-Programming)
-
