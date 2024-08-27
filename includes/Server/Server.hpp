#ifndef __LWM2M_SERVER_HPP
#define __LWM2M_SERVER_HPP

#include "Binding.hpp"
#include "DeviceRegistry.hpp"

#include "HaSLL/Logger.hpp"

#include <vector>

namespace LwM2M {

class Server {
  DeviceRegistryPtr registry_;
  std::vector<BindingInterfacePtr> bindings_;
  HaSLL::LoggerPtr logger_;

public:
  Server(const std::string& filepath);

  void start();
  std::vector<std::string> getRegisterBindingNames();
  void stop();

  EventSourcePtr getEventSource();
};
} // namespace LwM2M

#endif //__LWM2M_SERVER_HPP