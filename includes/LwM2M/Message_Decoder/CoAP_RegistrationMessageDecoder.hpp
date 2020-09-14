#ifndef __LWM2M_COAP_REGISTRATION_MESSAGE_DECODER_HPP
#define __LWM2M_COAP_REGISTRATION_MESSAGE_DECODER_HPP

#include "CoAP_Message.hpp"
#include "DeviceRegistry.hpp"
#include "Logger.hpp"
#include "LwM2M_Message.hpp"
#include "Stoppable.hpp"
#include "Threadsafe_HashSet.hpp"

#include <memory>

namespace LwM2M {
class CoAP_RegistrationMessageDecoder : public Stoppable {
  std::shared_ptr<DeviceRegistry> registration_;
  std::shared_ptr<ThreadsafeHashSet<CoAP::Message>> message_buffer_;
  std::shared_ptr<HaSLL::Logger> logger_;

  std::unique_ptr<ClientRequest_Register>
  makeRegisterMessage(std::shared_ptr<CoAP::Message> input);
  std::unique_ptr<ClientRequest_Deregister>
  makeDeRegisterMessage(std::shared_ptr<CoAP::Message> input);
  std::unique_ptr<ClientRequest_Update>
  makeUpdateMessage(std::shared_ptr<CoAP::Message> input);
  bool tryDecode(std::shared_ptr<CoAP::Message> message);

public:
  CoAP_RegistrationMessageDecoder(
      std::shared_ptr<DeviceRegistry> registration,
      std::shared_ptr<ThreadsafeHashSet<CoAP::Message>> message_buffer);

  void run() override;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_REGISTRATION_MESSAGE_DECODER_HPP