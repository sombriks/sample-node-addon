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

if (global.gc) {
  counter = null;
  console.log("Triggering garbage collection...");
  global.gc();
  console.log("Counter object deleted.");
}

console.log("addon.hello():", addon.hello());
