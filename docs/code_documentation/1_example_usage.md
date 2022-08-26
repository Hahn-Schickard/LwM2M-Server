# Example Usage

## Basic Setup

This library is split into two major parts from the users perspective: the Server instance and Event Consumer Interface. The Server instance has everything implemented that is required for a standard LwM2M Server to run and can be executed as is, without any extra modifications. The Event Consumer Interface, on the other hand, requires some extra work from the user, to specify the desired usage of the LwM2M Devices, managed by the server instance. This interface is the glue that binds your custom code with the Server instance. In this chapter we will go over the basics of starting the LwM2M Server and creation of the Custom Event Consumer. Interactions with the Device Model will be discussed in the next chapter Device Model.

## Starting the server

Starting the server is a very simple process, all you need to do is include the `Server.hpp` header, creating an instance of `LwM2M::Server` and calling the `start()` method. This will start all of the internal bindings, which are implemented in an asynchronous manner, however you may desire to use the main program thread for your custom application, in which case it is recommended to start the server in it's own thread and create proper shutdown and exception handling routines.

You do not need to provide any configuration files for a test setup, however, this will mean that no Resource Model will be loaded, thus all Devices that register to this server, will not have any objects/resources to work with. To create a Server with a specific Resource Model, you must provide a **json** configuration file. This configuration file is pretty simple and needs 2 major elements: the **model_descriptors** field, which points to the location of your Resource Model xml file and the **bindings** field, which specifies the supported communication protocols and their configuration files. In this case we are using **CoAP** as a communication protocol. Take note, that all of the paths in the server configuration file, are relative to the `LwM2M::Server` executable location. It is also recommended to setup the logging system that is used by the Server. This SHOULD be done at programs primary start mechanism (typically the `int main()` method in `main.cpp`). To set up the logging system, you must include the `LoggerRepository.hpp` header, that defines the logging repository singleton and initialize the singleton by calling `HaSLL::LoggerRepository::initialise(HaSLL::Configuration)` to use a hardcoded logger configuration or `HaSLL::LoggerRepository::initialise(std::string)` to try to deserialize a configuration file from a **json** file. An example `main.cpp` file and example configuration files are provided bellow:

```cpp
// main.cpp
#include "LoggerRepository.hpp"
#include "Server.hpp"

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

### LwM2M Server Configuration File Example
**serverConfig.json**
```json
{
    "model_descriptors": "model/descriptors.xml",
    "bindings": [
        {
            "type": "CoAP",
            "location": "socketConfig.json"
        }
    ]
}
```

### CoAP Socket Configuration File Example
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

To fully utilize the LwM2M Server library an Event Consumer, that implements the `Event_Model::EventListenerInterface<RegistryEvent>::handleEvent(RegistryEventPtr)` method is required. To implement it, you must first include the `DeviceRegistry.hpp` header, which defines the `LwM2M::EventSourcePtr` class, which will be used as the source of [LwM2M Client Registration Interface](https://www.openmobilealliance.org/release/LightweightM2M/V1_2-20201110-A/OMA-TS-LightweightM2M_Core-V1_2-20201110-A.pdf#__WKANCHOR_2m) Events, as well as `Event_Model/EventListenerInterface.hpp` header, which defines the `EventListenerInterface` implementation. Then declare a custom class inheriting from `Event_Model::EventListenerInterface<RegistryEvent>` structure. THis class must implement a parametrized constructor that accepts `LwM2M::EventSourcePtr` as it's sole parameter and overrides the virtual `void handleEvent(RegistryEventPtr)` method. This method should handle all of the cases, declared in `LwM2MM::RegistryEventType` enum.

This Event Consumer is the primary way, that your code SHOULD interact with the LwM2M Server. An example implementation is provided bellow:

```cpp
// RegistrationListener.hpp

#include "DeviceRegistry.hpp"
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

Once an Event Consumer has been implemented, we need to connect it to the Server Instance, so LwM2M Events can be dispatched to it. This is a very straight forward process and can be done by calling your custom Event Consumers constructor with `LwM2M::EventSourcePtr` parameter. To get the parameter, call the `getEventSource()` method from the server instance and pass it to the constructor of your Event Consumer. An example of this can be found bellow:

```cpp
// main.cpp
#include "LoggerRepository.hpp"
#include "RegistrationListener.hpp" // custom Event Consumer Implementation
#include "Server.hpp"

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