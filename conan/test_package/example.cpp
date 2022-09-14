#include "HaSLL/LoggerManager.hpp"
#include "LwM2M_Server/Server.hpp"

#include <iomanip>
#include <iostream>
#include <thread>

using namespace HaSLL;
using namespace std;

int main() {
  {
    auto config = HaSLL::Configuration(
        "./log", "logfile.log", "[%Y-%m-%d-%H:%M:%S:%F %z][%n]%^[%l]: %v%$",
        HaSLL::SeverityLevel::TRACE, false, 8192, 2, 25, 100, 1);
    auto repo = make_shared<SPD_LoggerRepository>(config);
    LoggerManager::initialise(repo)

    auto logger =
        LoggerManager::registerLogger("Integration_Runner");
    LoggerManager::configure(SeverityLevel::TRACE);

    unique_ptr<LwM2M::Server> server;
    try {
      server = make_unique<LwM2M::Server>("config/serverConfig.json");
      server->start();
      logger->log(SeverityLevel::INFO, "Started LwM2M Server!");
      this_thread::sleep_for(1s);

    } catch (exception &e) {
      logger->log(SeverityLevel::ERROR, "Received an exception: {}", e.what());
      cerr << e.what();
    }

    if (server) {
      server->stop();
      logger->log(SeverityLevel::INFO, "Stopped LwM2M Server!");
    }
  }

  exit(EXIT_SUCCESS);
}