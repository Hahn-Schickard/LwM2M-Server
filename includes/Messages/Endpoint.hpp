#ifndef __LWM2M_DEVICE_ENDPoINT_HPP
#define __LWM2M_DEVICE_ENDPoINT_HPP

#include <memory>
#include <string>

namespace LwM2M {
struct Endpoint {
  const std::string endpoint_address_ = std::string();
  const unsigned int endpoint_port_ = 0;
  const std::string sms_number_ = std::string();

  Endpoint() = delete;
  Endpoint(std::string sms_number);
  Endpoint(std::string endpoint_address, unsigned int endpoint_port);

  std::string toString();
};

using EndpointPtr = std::shared_ptr<Endpoint>;

} // namespace LwM2M

#endif //__LWM2M_DEVICE_ENDPoINT_HPP