#ifndef __LWM2M_DEVICE_ENDPoINT_HPP
#define __LWM2M_DEVICE_ENDPoINT_HPP

#include <memory>
#include <string>

namespace LwM2M {
struct Endpoint {
  std::string endpoint_address_;
  unsigned int endpoint_port_;
  std::string sms_number_;
};

using EndpointPtr = std::shared_ptr<Endpoint>;

} // namespace LwM2M

#endif //__LWM2M_DEVICE_ENDPoINT_HPP