#include "MessageSorter.hpp"
#include "LoggerRepository.hpp"
#include "UniquePtrCast.hpp"

using namespace std;
using namespace HaSLL;

namespace LwM2M_Model {

MessageSorter::MessageSorter()
    : MessageSorter(shared_ptr<ThreadsafeQueue<LwM2M_Message>>()) {}

MessageSorter::MessageSorter(
    shared_ptr<ThreadsafeQueue<LwM2M_Message>> incoming_message_queue)
    : incoming_message_queue_(incoming_message_queue),
      registration_interface_queue_(
          make_shared<ThreadsafeQueue<Regirstration_Interface_Message>>()),
      device_managment_interface_queue_(
          make_shared<ThreadsafeQueue<DeviceManagment_Interface_Message>>()),
      information_reporting_interface_queue_(
          make_shared<
              ThreadsafeQueue<InformationReporting_Interface_Message>>()),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {}

void MessageSorter::run() {
  while (!stopRequested()) {
    try {
      auto msg = incoming_message_queue_->wait_and_pop();
      switch (msg->interface_type_) {
      case InterfaceType::REGISTRATION: {
        registration_interface_queue_->push(
            utility::static_pointer_cast<Regirstration_Interface_Message>(
                move(msg)));
        break;
      }
      case InterfaceType::DEVICE_MANAGMENT: {

        device_managment_interface_queue_->push(
            utility::static_pointer_cast<DeviceManagment_Interface_Message>(
                move(msg)));
        break;
      }
      case InterfaceType::INFORMATION_REPORTING: {
        information_reporting_interface_queue_->push(
            utility::static_pointer_cast<
                InformationReporting_Interface_Message>(move(msg)));
        break;
      }
      case InterfaceType::BOOTSTRAP:
      default: {
        logger_->log(
            HaSLL::SeverityLevel::INFO, "Dropping unhandled {} message {} ",
            toString(msg->interface_type_), toString(msg->message_type_));
      }
      }
    } catch (exception &ex) {
      logger_->log(HaSLL::SeverityLevel::ERROR, "Caught an exception: {}",
                   ex.what());
    }
  }
}

shared_ptr<ThreadsafeQueue<Regirstration_Interface_Message>>
MessageSorter::getRegistrationInterfaceQueue() {
  return registration_interface_queue_;
}

shared_ptr<ThreadsafeQueue<DeviceManagment_Interface_Message>>
MessageSorter::getDeviceManagmentInterfaceQueue() {
  return device_managment_interface_queue_;
}

shared_ptr<ThreadsafeQueue<InformationReporting_Interface_Message>>
MessageSorter::getInformationReportingQueue() {
  return information_reporting_interface_queue_;
}

} // namespace LwM2M_Model