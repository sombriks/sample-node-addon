// lib/main.js
import bindings from "bindings";

const addon = bindings("sample_node_addon");

console.log("addon.hello():", addon.hello());
