#include "LoggerRepository.hpp"
#include "LwM2M_Server.hpp"

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

using namespace HaSLL;
using namespace std;

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