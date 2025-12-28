#ifndef SENSOR_SIM_HH
#define SENSOR_SIM_HH

#include <string>
#include <thread>
#include <chrono>
#include <random>
#include <iostream>
#include <functional>

class SensorSim
{
public:
  SensorSim();
  ~SensorSim();
  void start(std::function<void(const int)>);
  void stop();
  bool isRunning();

private:
  bool running;
};

#endif // SENSOR_SIM_HH