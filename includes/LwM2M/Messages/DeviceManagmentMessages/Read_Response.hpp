#ifndef __LWM2M_DEVICE_MANAGMENT_READ_RESPONSE_HPP
#define __LWM2M_DEVICE_MANAGMENT_READ_RESPONSE_HPP

#include "LwM2M_Message.hpp"

namespace LwM2M {
struct Read_Response : public Response {
  std::string location_;
  // content_type_declaration

  Read_Response(std::string endpoint_address, unsigned int endpoint_port,
                uint16_t message_id, std::vector<uint8_t> token,
                std::string location, std::vector<uint8_t> payload);
};
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_READ_RESPONSE_HPP