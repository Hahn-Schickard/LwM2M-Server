#include "CoAP_Message.hpp"
#include "Option_Builder.hpp"
#include "PayloadDecoder.hpp"

#include <deque>

#define HEADER_SIZE 4
#define PAYLOAD_MARKER 0xFF
#define PAYLOAD_MARKER_SIZE 1

using namespace std;
namespace CoAP {

vector<uint8_t> removeSubvector(vector<uint8_t> &parent, size_t end_index,
                                size_t start_index = 0) {
  vector<uint8_t> result(parent.begin() + start_index,
                         parent.begin() + end_index);
  parent.erase(parent.begin() + start_index, parent.begin() + end_index);
  return result;
}

void CoAP_Message::processToken(vector<uint8_t> &udp_datagram) {
  uint8_t token_lenght = header_.getTokenLenght();
  if (token_lenght > 0) {
    token_ = removeSubvector(udp_datagram, token_lenght);
  }
}

void CoAP_Message::processOptions(vector<uint8_t> &udp_datagram) {
  deque<uint8_t> payload(udp_datagram.begin(), udp_datagram.end());

  auto previous = shared_ptr<CoAP_Option>();
  shared_ptr<CoAP_Option> current;
  do {
    try {
      current = build(previous, payload);
      options_.push_back(current);
      previous = current;
    } catch (PayloadMarkerDetected &exp) {
      payload.erase(payload.begin(), payload.begin() + PAYLOAD_MARKER_SIZE);
      break;
    }
  } while (!payload.empty());
}

void CoAP_Message::processPayload(vector<uint8_t> &udp_datagram) {
  shared_ptr<ContentFormat> content_format;
  for (auto &option : options_) {
    if (option->getOptionNumber() == OptionNumber::CONTENT_FORMAT) {
      content_format = static_pointer_cast<ContentFormat>(option);
    }
  }
  if (!udp_datagram.empty() && content_format) {
    body_ = decode(content_format, udp_datagram);
  }
}

CoAP_Message::CoAP_Message() : CoAP_Message(string(), 0, vector<uint8_t>()) {}

CoAP_Message::CoAP_Message(string receiver_ip, unsigned int receiver_port,
                           vector<uint8_t> udp_datagram)
    : receiver_ip_(receiver_ip), receiver_port_(receiver_port) {
  vector<uint8_t> bytestream = move(udp_datagram);
  header_ = CoAP_Header(removeSubvector(bytestream, HEADER_SIZE));
  processToken(bytestream);
  processOptions(bytestream);
  processPayload(bytestream);
}

CoAP_Message::CoAP_Message(string receiver_ip, unsigned int receiver_port,
                           CoAP_Header header_data, vector<uint8_t> token,
                           vector<shared_ptr<CoAP_Option>> options,
                           vector<string> body)
    : receiver_ip_(receiver_ip), receiver_port_(receiver_port),
      header_(move(header_data)), token_(move(token)), options_(move(options)),
      body_(move(body)) {}

vector<uint8_t> CoAP_Message::toPacket() {
  auto result = header_.toPacket();

  shared_ptr<ContentFormat> content_format;
  for (auto &option : options_) {
    if (option->getOptionNumber() == OptionNumber::CONTENT_FORMAT) {
      content_format = static_pointer_cast<ContentFormat>(option);
    }
  }

  vector<uint8_t> payload;
  if (!body_.empty() && content_format) {
    payload = encode(content_format, body_);
  }

  result.insert(result.end(), payload.begin(), payload.end());
  return result;
}

string CoAP_Message::getReceiverIP() { return receiver_ip_; }

unsigned int CoAP_Message::getReceiverPort() { return receiver_port_; }

CoAP_Header &CoAP_Message::getHeader() { return header_; }

vector<uint8_t> CoAP_Message::getToken() { return token_; }

vector<shared_ptr<CoAP_Option>> CoAP_Message::getOptions() { return options_; }

vector<string> CoAP_Message::getBody() { return body_; }
} // namespace CoAP