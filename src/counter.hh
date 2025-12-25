// src/counter.hh
#ifndef COUNTER_HH
#define COUNTER_HH

#include <iostream>

class Counter
{
private:
  int count;

public:
  Counter();
  Counter(int);
  ~Counter();
  void increment();
  void decrement();
  int getCount();
};
#endif // COUNTER_HH
