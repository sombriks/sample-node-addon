// test/main.spec.js
import test from "ava"

import { hello, Counter, heavyCalculationSync, heavyCalculationAsync } from "../lib/main.js"

test("Should create and use Counter", t => {
  const counter = new Counter()
  t.is(counter.getCount(), 0)
  counter.increment()
  t.is(counter.getCount(), 1)
  counter.increment()
  t.is(counter.getCount(), 2)
  counter.decrement()
  t.is(counter.getCount(), 1)
})

// test("Should perform a heavy calculation, synchronous", t => {
//   const value = heavyCalculationSync(5)
//   t.is(value, 10)
// })

// test("Should perform a heavy calculation, promise", async t => {
//   const value = await new Promise(resolve => {
//     const value = heavyCalculationSync(10)
//     resolve(value)
//   })
//   t.is(value, 20)
// })

test("Should perform a heavy calculation, promise + timeout", async t => {
  const value = await new Promise(resolve => {
    setTimeout(() => {
      const value = heavyCalculationSync(10)
      resolve(value)
    }, 100)
  })
  t.is(value, 20)
})

test("Should perform a heavy calculation, native promise", async t => {
  const value = await heavyCalculationAsync(15)
  t.is(value, 30)
})

test("Should get hello world", t => {
  t.is(hello(), "hello world!")
})