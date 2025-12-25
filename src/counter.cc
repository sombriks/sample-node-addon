// src/counter.cc

#include "counter.hh"

Counter::Counter() : count(0)
{
  std::cout << "Counter initialized to zero." << std::endl;
}

Counter::Counter(int initial) : count(initial)
{
  std::cout << "Counter initialized to " << initial << "." << std::endl;
}

Counter::~Counter()
{
  std::cout << "Counter destroyed." << std::endl;
}

void Counter::increment()
{
  ++count;
  std::cout << "Counter incremented to " << count << "." << std::endl;
}

void Counter::decrement()
{
  --count;
  std::cout << "Counter decremented to " << count << "." << std::endl;
}

int Counter::getCount()
{
  std::cout << "Current counter value: " << count << "." << std::endl;
  return count;
}
