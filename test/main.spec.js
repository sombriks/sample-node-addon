// test/main.spec.js
import test from "ava"

import {
  hello,
  Counter,
  heavyCalculationSync,
  heavyCalculationAsync,
  heavyCalculationCallback
} from "../lib/main.js"

test("Should perform a heavy calculation, native callback + promise", async t => {
  t.log("Starting heavy calculation with callback...")
  const value = await new Promise(resolve => heavyCalculationCallback(21, resolve))
  t.is(value, 42)
  t.log("Heavy calculation with callback completed.")
})

test("Should get hello world", t => {
  t.log("Testing hello function...")
  t.is(hello(), "hello world!")
  t.log("hello function works correctly.")
})

test("Should create and use Counter", t => {
  t.log("Testing Counter class...")
  const counter = new Counter()
  t.is(counter.getCount(), 0)
  counter.increment()
  t.is(counter.getCount(), 1)
  counter.increment()
  t.is(counter.getCount(), 2)
  counter.decrement()
  t.is(counter.getCount(), 1)
  t.log("Counter class works correctly.")
})

test("Should perform a heavy calculation, synchronous", t => {
  t.log("Starting synchronous heavy calculation...")
  const value = heavyCalculationSync(5)
  t.is(value, 10)
  t.log("Synchronous heavy calculation completed.")
})

// test("Should perform a heavy calculation, promise + timeout", async t => {
//   const value = await new Promise(resolve => {
//     setTimeout(() => {
//       const value = heavyCalculationSync(10)
//       resolve(value)
//     }, 100)
//   })
//   t.is(value, 20)
// })

// test("Should perform a heavy calculation, native promise", async t => {
//   const value = await heavyCalculationAsync(15)
//   t.is(value, 30)
// })