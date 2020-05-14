#include "Converter.hpp"

#include <string>

using namespace std;
namespace LwM2M_Model {

LwM2M_Message &convert(CoAP::CoAP_Message input) {
  InterfaceType interface_type;
  MessageType message_type;
  std::string body;
  LwM2M_Message message(interface_type, message_type, body);
  return message;
}

CoAP::CoAP_Message &convert(LwM2M_Message input);

} // namespace LwM2M_Model
