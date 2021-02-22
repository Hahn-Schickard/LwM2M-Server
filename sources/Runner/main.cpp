#include "CoAP_Binding.hpp"
#include "CoAP_ContentTypes.hpp"
#include "Event_Model.hpp"
#include "LoggerRepository.hpp"
#include "RegistryEvent.hpp"

#include <chrono>
#include <iostream>
#include <locale>
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

string isSupported(uint16_t index) {
  return get<0>(CoAP::SupportedContentFormats::getSupportedContentFormat(
             index)) == true
             ? "is supported."
             : "is NOT supported.";
}

void printSupportedContentFormats() {
  cout << "Content Format: " << CoAP::ContentFormatEncodings::PlainText::index
       << " " << CoAP::ContentFormatEncodings::PlainText::toString() << " "
       << isSupported(CoAP::ContentFormatEncodings::PlainText::index) << endl;
  cout << "Content Format: " << CoAP::ContentFormatEncodings::CoRE_Link::index
       << " " << CoAP::ContentFormatEncodings::CoRE_Link::toString() << " "
       << isSupported(CoAP::ContentFormatEncodings::CoRE_Link::index) << endl;
  cout << "Content Format: "
       << CoAP::ContentFormatEncodings::Octet_Stream::index << " "
       << CoAP::ContentFormatEncodings::Octet_Stream::toString() << " "
       << isSupported(CoAP::ContentFormatEncodings::Octet_Stream::index)
       << endl;
  cout << "Content Format: " << CoAP::ContentFormatEncodings::LwM2M_TLV::index
       << " " << CoAP::ContentFormatEncodings::LwM2M_TLV::toString() << " "
       << isSupported(CoAP::ContentFormatEncodings::LwM2M_TLV::index) << endl;
  cout << "Content Format: " << CoAP::ContentFormatEncodings::LwM2M_CBOR::index
       << " " << CoAP::ContentFormatEncodings::LwM2M_CBOR::toString() << " "
       << isSupported(CoAP::ContentFormatEncodings::LwM2M_CBOR::index) << endl;
  cout << "Content Format: " << CoAP::ContentFormatEncodings::LwM2M_JSON::index
       << " " << CoAP::ContentFormatEncodings::LwM2M_JSON::toString() << " "
       << isSupported(CoAP::ContentFormatEncodings::LwM2M_JSON::index) << endl;
}

int main(int argc, const char *argv[]) {
  try {
    LoggerRepository::initialise("loggerConfig.json");
    auto logger =
        LoggerRepository::getInstance().registerLoger("Example_Runner");
    LoggerRepository::getInstance().configure(SeverityLevel::TRACE);

    auto coap_binding = make_unique<CoAP_Binding>(
        make_shared<DeviceRegistry>("model/descriptors.xml"));
    auto registration_listener =
        make_unique<RegistrationListener>(coap_binding->getEventSource());
    printSupportedContentFormats();
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
