#ifndef __LWM2M_COAP_MESSAGE_DECODER_HPP
#define __LWM2M_COAP_MESSAGE_DECODER_HPP

#include "CoAP_Message.hpp"
#include "Logger.hpp"
#include "LwM2M_Message.hpp"
#include "RegistrationInterface.hpp"
#include "Response_Handler.hpp"
#include "Stoppable.hpp"
#include "Threadsafe_Unique_Queue.hpp"

#include <memory>

namespace LwM2M {
class CoAP_Decoder : public Stoppable {
  std::shared_ptr<RegistrationInterface> registration_;
  std::shared_ptr<ThreadsafeUniqueQueue<CoAP::Message>> message_buffer_;
  std::shared_ptr<ResponseHandler> response_handler_;
  std::shared_ptr<HaSLL::Logger> logger_;

  bool processIfResponse(const CoAP::Message *message);
  bool processIfBootrstrapInterface(const CoAP::Message *message);
  bool processIfDeviceRegistrationInteraface(const CoAP::Message *message);
  bool processIfDeviceManagmentInterface(const CoAP::Message *message);
  bool processIfInformationReportingInterface(const CoAP::Message *message);
  void decode(std::unique_ptr<CoAP::Message> message);

public:
  CoAP_Decoder(
      std::shared_ptr<RegistrationInterface> registration,
      std::shared_ptr<ThreadsafeUniqueQueue<CoAP::Message>> message_buffer,
      std::shared_ptr<ResponseHandler> response_handler);

  void run() override;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_MESSAGE_DECODER_HPP