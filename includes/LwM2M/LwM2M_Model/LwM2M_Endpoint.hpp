#ifndef __LWM2M_DEVICE_ENDPoINT_HPP
#define __LWM2M_DEVICE_ENDPoINT_HPP

#include <string>

namespace LwM2M {
struct Endpoint {
  std::string endpoint_address_;
  unsigned int endpoint_port_;
  std::string sms_number_;
};
} // namespace LwM2M

#endif //__LWM2M_DEVICE_ENDPoINT_HPP