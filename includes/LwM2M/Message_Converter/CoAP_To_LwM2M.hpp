#ifndef __COAP_MESAGE_TO_LWM2M_MESSAGE_CONVERTER_HPP
#define __COAP_MESAGE_TO_LWM2M_MESSAGE_CONVERTER_HPP

#include "CoAP_Message.hpp"
#include "CoAP_Option.hpp"
#include "Converter.hpp"
#include "LwM2M_Message.hpp"
#include "RegistrationInterface.hpp"
#include "Threadsafe_Queue.hpp"

namespace LwM2M {
class CoAP_To_LwM2M : public Converter<CoAP::Message> {
  std::shared_ptr<ThreadsafeQueue<Message>> output_queue_;
  std::shared_ptr<RegistrationInterface> registration_;

  bool processIfBootrstrapInterface(std::shared_ptr<CoAP::Option> option,
                                    const CoAP::Message *message);
  bool
  processIfDeviceRegistrationInteraface(std::shared_ptr<CoAP::Option> option,
                                        const CoAP::Message *message);
  bool processIfDeviceManagmentInterface(std::shared_ptr<CoAP::Option> option,
                                         const CoAP::Message *message);
  bool
  processIfInformationReportingInterface(std::shared_ptr<CoAP::Option> option,
                                         const CoAP::Message *message);

public:
  CoAP_To_LwM2M(std::shared_ptr<ThreadsafeQueue<Message>> output_queue,
                std::shared_ptr<RegistrationInterface> registration);

  void convert(std::unique_ptr<CoAP::Message> message) override;
};
} // namespace LwM2M

#endif //__COAP_MESAGE_TO_LWM2M_MESSAGE_CONVERTER_HPP