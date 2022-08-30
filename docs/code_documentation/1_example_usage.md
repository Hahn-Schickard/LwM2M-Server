# Example Usage

## Basic Setup

This library is split into two major parts from the users perspective: the Server instance and Event Consumer Interface. The Server instance has everything implemented that is required for a standard %LwM2M Server to run and can be executed as is, without any extra modifications. The Event Consumer Interface, on the other hand, requires some extra work from the user, to specify the desired usage of the %LwM2M Devices, managed by the server instance. This interface is the glue that binds your custom code with the Server instance. In this chapter we will go over the basics of starting the %LwM2M Server and creation of the Custom Event Consumer. 

## Starting the server

Starting the server is a very simple process, all you need to do is include the `LwM2M_Server/Server.hpp` header, creating an instance of `LwM2M::Server` and calling the `start()` method. This will start all of the internal bindings, which are implemented in an asynchronous manner, however you may desire to use the main program thread for your custom application, in which case it is recommended to start the server in it's own thread and create proper shutdown and exception handling routines.

You do not need to provide any configuration files for a test setup, however, this will mean that no Resource Model will be loaded, thus all Devices that register to this server, will not have any objects/resources to work with. To create a Server with a specific Resource Model, you must provide a **json** configuration file. This configuration file is pretty simple and needs 2 major elements: the **model_descriptors** field, which points to the location of your Resource Model xml file and the **bindings** field, which specifies the supported communication protocols and their configuration files. In this case we are using **%CoAP**  as a communication protocol. Take note, that all of the paths in the server configuration file, are relative to the %LwM2M Server executable location. It is also recommended to setup the logging system that is used by the Server. This SHOULD be done at programs primary start mechanism (typically the `int main()` method in `main.cpp`). To set up the logging system, you must include the `LoggerRepository.hpp` header, that defines the logging repository singleton and initialize the singleton by calling `HaSLL::LoggerRepository::initialise(HaSLL::Configuration)` to use a hardcoded logger configuration or `HaSLL::LoggerRepository::initialise(std::string)` to try to deserialize a configuration file from a **json** file. An example `main.cpp` file and example configuration files are provided bellow:

```cpp
// main.cpp
#include "LoggerRepository.hpp"
#include "LwM2M_Server/Server.hpp"

#include <iostream>
#include <stdexcept>

int main(int argc, const char* argv[]){
    try {
        HaSLL::LoggerRepository::initialise("loggerConfig.json");
        // It's recommended to use the smart pointers for simpler
        // memory management process, but you can also use a local
        // instance, instance created with new operator, or a malloced
        // instance
        auto server = make_unique<Server>("serverConfig.json");
        server->start();
        std::cout << "LwM2M Server Started!" << std::endl;
        for (;;) {} // endless sleep
    } catch (const std::exception& ex) {
        std::cerr << ex.what();
        return 1;
    }
    return 0;
}
```

### %LwM2M Server Configuration File Example
**serverConfig.json**
```json
{
    "model_descriptors": "model/descriptors.xml",
    "bindings": [
        {
            "type": " CoAP  ",
            "location": "socketConfig.json"
        }
    ]
}
```

### %CoAP Socket Configuration File Example
**socketConfig.json**
```json
{
    "server_address": "0.0.0.0",
    "ipv6": false,
    "send_timeout_ms": 2000,
    "server_port": 5683
}
```

### Logger Configuration File Example
**loggerConfig.json**
```json
{
    "log_to_standard_output": false,
    "logfile_name": "lwm2m_server.log",
    "logfile_path": "logs",
    "logging_level": "debug",
    "item_queue_size": 8192,
    "thread_count": 2,
    "maximum_file_count": 25,
    "maximum_file_size_MB": 100,
    "flush_period": 1,
    "message_pattern": "[%Y-%m-%d-%H:%M:%S:%F %z][%n]%^[%l]: %v%$"
}
```

## Creating an Event Consumer

To fully utilize the %LwM2M Server library an Event Consumer, that implements the `Event_Model::EventListenerInterface<RegistryEvent>::handleEvent(RegistryEventPtr)` method is required. To implement it, you must first include the `LwM2M_Server/DeviceRegistry.hpp` header, which defines the `LwM2M::EventSourcePtr` class, which will be used as the source of [LwM2M Client Registration Interface](https://www.openmobilealliance.org/release/LightweightM2M/V1_2-20201110-A/OMA-TS-LightweightM2M_Core-V1_2-20201110-A.pdf#__WKANCHOR_2m) Events, as well as `Event_Model/EventListenerInterface.hpp` header, which defines the `EventListenerInterface` implementation. Then declare a custom class inheriting from `Event_Model::EventListenerInterface<RegistryEvent>` structure. THis class must implement a parametrized constructor that accepts `LwM2M::EventSourcePtr` as it's sole parameter and overrides the virtual `void handleEvent(RegistryEventPtr)` method. This method should handle all of the cases, declared in `LwM2M::RegistryEventType` enum.

This Event Consumer is the primary way, that your code SHOULD interact with the %LwM2M Server. An example implementation is provided bellow:

```cpp
// RegistrationListener.hpp

#include "LwM2M_Server/DeviceRegistry.hpp"
#include "Event_Model/EventListenerInterface.hpp"

#include <iostream> // Used for std::cout and std::endl stream operators

struct RegistrationListener
    : public Event_Model::EventListenerInterface<RegistryEvent> {

  RegistrationListener(LwM2M::EventSourcePtr registration)
    : EventListenerInterface(registration) {}

  void handleEvent(LwM2M::RegistryEventPtr event) override {
    switch (event->type_) {
    case LwM2M::RegistryEventType::REGISTERED: {
        // Obtain the device abstraction
        auto device = event->device_.value();
        // Handle registration process (reading initial device
        // values, building out custom representation, etc...)
        std::cout << "New device " << device->getName()
            << " with id: " << device->getDeviceId()
            << " was registered!" << std::endl;
        break;
    }
    case LwM2M::RegistryEventType::UPDATED: {
        // Handle device update (check if Assigned objects/resources,
        // device name, device id, address, lifetime or communication
        // protocol have changed, @see UpdateRequest for more info)
        std::cout << "Device with id: " << event->identifier_
        << " has been updated!" << std::endl;
        break;
    }
    case LwM2M::RegistryEventType::DEREGISTERED: {
        // Handle device deregistration (deletion/removal)
        std::cout << "Device with id: " << event->identifier_
            << " has been updated!" << std::endl;
        break;
    }
    default: {
        // Handle unexpected event types (These might come from updates
        // to LwM2M server implementation, it is recommended to print
        // out or log the unhandled event type by calling the
        // LwM2M::toString(LwM2M::RegistryEventType) function)
        std::cout << "Received an unhandled event: "
            << LwM2M::toString(event->type_) << std::endl;
        break;
    }
    }
  }
};
```
## Assigning an Event Consumer to a Server instance

Once an Event Consumer has been implemented, we need to connect it to the Server Instance, so %LwM2M Events can be dispatched to it. This is a very straight forward process and can be done by calling your custom Event Consumers constructor with `LwM2M::EventSourcePtr` parameter. To get the parameter, call the `getEventSource()` method from the server instance and pass it to the constructor of your Event Consumer. An example of this can be found bellow:

```cpp
// main.cpp
#include "LoggerRepository.hpp"
#include "RegistrationListener.hpp" // custom Event Consumer Implementation
#include "LwM2M_Server/Server.hpp"

#include <iostream>
#include <stdexcept>

int main(int argc, const char* argv[]){
    try {
        HaSLL::LoggerRepository::initialise("loggerConfig.json");
        auto server = make_unique<Server>("serverConfig.json");
        // It is recommended to assign the event consumer before starting
        // the server. This way, you will not miss any registration events.
        auto registration_listener = make_unique<RegistrationListener>(
            server->getEventSource());
        server->start();
        std::cout << "LwM2M Server Started!" << std::endl;
        for (;;) {} // endless sleep
    } catch (const std::exception& ex) {
        std::cerr << ex.what();
        return 1;
    }
    return 0;
}
```

## Device Usage

Every **%LwM2M Client** is represented as a LwM2M::Device instance. Each Device has a unique identifier, which is used to identify it and it's Resource model.
Every **%LwM2M Client** is built up of Object/ObjectInstance/Resource/ResourceInstance elements, which model the client's functionality. Object and ObjectInstance elements organize various functionalities in a logical structure, where Resource elements describe a specific functionality, that the client provides, and finally ResourceInstance elements provide the interface to said functionality. When working with LwM2M::Device instances, it is suggested to first obtain all Object elements and handle every Resource within every ObjectInstance element. This approach allows you to identify the Resource model hierarchy and properly represent it. When you are dealing with the ResourceInstance element, it is recommended to use the provided `Variant_Visitor.hpp` header. It provides a helper function called `template <typename Variant, typename... Matchers> auto match(Variant &&variant, Matchers &&... matchers)`, which simplifies accessing variant value and makes it work akin to switch-case statement. Currently, ResourceInstance has 4 variant types: Readable, Writable, ReadAndWritable and Executable. Each type can use DataVariant as a parameter or as a result, thus when dealing with ResourceInstance you will also have to deal with DataVariant type. Currently, DataVariant has 8 data types: `bool`, `int64_t`, `uint64_t`, `double`, `std::string`, `ObjectLink` and `std::vector<uint8_t>`, which is used to represent opaque byte arrays. The example bellow, showcases a generic mechanism for reading and resource value as a string by utilizing the Variant_Visitor header. 

```cpp
#include "Variant_Visitor.hpp"
#include "LwM2M_Server/Readable.hpp"
#include "LwM2M_Server/ReadAndWritable.hpp"

#include <string>

using namespace std;
using namespace LwM2M;

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
      [&](ReadablePtr readable) {
        auto resource_future = readable->read();
        auto value = resource_future.get();
        response = stringifyDataVariant(value);
      },
      [&](ReadAndWritablePtr readable) {
        auto resource_future = readable->read();
        auto value = resource_future.get();
        response = stringifyDataVariant(value);
      },
      [&](auto /*value*/) {
        // You can use fall-through like mechanism to handle every other case
        auto descriptor = resource->getDescriptor();
        auto error_msg = "Resource " + descriptor->name_ + " is not readable";
        throw runtime_error(error_msg);
      });
  auto name = resource->getDescriptor()->name_;
  return string("Resource " + name + " value is: " + response);
}
```