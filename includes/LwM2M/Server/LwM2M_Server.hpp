#ifndef __LWM2M_SERVER_HPP
#define __LWM2M_SERVER_HPP

#include "Logger.hpp"
#include "LwM2M_Device.hpp"
#include "RegistryHandler.hpp"
#include "StoppableTask.hpp"

#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace LwM2M {
using RegistryEventSourcePtr =
    std::shared_ptr<ObserverPattern::EventSource<RegistryEvent>>;

struct Configuration {
  std::string object_descriptors_location;
  std::string ip_address;
  unsigned int server_port;
  unsigned int read_timeout;
};

class Server {
  std::vector<StoppableTask> tasks_;
  std::shared_ptr<RegistryHandler> registration_;
  std::shared_ptr<HaSLL::Logger> logger_;

public:
  Server();
  Server(Configuration config);

  void stop();
  void start();
  DevicePtr getDevice(std::string device_id);
  RegistryEventSourcePtr getEventSource();
};
} // namespace LwM2M
#endif //__LWM2M_SERVER_HPP