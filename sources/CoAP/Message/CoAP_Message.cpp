#include "CoAP_Message.hpp"
#include "Option_Builder.hpp"

#include <deque>

#define PAYLOAD_MARKER_SIZE 1

using namespace std;
namespace CoAP {

CoAP_Message::CoAP_Message()
    : CoAP_Message(string(), 0, CoAP_Header(), vector<uint8_t>()) {}

CoAP_Message::CoAP_Message(string receiver_ip, unsigned int receiver_port,
                           CoAP_Header header_data, vector<uint8_t> body_data)
    : receiver_ip_(receiver_ip), receiver_port_(receiver_port),
      header_(move(header_data)) {
  deque<uint8_t> payload(body_data.begin(), body_data.end());

  if (header_.getTokenLenght() != 0) {
    uint8_t index = header_.getTokenLenght();
    while (index != 0) {
      token_.push_back(payload.front());
      payload.pop_front();
      index--;
    }
  }

  auto previous = shared_ptr<CoAP_Option>();
  shared_ptr<CoAP_Option> current;
  do {
    try {
      if (!payload.empty()) {
        current = build(previous, payload);
        options_.push_back(current);
        previous = current;
        payload.erase(payload.begin(), payload.begin() + current->size());
      } else {
        break; // received a message without a payload
      }
    } catch (PayloadMarkerDetected &exp) {
      payload.erase(payload.begin(), payload.begin() + PAYLOAD_MARKER_SIZE);
      break;
    }
  } while (true);

  if (!payload.empty()) {
    body_ = vector<uint8_t>(payload.begin(), payload.end());
  } else {
    body_ = vector<uint8_t>(0);
  }
}

vector<uint8_t> CoAP_Message::toPacket() {
  auto result = header_.toPacket();
  result.insert(result.end(), body_.begin(), body_.end());
  return result;
}

string CoAP_Message::getReceiverIP() { return receiver_ip_; }

unsigned int CoAP_Message::getReceiverPort() { return receiver_port_; }

CoAP_Header &CoAP_Message::getHeader() { return header_; }

vector<uint8_t> CoAP_Message::getToken() { return token_; }

vector<shared_ptr<CoAP_Option>> CoAP_Message::getOptions() { return options_; }

vector<uint8_t> CoAP_Message::getBody() { return body_; }
} // namespace CoAP