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

void printResource(ResourcePtr resource, string prefix) {
  cout << prefix << "_"
       << "Resource: " << resource->getDescriptor()->id_ << ": "
       << resource->getDescriptor()->name_ << endl;
  prefix += string(1, ' ') + string("|");
  cout << prefix << "_"
       << "Resource type: " << toString(resource->getDescriptor()->operations_)
       << endl;
  cout << prefix << "_"
       << "Value type: " << toString(resource->getDescriptor()->data_type_)
       << endl;
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
      printObjectInstances(object.second->getObjectInstances(), prefix);
    }
  }
}

void printDevice(DevicePtr device) {
  cout << device->getName() << " with id: " << device->getDeviceId() << endl;
  printObjects(device->getObjects());
  cout << string(getTerminalWidth(), '=') << endl;
}

string stringifyDataVariant(DataVariant variant) {
  string result;
  match(
      variant, [&](bool value) { result = value ? "True" : "False"; },
      [&](int64_t value) { result = to_string(value); },
      [&](uint64_t value) { result = to_string(value); },
      [&](double value) { result = to_string(value); },
      [&](string value) { result = value; },
      [&](TimeStamp value) { result = value.toString(); },
      [&](ObjectLink value) {
        result = "Object link:" + to_string(value.object_id_) + ":" +
            to_string(value.instance_id_);
      },
      [&](vector<uint8_t> value) {
        result = string(value.begin(), value.end());
      });
  return result;
}

string stringifyResourceValue(ResourcePtr resource) {
  string response;
  match(
      resource->getResourceInstance(),
      [&](ReadablePtr& readable) {
        auto resource_future = readable->read();
        auto value = resource_future.get();
        response = stringifyDataVariant(value);
      },
      [&](ReadAndWritablePtr& readable) {
        auto resource_future = readable->read();
        auto value = resource_future.get();
        response = stringifyDataVariant(value);
      },
      [&](...) {
        auto descriptor = resource->getDescriptor();
        auto error_msg = "Resource " + descriptor->name_ + " is not readable";
        throw runtime_error(error_msg);
      });
  auto name = resource->getDescriptor()->name_;
  return string("Resource " + name + " value is: " + response);
}

future<void> asyncRead(DevicePtr device, ElementID id) {
  return async(
      std::launch::async,
      [](DevicePtr device, ElementID element_id) {
        try {
          auto object = device->getObject(element_id);
          auto resource_value = object->getResource(element_id);
          auto value = stringifyResourceValue(resource_value);
          cout << "Device: " << device->getDeviceId() << " "
               << object->getDescriptor()->name_ << " " << value << endl;

        } catch (ResponseReturnedAnErrorCode& ex) {
          cout << "Read request for device " << device->getName() << " "
               << " element " << element_id.toString()
               << " failed! Received an error "
                  "code: "
               << toString(ex.response_code_) << endl;
        } catch (runtime_error& ex) {
          cout << "Encountered a runtime error while processing device "
               << device->getName() << " element " << element_id.toString()
               << ". Exception is: " << ex.what() << endl;
        } catch (exception& ex) {
          cerr << "Caught an unhandled exception while processing device "
               << device->getName() << " element " << element_id.toString()
               << ". Exception is: " << ex.what() << endl;
        }
      },
      device, id);
}

RegistrationListener::RegistrationListener(EventSourcePtr registration)
    : EventListenerInterface(registration) {}

void RegistrationListener::handleEvent(RegistryEventPtr event) {
  switch (event->type_) {
  case RegistryEventType::REGISTERED: {
    auto device = event->device_.value();
    cout << "New device has been registered: ";
    printDevice(device);

    vector<future<void>> threads;
    threads.push_back(
        asyncRead(device, ElementID(3, 0, 4))); // try to read a not readable
    threads.push_back(
        asyncRead(device, ElementID(3, 0, 8))); // try to read optional resource
    threads.push_back(asyncRead(
        device, ElementID(3, 0, 11))); // try to read multiple instance resource
    threads.push_back(asyncRead(device, ElementID(3, 0, 16)));
    threads.push_back(
        asyncRead(device, ElementID(6, 0, 0))); // try to read Latitude as float
    threads.push_back(asyncRead(
        device, ElementID(6, 0, 1))); // try to read Longitude as float
    threads.push_back(
        asyncRead(device, ElementID(6, 0, 5))); // try to read Timestamp
    threads.push_back(asyncRead(
        device, ElementID(3303, 0, 5700))); // try to read temperature value

    for (auto thread = threads.begin(); thread != threads.end(); ++thread) {
      auto status = thread->wait_for(1s);
      if (status == future_status::ready) {
        thread = threads.erase(thread);
      }
    }
    cout << string(getTerminalWidth(), '=') << endl;
    cout << "Finished reading resources" << endl;

    try {
      auto device_obj = device->getObject(ElementID(3, 0));
      auto battery_level_resource = device_obj->getResource(ElementID(3, 0, 9));
      auto battery_level =
          std::get<ReadablePtr>(battery_level_resource->getResourceInstance());
      cout << "Registering a new observer for "
           << battery_level->getID().toString() << endl;
      auto observer = std::make_shared<Observer>(battery_level);
      cout << "Observer for element " << observer->getId()
           << " built. Saving it in the map of observers." << endl;
      observers_.emplace(device->getDeviceId(), observer);
    } catch (ObjectInstanceDoesNotExist& ex) {
      cout << "Could not register battery level observer for device "
           << device->getDeviceId() << ex.what() << endl;
    } catch (exception& ex) {
      cout << "Cought unhandeled exception while trying to register a new "
              "observer for device "
           << device->getDeviceId() << endl
           << "Exception: " << ex.what() << endl;
    }
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
    auto range = observers_.equal_range(event->identifier_);
    for (auto it = range.first; it != range.second; ++it) {
      cout << "Erasing element observer: " << it->second->getId() << endl;
      it->second.reset();
      observers_.erase(it);
    }
    break;
  }
  default: {
    cout << "Received an unhandled event!" << endl;
    break;
  }
  }
}
