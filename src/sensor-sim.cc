// src/sensor-sim.cc
#include <thread>
#include <chrono>
#include <random>
#include <iostream>

void genData(void consumer(int data, void *userData), void *userData)
{
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  std::uniform_int_distribution<int> dist(1, 100);

  // Simulate data generation
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  int v = dist(gen);
  std::cout << "Generated data: " << v << std::endl;
  consumer(v, userData); // Generate data in range [0, 99]
}

void sensorWatch(void consumer(int data, void *userData), void *userData)
{
  std::thread dataThread(genData, consumer, userData);
  dataThread.join();
}
