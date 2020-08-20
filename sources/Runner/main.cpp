#include "LoggerRepository.hpp"
#include "LwM2M_Server.hpp"
#include "Observer_Pattern.hpp"
#include "RegistrationInterfaceEvent.hpp"

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

using namespace HaSLL;
using namespace std;

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