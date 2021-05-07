#include "CoAP_ContentTypes.hpp"
#include "Event_Model/EventListenerInterface.hpp"
#include "LoggerRepository.hpp"
#include "RegistryEvent.hpp"
#include "Server.hpp"
#include "Variant_Visitor.hpp"

#include <chrono>
#include <iostream>
#include <thread>

using namespace HaSLL;
using namespace LwM2M;
using namespace std;

future<string> stringifyResourceValue(ResourceVariant variant) {
  promise<string> result;
  match(variant,
        [&](ResourcePtr<bool> resource) {
          auto resource_future = resource->read();
          bool value = resource_future.get();
          result.set_value(value ? "True" : "False");
        },
        [&](ResourcePtr<int64_t> resource) {
          auto resource_future = resource->read();
          int64_t value = resource_future.get();
          result.set_value(to_string(value));
        },
        [&](ResourcePtr<double> resource) {
          auto resource_future = resource->read();
          double value = resource_future.get();
          result.set_value(to_string(value));
        },
        [&](ResourcePtr<string> resource) {
          auto resource_future = resource->read();
          result.set_value(resource_future.get());
        },
        [&](ResourcePtr<uint64_t> resource) {
          auto resource_future = resource->read();
          uint64_t value = resource_future.get();
          result.set_value(to_string(value));
        },
        [&](ResourcePtr<ObjectLink> resource) {
          auto resource_future = resource->read();
          ObjectLink value = resource_future.get();
          string stringy = "Object link:" + to_string(value.object_id_) + ":" +
                           to_string(value.instance_id_);
          result.set_value(stringy);
        },
        [&](ResourcePtr<vector<uint8_t>> resource) {
          auto resource_future = resource->read();
          vector<uint8_t> value = resource_future.get();
          result.set_value(string(value.begin(), value.end()));
        });
  return result.get_future();
}

void asyncRead(DevicePtr device, ResourceID id) {
  thread(
      [](DevicePtr device, ResourceID element_id) {
        try {
          auto object =
              device->getObject(element_id.object_instance_.object_.id_);
          auto resource = object->getResource(element_id.object_instance_.id_,
                                              element_id.id_);
          auto future = stringifyResourceValue(move(resource));
          future.wait();
          cout << "Device: " << device->getDeviceId() << " "
               << object->getDescriptor()->name_
               << " value is: " << future.get() << endl;
        } catch (ResponseReturnedAnErrorCode &ex) {
          string id;
          for (auto element : element_id.toStrings()) {
            id += element + " ";
          }
          cout << "Read request for Element " << id
               << " failed! Received an error "
                  "code: "
               << toString(ex.response_code_) << endl;
        } catch (UnsupportedMethod &ex) {
          cout << "Called an unsupported method: " << ex.what() << endl;
        } catch (runtime_error &ex) {
          cout << "Encountered a runtime error: " << ex.what() << endl;
        } catch (exception &ex) {
          cerr << "Caught an unhandled exception: " << ex.what() << endl;
        }
      },
      device, id)
      .detach();
}

class RegistrationListener
    : public Event_Model::EventListenerInterface<RegistryEvent> {
public:
  RegistrationListener(EventSourcePtr registration)
      : EventListenerInterface(registration) {}

  void handleEvent(RegistryEventPtr event) override {
    switch (event->type_) {
    case RegistryEventType::REGISTERED: {
      cout << "A new device with id: " << event->identifier_
           << " has been registered!" << endl;
      auto device = event->device_;
      asyncRead(device, ResourceID(3, 0, 4)); // try to read a not readable
      asyncRead(device, ResourceID(3, 0, 8)); // try to read a non existant
      asyncRead(device,
                ResourceID(3303, 0, 5700)); // try to read temperature value
      asyncRead(device, ResourceID(3, 0, 11));
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
    LoggerRepository::initialise("config/loggerConfig.json");
    auto logger =
        LoggerRepository::getInstance().registerLoger("Example_Runner");
    LoggerRepository::getInstance().configure(SeverityLevel::TRACE);

    auto server = make_unique<Server>("config/serverConfig.json");
    auto registration_listener =
        make_unique<RegistrationListener>(server->getEventSource());
    try {
      server->start();
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

    server->stop();
    exit(EXIT_SUCCESS);
  } catch (const exception &ex) {
    cerr << ex.what();
    exit(EXIT_FAILURE);
  }
}
