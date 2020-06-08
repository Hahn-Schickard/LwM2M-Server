#include "CoAP_Message.hpp"
#include "CoAP_Server.hpp"
#include "DummyClient.hpp"
#include "LoggerRepository.hpp"

#include <iostream>
#include <memory>
#include <thread>
#include <vector>

using namespace HaSLL;
using namespace CoAP;
using namespace std;

void printCoAPHeader(CoAP_Header header) {
  cout << toString(header.getMesageType()) << " Message type with "
       << toString(header.getCodeType()) << endl;
  cout << "Message id is: " << header.getMessageID() << endl;
}

void printCoAPToken(vector<uint8_t> token) {
  cout << "With Token:" << endl;
  for (uint8_t byte : token) {
    cout << to_string(byte) << " ";
  }
  cout << endl;
}

void printCoAPOptions(vector<shared_ptr<CoAP_Option>> options) {
  cout << "With " << options.size() << " Options" << endl;
  for (auto option : options) {
    cout << (option->isCritical() ? "Critical" : "Non-critical") << "; ";
    cout << (option->isRepeatable() ? "Repeatable" : "Non-repeatabe") << "; ";
    cout << (option->isUnsafe() ? "Unsafe" : "Safe") << "; ";
    cout << option->size() << " Byte long option: ";
    cout << toString(option->getOptionNumber()) << "; ";
    cout << "With value: " << option->getValue() << endl;
  }
}

void printCoAPPayload(vector<uint8_t> payload) {
  cout << "With Data:" << endl;
  for (uint8_t byte : payload) {
    cout << to_string(byte) << " ";
  }
  cout << endl;
}

void printCoAPMessage(shared_ptr<CoAP_Message> message) {
  cout << "Received a message from: " << message->getReceiverIP() << ":"
       << message->getReceiverPort() << endl;
  printCoAPHeader(message->getHeader());

  if (!message->getToken().empty()) {
    printCoAPToken(message->getToken());
  }

  if (!message->getOptions().empty()) {
    printCoAPOptions(message->getOptions());
  }

  if (!message->getBody().empty()) {
    printCoAPPayload(message->getBody());
  }
}

CoAP_Message makeResponse(shared_ptr<CoAP_Message> message) {
  CoAP_Header header(MessageType::ACKNOWLEDGMENT,
                     message->getHeader().getTokenLenght(), CodeType::OK,
                     message->getHeader().getMessageID() + 1);
  return CoAP_Message(message->getReceiverIP(), message->getReceiverPort(),
                      header, message->getToken(),
                      vector<shared_ptr<CoAP_Option>>(), vector<uint8_t>());
}

int main() {
  auto logger = LoggerRepository::getInstance("./loggerConfig.json")
                    .registerLoger("Example_Runner");
  LoggerRepository::getInstance().configure(SeverityLevel::TRACE);

  thread server_thread;
  logger->log(SeverityLevel::INFO, "Started LwM2M Server!");
  LwM2M_Server::CoAP_Server server(false, 5683, 10);
  try {
    server_thread = thread([&]() { server.run(); });

    do {
      auto message = server.pullRequest();
      printCoAPMessage(message);
      server.pushResponse(makeResponse(move(message)));
    } while (!server.stopRequested());

  } catch (exception &e) {
    logger->log(SeverityLevel::ERROR, "Received an exception: {}", e.what());
    cerr << e.what();
    server.stop();
  }

  server_thread.join();
  exit(EXIT_SUCCESS);
}