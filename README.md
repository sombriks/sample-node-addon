# sample-node-addon

Sample code exploring distinct scenarios of the C++ / javascript bridge in the
node runtime.

## Requirements

- Node 22
- C compiler compatible with modern (C++20) standard, like gcc
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
      "target_name": "my-addon",
      "sources": [ "src/main.cc" ]
    }
  ]
}
```

Once node dependencies are set, you're good to `configure`:

```bash
npx node-gyp configure
```

## How to build

Check the project branches, so you can see how small and big things are made
using native code and node.

## Further reading

- [Node Addons API](https://nodejs.org/api/addons.html)
- [Modern C++](https://github.com/federico-busato/Modern-CPP-Programming)
-
