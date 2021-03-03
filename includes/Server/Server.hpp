#ifndef __LWM2M_SERVER_HPP
#define __LWM2M_SERVER_HPP

#include "Binding.hpp"
#include "DeviceRegistry.hpp"
#include "StoppableTask.hpp"

#include <vector>

namespace LwM2M {

class Server {
  DeviceRegistryPtr registry_;
  std::vector<StoppableTask> bindings_;

public:
  Server(const std::string filepath = std::string());

  void start();
  void stop();

  EventSourcePtr getEventSource();
};
} // namespace LwM2M

#endif //__LWM2M_SERVER_HPP