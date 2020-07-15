#ifndef __LWM2M_MESSAGE_SORTER_HPP
#define __LWM2M_MESSAGE_SORTER_HPP

#include "DeviceManagmentInterfaceMessages.hpp"
#include "InformationReportingInterfaceMessages.hpp"
#include "Logger.hpp"
#include "RegistrationInterfaceMessages.hpp"
#include "Stoppable.hpp"
#include "Threadsafe_Queue.hpp"

#include <memory>
#include <thread>

namespace LwM2M {
class MessageSorter : public Stoppable {
  std::shared_ptr<ThreadsafeQueue<Message>> incoming_message_queue_;
  std::shared_ptr<ThreadsafeQueue<Regirstration_Interface_Message>>
      registration_interface_queue_;
  std::shared_ptr<ThreadsafeQueue<DeviceManagment_Interface_Message>>
      device_managment_interface_queue_;
  std::shared_ptr<ThreadsafeQueue<InformationReporting_Interface_Message>>
      information_reporting_interface_queue_;
  std::shared_ptr<HaSLL::Logger> logger_;

public:
  MessageSorter();
  MessageSorter(
      std::shared_ptr<ThreadsafeQueue<Message>> incoming_message_queue);

  void run() override;
  std::shared_ptr<ThreadsafeQueue<Regirstration_Interface_Message>>
  getRegistrationInterfaceQueue();
  std::shared_ptr<ThreadsafeQueue<DeviceManagment_Interface_Message>>
  getDeviceManagmentInterfaceQueue();
  std::shared_ptr<ThreadsafeQueue<InformationReporting_Interface_Message>>
  getInformationReportingQueue();
};
} // namespace LwM2M

#endif //__LWM2M_MESSAGE_SORTER_HPP