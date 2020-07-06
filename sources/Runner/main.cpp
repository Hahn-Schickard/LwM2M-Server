#include "LoggerRepository.hpp"
#include "LwM2M_Server.hpp"

#include <iomanip>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

using namespace HaSLL;
using namespace LwM2M_Model;
using namespace std;

int main() {
  auto logger = LoggerRepository::getInstance("./loggerConfig.json")
                    .registerLoger("Example_Runner");
  LoggerRepository::getInstance().configure(SeverityLevel::TRACE);

  LwM2M_Server server;
  try {
    server = LwM2M_Server(LwM2M_Configuration{string("model/descriptors.xml"),
                                              string("0.0.0.0"), 5683, 10});
    server.start();
    logger->log(SeverityLevel::INFO, "Started LwM2M Server!");
    for (;;) {
    }

  } catch (exception &e) {
    logger->log(SeverityLevel::ERROR, "Received an exception: {}", e.what());
    cerr << e.what();
    server.stop();
  }

  exit(EXIT_SUCCESS);
}