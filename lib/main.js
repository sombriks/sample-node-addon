// lib/main.js
import bindings from "bindings";

const addon = bindings("sample_node_addon");

export const hello = addon.hello;
export const Counter = addon.Counter;
export const heavyCalculationSync = addon.heavyCalculationSync;
export const heavyCalculationAsync = addon.heavyCalculationAsync;
export const heavyCalculationCallback = addon.heavyCalculationCallback;
