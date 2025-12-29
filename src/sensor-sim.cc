// src/sensor-sim.cc

#include "sensor-sim.hh"

SensorSim::SensorSim() : running(false)
{
  std::cout << "SensorSim created" << std::endl;
}

SensorSim::~SensorSim()
{
  stop();
  std::cout << "SensorSim destroyed" << std::endl;
}

void SensorSim::start(std::function<void(const int)> dataCallback)
{
  if (this->running)
    return;
    
  std::cout << "SensorSim starting..." << std::endl;
  this->running = true;

  auto sim = [this, dataCallback]()
  {
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 engine(seed);
    std::uniform_int_distribution<int> dist(50, 250);

    std::cout << "SensorSim simulation thread started" << std::endl;
    while (this->running)
    {
      std::cout << "SensorSim generating data..." << std::endl;
      int random_num = dist(engine);
      std::this_thread::sleep_for(std::chrono::milliseconds(random_num));
      std::cout << "SensorSim generated data: " << random_num << std::endl;
      dataCallback(random_num);
    }
  };
  // spawn and forget
  std::thread(sim).detach();
}

void SensorSim::stop()
{
  std::cout << "SensorSim stopping..." << std::endl;
  this->running = false;
}

bool SensorSim::isRunning()
{
  return this->running;
}