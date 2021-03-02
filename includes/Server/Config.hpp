#ifndef __LWM2M_SERVER_CONFIGURATION_HPP
#define __LWM2M_SERVER_CONFIGURATION_HPP

#include <string>
#include <unordered_map>
#include <utility>

namespace LwM2M {

enum class ServerBindingType : uint8_t { CoAP = 0, UNKOWN };

std::string toString(ServerBindingType type);

using Bindings = std::unordered_multimap<ServerBindingType, std::string>;

struct Configuration {
  std::string model_descriptors_;
  Bindings bindings_;
};

Configuration getConfig(const std::string filepath);
} // namespace LwM2M

#endif //__LWM2M_SERVER_CONFIGURATION_HPP