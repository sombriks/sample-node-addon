// test/main.spec.js
import test from "ava"

import { hello, Counter } from "../lib/main.js"

test("Should get hello world", t => {
  t.is(hello(), "hello world!")
})

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