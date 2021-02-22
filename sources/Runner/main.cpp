#include "CoAP_Binding.hpp"
#include "Event_Model.hpp"
#include "LoggerRepository.hpp"
#include "RegistryEvent.hpp"

#include <chrono>
#include <iostream>
#include <thread>

using namespace HaSLL;
using namespace LwM2M;
using namespace std;

class RegistrationListener : public Event_Model::EventListener<RegistryEvent> {
public:
  RegistrationListener(EventSourcePtr registration)
      : EventListener(registration) {}

  void handleEvent(RegistryEventPtr event) override {
    switch (event->type_) {
    case RegistryEventType::REGISTERED: {
      cout << "A new device with id: " << event->identifier_
           << " has been registered!" << endl;
      auto device = event->device_;
      cout << "Sent a write request to Device object 1 instance 0, resource 1"
           << endl;
      cout << "Sent a read request to Device: " << device->getDeviceId()
           << endl;
      break;
    }
    case RegistryEventType::UPDATED: {
      cout << "Device with id: " << event->identifier_ << " has been updated!"
           << endl;
      break;
    }
    case RegistryEventType::DEREGISTERED: {
      cout << "Device with id: " << event->identifier_
           << " has been deregistered!" << endl;
      break;
    }
    default: {
      cout << "Received an unhandled event!" << endl;
      break;
    }
    }
  }
};

int main(int argc, const char *argv[]) {
  try {
    LoggerRepository::initialise("loggerConfig.json");
    auto logger =
        LoggerRepository::getInstance().registerLoger("Example_Runner");
    LoggerRepository::getInstance().configure(SeverityLevel::TRACE);

    auto coap_binding = make_unique<CoAP_Binding>(
        make_shared<CoAP_BindingConfig>(),
        make_shared<DeviceRegistry>("model/descriptors.xml"));
    auto registration_listener =
        make_unique<RegistrationListener>(coap_binding->getEventSource());
    try {
      coap_binding->start();
      logger->log(SeverityLevel::INFO, "Started LwM2M Server!");
      if (argc > 1) {
        int sleep_period = atoi(argv[1]);
        this_thread::sleep_for(chrono::seconds(sleep_period));
      } else {
        for (;;) {
        }
      }
    } catch (exception &e) {
      logger->log(SeverityLevel::ERROR, "Received an exception: {}", e.what());
      cerr << e.what();
    }

    coap_binding->stop();
    exit(EXIT_SUCCESS);
  } catch (const exception &ex) {
    cerr << ex.what();
    exit(EXIT_FAILURE);
  }
}
