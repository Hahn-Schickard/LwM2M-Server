#ifndef __LWM2M_MESSAGE_ENCODER_HPP
#define __LWM2M_MESSAGE_ENCODER_HPP

#include "DeviceManagmentInterfaceMessages.hpp"
#include "InformationReportingInterfaceMessages.hpp"
#include "LwM2M_Message.hpp"
#include "RegistrationInterfaceMessages.hpp"

#include <memory>

namespace LwM2M {

class MessageEncoder {
public:
  virtual ~MessageEncoder() = default;

  virtual void encode(std::unique_ptr<Read_Request> input) = 0;

  virtual void encode(std::unique_ptr<Register_Response> input) = 0;
  virtual void encode(std::unique_ptr<Response> input) = 0;
};
} // namespace LwM2M

#endif //__LWM2M_MESSAGE_ENCODER_HPP