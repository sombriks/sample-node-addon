#ifndef SENSOR_SIM_HH
#define SENSOR_SIM_HH

#include <string>
#include <thread>
#include <chrono>
#include <random>
#include <functional>

class SensorSim
{
public:
  SensorSim(std::function<void(const int)> &dataCallback);
  ~SensorSim();
  void start();
  void stop();

private:
  bool running;
  std::function<void(const int)> &dataCallback;
};

#endif // SENSOR_SIM_HH