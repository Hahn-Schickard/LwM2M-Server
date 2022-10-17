#include "HaSLL/LoggerManager.hpp"
#include "HaSLL/SPD_LoggerRepository.hpp"
#include "LwM2M_Server/Server.hpp"

#include <iostream>

using namespace HaSLL;
using namespace std;

int main() {
  {
    auto repo = make_shared<SPD_LoggerRepository>();
    LoggerManager::initialise(repo);
    LoggerManager::configure(SeverityLevel::TRACE);
    auto logger = LoggerManager::registerLogger("Integration_Runner");

    unique_ptr<LwM2M::Server> server;
    try {
      server = make_unique<LwM2M::Server>("config/serverConfig.json");
      cout << "LwM2M Server uses the following bindings:" << endl;
      for (const auto& name : server->getRegisterBindingNames()) {
        cout << name << endl;
      }
      server->start();
      cout << "Started LwM2M Server!" << endl;
      this_thread::sleep_for(1s);
    } catch (exception& e) {
      logger->log(SeverityLevel::ERROR, "Received an exception: {}", e.what());
      cerr << e.what();
    }

    if (server) {
      server->stop();
      cout << "Stopped LwM2M Server!" << endl;
    }
  }

  exit(EXIT_SUCCESS);
}