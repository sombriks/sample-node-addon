// src/sensor-sim.cc

#include "sensor-sim.hh"

SensorSim::SensorSim(std::function<void(const int)> &dataCallback)
{
  this->dataCallback = dataCallback;
  this->running = false;
}

SensorSim::~SensorSim()
{
  stop();
}

void SensorSim::start()
{
  this->running = true;
  unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 engine(seed);
  std::uniform_int_distribution<int> dist(50, 250);

  auto sim = [this, &dist, &engine]()
  {
    while (this->running)
    {
      int random_num = dist(engine);

      std::this_thread::sleep_for(std::chrono::milliseconds(random_num));
      this->dataCallback(random_num);
    }
  };

  std::thread(sim).detach();
}

void SensorSim::stop()
{
  this->running = false;
}
