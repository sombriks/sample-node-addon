// src/heavy-calculation.cc
#include <chrono>
#include <thread>

int heavyCalculation(int n)
{
  int result = n + n;
  // wait for 3 second
  std::this_thread::sleep_for(std::chrono::seconds(3));
  return result;
}