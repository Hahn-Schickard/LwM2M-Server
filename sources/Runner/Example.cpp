#include "Example.hpp"
#include "CoAP_ContentTypes.hpp"
#include "Observable.hpp"
#include "Readable.hpp"
#include "Variant_Visitor.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#elif defined(__linux__)
#include <sys/ioctl.h>
#endif // Windows/Linux

using namespace std;
using namespace LwM2M_Example;

size_t getTerminalWidth() {
#if defined(_WIN32)
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  return (size_t)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
#elif defined(__linux__)
  struct winsize w;
  ioctl(fileno(stdout), TIOCGWINSZ, &w);
  return (size_t)(w.ws_col);
#endif // Windows/Linux
}

void printResource(ResourceVariant variant, string prefix) {
  match(variant, [&prefix](auto resource) {
    cout << prefix << "_"
         << "Resource: " << resource->getDescriptor()->id_ << ": "
         << resource->getDescriptor()->name_ << endl;
    prefix += string(1, ' ') + string("|");
    cout << prefix << "_"
         << "Resource type: "
         << toString(resource->getDescriptor()->operations_) << endl;
    cout << prefix << "_"
         << "Value type: " << toString(resource->getDescriptor()->data_type_)
         << endl;
  });
};

void printResources(Resources resources, string prefix) {
  prefix += string(1, ' ') + string("|");
  cout << prefix << "_"
       << "Resources: " << resources.size() << endl;
  for (auto resource : resources) {
    printResource(resource.second, prefix);
  }
}

void printObjectInstances(ObjectInstances instances, string prefix) {
  if (!instances.empty()) {
    prefix += string(1, ' ') + string("|");
    cout << prefix << "_"
         << "Object instances: " << instances.size() << endl;
    for (auto instance : instances) {
      cout << prefix << "_"
           << "Instance: "
           << to_string(instance.second->getId().getObjectInstanceID()) << endl;
      printResources(instance.second->getResources(), prefix);
    }
  }
}

void printObjects(ObjectsMap objects) {
  if (!objects.empty()) {
    cout << "With " << objects.size() << " Objects: " << endl;
    auto prefix = string("|");
    for (auto object : objects) {
      cout << prefix << "_"
           << "Object " << object.second->getDescriptor()->id_ << ": "
           << object.second->getDescriptor()->name_ << endl;
      printObjectInstances(object.second->getInstances(), prefix);
    }
  }
}

void printDevice(DevicePtr device) {
  cout << device->getName() << " with id: " << device->getDeviceId() << endl;
  printObjects(device->getObjects());
  cout << string(getTerminalWidth(), '=') << endl;
}

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

void asyncRead(DevicePtr device, ElementID id) {
  thread(
      [](DevicePtr device, ElementID element_id) {
        try {
          auto object = device->getObject(element_id);
          auto resource_value = object->getResource(element_id);
          auto value = stringifyResourceValue(resource_value);
          cout << "Device: " << device->getDeviceId() << " "
               << object->getDescriptor()->name_ << " " << value << endl;

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

struct Observer : public ObserverInterface {
  Observer(ObservablePtr element)
      : ObserverInterface(element), oberser_id_(element->getID().toString()) {}

  void handleEvent(PayloadDataPtr payload) override {
    if (payload) {
      match(*payload,
            [&](DataFormatPtr data) {
              handeData(data);
              cout << endl;
            },
            [&](TargetContent data) {
              handeTargetContent(data);
              cout << endl;
            },
            [&](TargetContentVector data) {
              handleTargetContentVector(data);
              cout << endl;
            },
            [&](ElementID id) {
              handleElementID(id);
              cout << endl;
            },
            [&](ElementIDs ids) {
              handleElementIDs(ids);
              cout << endl;
            },
            [&](TargetAttributes attributes) {
              handleTargetAttributes(attributes);
              cout << endl;
            });
    } else {
      string error = " received a notification with an empty data payload";
    }
  }

private:
  void handeData(DataFormatPtr data) {}

  void handeTargetContent(TargetContent data) {
    cout << data.first.toString() << " ";
    handeData(data.second);
  }

  void handleTargetContentVector(TargetContentVector vector) {
    cout << "{";
    for (auto target_data = vector.begin(); target_data != vector.end();
         ++target_data) {
      handeTargetContent(*target_data);
      if (next(target_data) != vector.end()) {
        cout << ",";
      }
    }
    cout << "}";
  }

  void handleElementID(ElementID id) { cout << id.toString(); }

  void handleElementIDs(ElementIDs ids) {
    cout << "{";
    for (auto id = ids.begin(); id != ids.end(); ++id) {
      handleElementID(*id);
      if (next(id) != ids.end()) {
        cout << ",";
      }
    }
    cout << "}";
  }

  void handleTargetAttribute(TargetAttribute attribute) {
    handleElementID(attribute.first);
    cout << " " << attribute.second->toString();
  }

  void handleTargetAttributes(TargetAttributes attributes) {
    cout << "{";
    for (auto attribute = attributes.begin(); attribute != attributes.end();
         ++attribute) {
      handleTargetAttribute(*attribute);
      if (next(attribute) != attributes.end()) {
        cout << ",";
      }
    }
    cout << "}";
  }

  string oberser_id_;
};

RegistrationListener::RegistrationListener(EventSourcePtr registration)
    : EventListenerInterface(registration) {}

void RegistrationListener::handleEvent(RegistryEventPtr event) {
  switch (event->type_) {
  case RegistryEventType::REGISTERED: {
    auto device = event->device_;
    cout << "New device has been registered: ";
    printDevice(device);
    asyncRead(device, ElementID(3, 0, 4)); // try to read a not readable
    asyncRead(device, ElementID(3, 0, 8)); // try to read optional resource
    asyncRead(device,
              ElementID(3, 0, 11)); // try to read multiple instance resource
    asyncRead(device, ElementID(3, 0, 16));
    asyncRead(device, ElementID(6, 0, 0)); // try to read Latitude as float
    asyncRead(device, ElementID(6, 0, 1)); // try to read Longitude as float
    asyncRead(device, ElementID(6, 0, 5)); // try to read Timestamp
    asyncRead(device,
              ElementID(3303, 0, 5700)); // try to read temperature value

    auto battery_level =
        device->getObject(ElementID(3, 0))->getResource(ElementID(3, 0, 9));
    auto observable = dynamic_pointer_cast<Readable<uint64_t>>(
        get<ResourcePtr<uint64_t>>(battery_level));

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
