// lib/main.js
import bindings from "bindings";

const addon = bindings("sample_node_addon");

export const Counter = addon.Counter;

export const hello = addon.hello;
