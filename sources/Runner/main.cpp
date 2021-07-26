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

string stringifyResourceValue(ResourceVariant variant) {
  string name;
  string response;
  match(variant,
        [&](ResourcePtr<bool> resource) {
          name = resource->getDescriptor()->name_;
          auto resource_future = resource->read();
          bool value = resource_future.get();
          response = value ? "True" : "False";
        },
        [&](ResourcePtr<int64_t> resource) {
          name = resource->getDescriptor()->name_;
          auto resource_future = resource->read();
          int64_t value = resource_future.get();
          response = to_string(value);
        },
        [&](ResourcePtr<double> resource) {
          name = resource->getDescriptor()->name_;
          auto resource_future = resource->read();
          double value = resource_future.get();
          response = to_string(value);
        },
        [&](ResourcePtr<string> resource) {
          name = resource->getDescriptor()->name_;
          auto resource_future = resource->read();
          response = resource_future.get();
        },
        [&](ResourcePtr<uint64_t> resource) {
          name = resource->getDescriptor()->name_;
          auto resource_future = resource->read();
          uint64_t value = resource_future.get();
          response = to_string(value);
        },
        [&](ResourcePtr<TimeStamp> resource) {
          name = resource->getDescriptor()->name_;
          auto resource_future = resource->read();
          auto value = resource_future.get();
          response = value.toString();
        },
        [&](ResourcePtr<ObjectLink> resource) {
          name = resource->getDescriptor()->name_;
          auto resource_future = resource->read();
          ObjectLink value = resource_future.get();
          response = "Object link:" + to_string(value.object_id_) + ":" +
                     to_string(value.instance_id_);
        },
        [&](ResourcePtr<vector<uint8_t>> resource) {
          name = resource->getDescriptor()->name_;
          auto resource_future = resource->read();
          vector<uint8_t> value = resource_future.get();
          response = string(value.begin(), value.end());
        });
  return string("Resource " + name + " value is: " + response);
}

void asyncRead(DevicePtr device, ObjectID id) {
  thread(
      [](DevicePtr device, ObjectID element_id) {
        try {
          auto object = device->getObject(element_id);
          for (auto instance : element_id.getObjectInstanceIDs()) {
            for (auto resource : instance.getResourceIDs()) {
              auto resource_value =
                  object->getResource(instance.getID(), resource.getID());
              auto value = stringifyResourceValue(move(resource_value));
              cout << "Device: " << device->getDeviceId() << " "
                   << object->getDescriptor()->name_ << " " << value << endl;
            }
          }
        } catch (ResponseReturnedAnErrorCode &ex) {
          cout << "Read request for device " << device->getName() << " "
               << " element " << element_id.toString()
               << " failed! Received an error "
                  "code: "
               << toString(ex.response_code_) << endl;
        } catch (UnsupportedMethod &ex) {
          cout << "Called an unsupported method for device "
               << device->getName() << " element " << element_id.toString()
               << ". Exception is: " << ex.what() << endl;
        } catch (runtime_error &ex) {
          cout << "Encountered a runtime error while processing device "
               << device->getName() << " element " << element_id.toString()
               << ". Exception is: " << ex.what() << endl;
        } catch (exception &ex) {
          cerr << "Caught an unhandled exception while processing device "
               << device->getName() << " element " << element_id.toString()
               << ". Exception is: " << ex.what() << endl;
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
      asyncRead(device, ObjectID(3, 0, 4)); // try to read a not readable
      asyncRead(device, ObjectID(3, 0, 8)); // try to read optional resource
      asyncRead(device,
                ObjectID(3, 0, 11)); // try to read multiple instance resource
      asyncRead(device, ObjectID(3, 0, 16));
      asyncRead(device, ObjectID(6, 0, 0)); // try to read Latitude as float
      asyncRead(device, ObjectID(6, 0, 1)); // try to read Longitude as float
      asyncRead(device, ObjectID(6, 0, 5)); // try to read Timestamp
      asyncRead(device,
                ObjectID(3303, 0, 5700)); // try to read temperature value

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
    auto source = server->getEventSource();
    auto registration_listener = make_unique<RegistrationListener>(source);
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
