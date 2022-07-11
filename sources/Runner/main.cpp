
#include "Example.hpp"
#include "LoggerRepository.hpp"
#include "Server.hpp"

#include <chrono>
#include <iostream>
#include <thread>

using namespace HaSLL;
using namespace LwM2M_Example;
using namespace std;

int main(int argc, const char* argv[]) {
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
    } catch (exception& e) {
      logger->log(SeverityLevel::ERROR, "Received an exception: {}", e.what());
      cerr << e.what();
    }

    server->stop();
  } catch (const exception& ex) {
    cerr << ex.what();
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
