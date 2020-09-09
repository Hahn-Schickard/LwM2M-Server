#ifndef __LWM2M_SERVER_HPP
#define __LWM2M_SERVER_HPP

#include "Logger.hpp"
#include "LwM2M_Device.hpp"
#include "RegistryHandler.hpp"
#include "Stoppable.hpp"

#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace LwM2M {
using RegistryEventSourcePtr =
    std::shared_ptr<ObserverPattern::EventSource<RegistryEvent>>;

class StoppableTask {
  std::unique_ptr<Stoppable> task_;
  std::unique_ptr<std::thread> task_thread_;
  std::string task_name_;

public:
  StoppableTask();
  StoppableTask(std::unique_ptr<Stoppable> task, std::string task_name);

  void startTask();
  void stopTask();
  std::string getName();
};

struct Configuration {
  std::string object_descriptors_location;
  std::string ip_address;
  unsigned int server_port;
  unsigned int read_timeout;
};

class Server {
  std::vector<StoppableTask> stoppabletaskes_;
  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Device>>>
      device_registery_;
  std::shared_ptr<RegistryHandler> registration_;
  std::shared_ptr<HaSLL::Logger> logger_;

public:
  Server();
  Server(Configuration config);

  void stop();
  void start();
  std::shared_ptr<Device> getDevice(std::string device_id);
  RegistryEventSourcePtr getEventSource();
};
} // namespace LwM2M
#endif //__LWM2M_SERVER_HPP