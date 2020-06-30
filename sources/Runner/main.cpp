#include "CoAP_Message.hpp"
#include "CoAP_Server.hpp"
#include "LoggerRepository.hpp"

#include <iomanip>
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
  cout << "Option Number   | Size | Critical | Repeatable | Unsafe | Value "
       << endl;
  cout << "================|======|==========|============|========|======="
       << endl;
  for (auto option : options) {
    string name = toString(option->getOptionNumber());
    cout << name << setw(18 - name.size()) << setfill(' ') << "| ";
    string option_size = to_string(option->size());
    cout << option_size << setw(7 - option_size.size()) << setfill(' ') << "| ";
    string is_critical = (option->isCritical() ? " Yes" : " No");
    cout << is_critical << setw(11 - is_critical.size()) << setfill(' ')
         << "| ";
    string is_repeatable = (option->isRepeatable() ? " Yes" : " No");
    cout << is_repeatable << setw(13 - is_repeatable.size()) << setfill(' ')
         << "| ";
    string is_unsafe = (option->isUnsafe() ? " Yes" : " No");
    cout << is_unsafe << setw(9 - is_unsafe.size()) << setfill(' ') << "| ";
    cout << option->getValue() << endl;
  }
  cout << "================|======|==========|============|========|======="
       << endl;
}

void printCoAPPayload(shared_ptr<PayloadFormat> payload) {
  cout << "With Data:" << endl;
  cout << payload->toString() << endl;
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

  if (message->getBody()) {
    printCoAPPayload(message->getBody());
  }
  cout << string(80, '~') << endl;
}

CoAP_Message makeResponse(shared_ptr<CoAP_Message> message) {
  CoAP_Header header(MessageType::ACKNOWLEDGMENT,
                     message->getHeader().getTokenLenght(), CodeType::OK,
                     message->getHeader().getMessageID() + 1);
  return CoAP_Message(message->getReceiverIP(), message->getReceiverPort(),
                      header, message->getToken(),
                      vector<shared_ptr<CoAP_Option>>(),
                      shared_ptr<PayloadFormat>());
}

int main() {
  auto logger = LoggerRepository::getInstance("./loggerConfig.json")
                    .registerLoger("Example_Runner");
  LoggerRepository::getInstance().configure(SeverityLevel::TRACE);

  thread server_thread;
  logger->log(SeverityLevel::INFO, "Started LwM2M Server!");
  CoAP_Server server(string("0.0.0.0"), 5683, 10);
  try {
    server_thread = thread([&]() { server.run(); });

    do {
      auto message = server.pullRequest();
      printCoAPMessage(message);
      // server.pushResponse(makeResponse(move(message)));
    } while (!server.stopRequested());

  } catch (exception &e) {
    logger->log(SeverityLevel::ERROR, "Received an exception: {}", e.what());
    cerr << e.what();
    server.stop();
  }

  server_thread.join();
  exit(EXIT_SUCCESS);
}