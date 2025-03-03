
#include "Example.hpp"
#include "Server.hpp"

#include "HaSLL/LoggerManager.hpp"

#include <chrono>
#include <iostream>
#include <thread>

using namespace HaSLL;
using namespace LwM2M_Example;
using namespace std;

int main(int argc, const char* argv[]) {
  try {
    LoggerManager::initialise(
        makeDefaultRepository("config/loggerConfig.json"));
    auto logger = LoggerManager::registerLogger("Example_Runner");
    LoggerManager::configure(SeverityLevel::Trace);

    auto server = make_unique<Server>("config/serverConfig.json");
    auto source = server->getEventSource();
    auto registration_listener = make_unique<RegistrationListener>(source);
    try {
      server->start();
      logger->info("Started LwM2M Server!");
      if (argc > 1) {
        int sleep_period = atoi(argv[1]); // NOLINT
        this_thread::sleep_for(chrono::seconds(sleep_period));
      } else {
        for (;;) {
        }
      }
    } catch (const exception& e) {
      logger->error("Received an exception: {}", e.what());
      cerr << e.what() << endl;
    }

    server->stop();
  } catch (const exception& ex) {
    cerr << ex.what() << endl;
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
