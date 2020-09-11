#ifndef __LWM2M_COAP_DEVICE_MANAGMENT_MESSAGE_DECODER_HPP
#define __LWM2M_COAP_DEVICE_MANAGMENT_MESSAGE_DECODER_HPP

#include "CoAP_Message.hpp"
#include "Logger.hpp"
#include "LwM2M_Message.hpp"
#include "Response_Handler.hpp"
#include "Stoppable.hpp"
#include "Threadsafe_HashSet.hpp"

#include <memory>

namespace LwM2M {
class CoAP_DeviceManagmentMessageDecoder : public Stoppable {
  std::shared_ptr<ResponseHandler> response_handler_;
  std::shared_ptr<ThreadsafeHashSet<CoAP::Message>> message_buffer_;
  std::shared_ptr<HaSLL::Logger> logger_;

  std::unique_ptr<Response>
  makeResponse(std::shared_ptr<CoAP::Message> message);
  bool tryDecode(std::shared_ptr<CoAP::Message> message);

public:
  CoAP_DeviceManagmentMessageDecoder(
      std::shared_ptr<ResponseHandler> response_handler,
      std::shared_ptr<ThreadsafeHashSet<CoAP::Message>> message_buffer);

  void run() override;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_DEVICE_MANAGMENT_MESSAGE_DECODER_HPP