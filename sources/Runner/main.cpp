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

future<string> stringifyResourceValue(LwM2M::ResourceVariant variant) {
  promise<string> result;
  uint8_t timeout = 30;
  match(variant,
        [&](shared_ptr<LwM2M::Resource<bool>> resource) {
          auto resource_future = resource->read(timeout);
          bool value = resource_future.get();
          result.set_value(value ? "True" : "False");
        },
        [&](shared_ptr<LwM2M::Resource<int64_t>> resource) {
          auto resource_future = resource->read(timeout);
          int64_t value = resource_future.get();
          result.set_value(to_string(value));
        },
        [&](shared_ptr<LwM2M::Resource<double>> resource) {
          auto resource_future = resource->read(timeout);
          double value = resource_future.get();
          result.set_value(to_string(value));
        },
        [&](shared_ptr<LwM2M::Resource<string>> resource) {
          auto resource_future = resource->read(timeout);
          result.set_value(resource_future.get());
        },
        [&](shared_ptr<LwM2M::Resource<uint64_t>> resource) {
          auto resource_future = resource->read(timeout);
          uint64_t value = resource_future.get();
          result.set_value(to_string(value));
        },
        [&](shared_ptr<LwM2M::Resource<LwM2M::ObjectLink>> resource) {
          auto resource_future = resource->read(timeout);
          LwM2M::ObjectLink value = resource_future.get();
          string stringy = "Object link:" + to_string(value.object_id_) + ":" +
                           to_string(value.instance_id_);
          result.set_value(stringy);
        },
        [&](shared_ptr<LwM2M::Resource<vector<uint8_t>>> resource) {
          auto resource_future = resource->read(timeout);
          vector<uint8_t> value = resource_future.get();
          result.set_value(string(value.begin(), value.end()));
        });
  return result.get_future();
}

void asyncRead(shared_ptr<LwM2M::Device> device) {
  thread([&] {
    auto future =
        stringifyResourceValue(device->getObject(3)->getResource(0, 2));
    future.wait();
    cout << "Device: " << device->getDeviceId()
         << " Model Number of this device is: " << future.get() << endl;
  })
      .detach();
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
      auto device = event->device;
      asyncRead(device);
      cout << "Sent a read request to Device: " << device->getDeviceId()
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