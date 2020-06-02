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

void printCoAPMessage(shared_ptr<CoAP_Message> message) {
  cout << "Received a message from: " << message->getReceiverIP() << ":"
       << message->getReceiverPort() << endl;
  printCoAPHeader(message->getHeader());
  for (const auto &byte : message->getBody()) {
    cout << byte << " ";
  }
  cout << endl;
}

CoAP_Message makeDummyMessage(const string &receiver_address, unsigned int port,
                              int message_id) {
  return CoAP_Message(
      receiver_address, port,
      CoAP_Header(MessageType::CONFIRMABLE, 0, CodeType::GET, message_id),
      vector<uint8_t>{0xFF});
}

int main() {
  auto logger = LoggerRepository::getInstance("./loggerConfig.json")
                    .registerLoger("Example_Runner");
  LoggerRepository::getInstance().configure(SeverityLevel::TRACE);

  thread server_thread;
  logger->log(SeverityLevel::INFO, "Started LwM2M Server!");
  LwM2M_Server::CoAP_Server server(false, 16833, 10);
  thread client_thread;
  LwM2M_Client::DummyClient client(false, 16835);
  try {
    server_thread = thread([&]() { server.run(); });
    client_thread = thread([&]() {
      int message_id = 0;
      do {
        client.sendMessage(makeDummyMessage("0.0.0.0", 16833, message_id));
        message_id++;
      } while (!server.stopRequested());
    });

    do {
      printCoAPMessage(server.pullRequest());
    } while (!server.stopRequested());

  } catch (exception &e) {
    logger->log(SeverityLevel::ERROR, "Received an exception: {}", e.what());
    cerr << e.what();
    server.stop();
  }

  server_thread.join();
  client_thread.join();
  exit(EXIT_SUCCESS);
}