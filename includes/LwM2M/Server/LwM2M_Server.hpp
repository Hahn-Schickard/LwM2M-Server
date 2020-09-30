#ifndef __LWM2M_SERVER_HPP
#define __LWM2M_SERVER_HPP

#include "DeviceRegistry.hpp"
#include "Logger.hpp"
#include "LwM2M_Config.hpp"
#include "LwM2M_Device.hpp"
#include "StoppableTask.hpp"

#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace LwM2M {
using RegistryEventSourcePtr =
    std::shared_ptr<Event_Model::EventSource<RegistryEvent>>;

class Server {
  std::vector<StoppableTask> tasks_;
  std::shared_ptr<DeviceRegistry> registration_;
  std::shared_ptr<HaSLL::Logger> logger_;

public:
  Server();
  Server(const std::string filepath);
  Server(Configuration config);

  void stop();
  void start();
  DevicePtr getDevice(std::string device_id);
  RegistryEventSourcePtr getEventSource();
};
} // namespace LwM2M
#endif //__LWM2M_SERVER_HPP