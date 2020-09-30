#ifndef __LWM2M_CONFIG_SERIALIZER_HPP
#define __LWM2M_CONFIG_SERIALIZER_HPP

#include <string>

namespace LwM2M {
struct Configuration {
  std::string object_descriptors_location;
  std::string ip_address;
  unsigned int server_port;
  unsigned int read_timeout;
};

Configuration deserialize(const std::string filepath);
} // namespace LwM2M
#endif //__LWM2M_CONFIG_SERIALIZER_HPP