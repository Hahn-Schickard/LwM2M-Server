#include "LoggerRepository.hpp"
#include "LwM2M_Server.hpp"

#include <iomanip>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

using namespace HaSLL;
using namespace std;

int main() {
  auto logger = LoggerRepository::getInstance("loggerConfig.json")
                    .registerLoger("Example_Runner");
  LoggerRepository::getInstance().configure(SeverityLevel::TRACE);

  LwM2M::Server server;
  try {
    server = LwM2M::Server(LwM2M::Configuration{string("model/descriptors.xml"),
                                                string("0.0.0.0"), 5683, 10});
    server.start();
    logger->log(SeverityLevel::INFO, "Started LwM2M Server!");
    this_thread::sleep(1s);

  } catch (exception &e) {
    logger->log(SeverityLevel::ERROR, "Received an exception: {}", e.what());
    cerr << e.what();
  }

  server.stop();
  logger->log(SeverityLevel::INFO, "Stopped LwM2M Server!");

  exit(EXIT_SUCCESS);
}