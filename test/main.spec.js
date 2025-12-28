// test/main.spec.js
import test from "ava"

import { hello, Counter, heavyCalculation, SensorSimMonitor } from "../lib/main.js"

test("Should create and use SensorSimMonitor", async t => {
  const monitor = new SensorSimMonitor()
  t.false(monitor.isMonitoring())
  let readings = []
  monitor.startMonitoring((reading) => {
    readings.push(reading)
    if (readings.length >= 3) {
      monitor.stopMonitoring()
    }
  })
  // wait until we have at least 3 readings
  await new Promise(resolve => {
    const checkReadings = () => {
      if (readings.length >= 3) {
        resolve()
      } else {
        setTimeout(checkReadings, 4000)
      }
    }
  })
  t.true(monitor.isMonitoring() === false)
})

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

test("Should perform a heavy calculation, native callback + promise", async t => {
  const value = await Promise.all([
    new Promise(resolve => heavyCalculation(15, resolve)),
    new Promise(resolve => heavyCalculation(21, resolve))
  ])
  t.like(value, [30, 42])
})