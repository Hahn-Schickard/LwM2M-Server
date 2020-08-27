#include "LoggerRepository.hpp"
#include "LwM2M_Server.hpp"
#include "Observer_Pattern.hpp"
#include "RegistrationInterfaceEvent.hpp"
#include "Variant_Visitor.hpp"

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

using namespace HaSLL;
using namespace std;

string stringifyResourceValue(LwM2M::ResourceVariant variant) {
  string result;
  match(variant,
        [&](shared_ptr<LwM2M::Resource<bool>> resource) {
          bool value = resource->read();
          result = value ? "True" : "False";
        },
        [&](shared_ptr<LwM2M::Resource<int64_t>> resource) {
          int64_t value = resource->read();
          result = to_string(value);
        },
        [&](shared_ptr<LwM2M::Resource<double>> resource) {
          double value = resource->read();
          result = to_string(value);
        },
        [&](shared_ptr<LwM2M::Resource<string>> resource) {
          result = resource->read();
        },
        [&](shared_ptr<LwM2M::Resource<uint64_t>> resource) {
          uint64_t value = resource->read();
          result = to_string(value);
        },
        [&](shared_ptr<LwM2M::Resource<LwM2M::ObjectLink>> resource) {
          LwM2M::ObjectLink value = resource->read();
          result = "Object link:" + to_string(value.object_id_) + ":" +
                   to_string(value.instance_id_);
        },
        [&](shared_ptr<LwM2M::Resource<vector<uint8_t>>> resource) {
          vector<uint8_t> value = resource->read();
          result = string(value.begin(), value.end());
        });
  return result;
}

class RegistrationListener
    : public ObserverPattern::EventListener<LwM2M::RegistrationInterfaceEvent> {
public:
  RegistrationListener(LwM2M::RegistrationEventSourcePtr registration)
      : EventListener(registration) {}

  void
  handleEvent(shared_ptr<LwM2M::RegistrationInterfaceEvent> event) override {
    switch (event->type) {
    case LwM2M::RegistrationInterfaceEventType::REGISTERED: {
      cout << "A new device with id: " << event->identifier
           << " has been registered!" << endl;
      cout << "Model Number of this device is :";
      auto device = event->device;
      cout << stringifyResourceValue(device->getObject(3)->getResource(0, 2))
           << endl;
      break;
    }
    case LwM2M::RegistrationInterfaceEventType::UPDATED: {
      cout << "Device with id: " << event->identifier << " has been updated!"
           << endl;
      break;
    }
    case LwM2M::RegistrationInterfaceEventType::DEREGISTERED: {
      cout << "Device with id: " << event->identifier
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

    LwM2M::Server server;
    try {
      server = LwM2M::Server(LwM2M::Configuration{
          string("model/descriptors.xml"), string("0.0.0.0"), 5683, 10});
      auto registration_listener =
          make_unique<RegistrationListener>(server.getEventSource());
      server.start();
      logger->log(SeverityLevel::INFO, "Started LwM2M Server!");
      if (argc > 1) {
        int sleep_period = atoi(argv[1]);
        this_thread::sleep_for(chrono::seconds(sleep_period));
        server.stop();
      } else {
        for (;;) {
        }
      }
    } catch (exception &e) {
      logger->log(SeverityLevel::ERROR, "Received an exception: {}", e.what());
      cerr << e.what();
      server.stop();
    }

    exit(EXIT_SUCCESS);
  } catch (const exception &ex) {
    cerr << ex.what();
    exit(EXIT_FAILURE);
  }
}