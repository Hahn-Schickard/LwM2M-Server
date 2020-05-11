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

int main() {

  auto logger = LoggerRepository::getInstance().registerLoger("Example_Runner");
  LoggerRepository::getInstance().configure(SeverityLevel::TRACE);

  thread server_thread;
  LwM2M_Server::CoAP_Server server(false, 16833, 1);
  thread client_thread;
  LwM2M_Client::DummyClient client(false, "127.0.0.1", 16833);
  try {
    server_thread = thread([&]() { server.run(); });
    this_thread::sleep_for(chrono::seconds(1));
    client_thread = thread([&]() {
      int message_id = 0;
      do {
        client.sendMessage(CoAP_Message(
            "127.0.0.1", 16833,
            CoAP_Header(MessageType::CONFIRMABLE, 0, CodeType::GET, message_id),
            vector<char>()));
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