#ifndef __LWM2M_DEVICE_ENDPoINT_HPP
#define __LWM2M_DEVICE_ENDPoINT_HPP

#include <memory>
#include <string>

namespace LwM2M {
struct Endpoint {
  const std::string endpoint_address_;
  const unsigned int endpoint_port_;
  const std::string sms_number_;

  Endpoint(std::string endpoint_address = std::string(),
           unsigned int endpoint_port = 0,
           std::string sms_number = std::string())
      : endpoint_address_(endpoint_address), endpoint_port_(endpoint_port),
        sms_number_(sms_number) {}
};

using EndpointPtr = std::shared_ptr<Endpoint>;

} // namespace LwM2M

#endif //__LWM2M_DEVICE_ENDPoINT_HPP