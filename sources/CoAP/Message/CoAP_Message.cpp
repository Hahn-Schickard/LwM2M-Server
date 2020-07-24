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
  vector<uint8_t> result;
  if (!parent.empty()) {
    result = vector<uint8_t>(parent.begin() + start_index,
                             parent.begin() + end_index);
    parent.erase(parent.begin() + start_index, parent.begin() + end_index);
  }
  return result;
}

void Message::processToken(vector<uint8_t> &udp_datagram) {
  if (!udp_datagram.empty()) {
    uint8_t token_lenght = header_.getTokenLenght();
    if (token_lenght > 0) {
      token_ = removeSubvector(udp_datagram, token_lenght);
    }
  }
}

void Message::processOptions(vector<uint8_t> &udp_datagram) {
  if (!udp_datagram.empty()) {
    deque<uint8_t> payload(udp_datagram.begin(), udp_datagram.end());

    auto previous = shared_ptr<Option>();
    shared_ptr<Option> current;
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
    udp_datagram = vector<uint8_t>(payload.begin(), payload.end());
  }
}

void Message::processPayload(vector<uint8_t> &udp_datagram) {
  if (!udp_datagram.empty()) {
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
}

Message::Message() : Message(string(), 0, vector<uint8_t>()) {}

Message::Message(string receiver_ip, unsigned int receiver_port,
                 vector<uint8_t> udp_datagram)
    : receiver_ip_(receiver_ip), receiver_port_(receiver_port) {
  vector<uint8_t> bytestream = move(udp_datagram);
  header_ = Header(removeSubvector(bytestream, HEADER_SIZE));
  processToken(bytestream);
  processOptions(bytestream);
  processPayload(bytestream);
}

Message::Message(string receiver_ip, unsigned int receiver_port,
                 Header header_data, vector<uint8_t> token,
                 vector<shared_ptr<Option>> options,
                 shared_ptr<PayloadFormat> body)
    : receiver_ip_(receiver_ip), receiver_port_(receiver_port),
      header_(move(header_data)), token_(move(token)), options_(move(options)),
      body_(move(body)) {}

vector<uint8_t> Message::toPacket() {
  auto result = header_.toPacket();

  result.insert(result.end(), token_.begin(), token_.end());

  vector<uint8_t> option_pack = encode(options_);
  result.insert(result.end(), option_pack.begin(), option_pack.end());

  if (body_) {
    vector<uint8_t> payload = encode(body_);
    result.insert(result.end(), payload.begin(), payload.end());
  }

  return result;
}

string Message::getReceiverIP() const { return receiver_ip_; }

unsigned int Message::getReceiverPort() const { return receiver_port_; }

const Header Message::getHeader() const {
  auto header_copy(header_);
  return header_copy;
}

vector<uint8_t> Message::getToken() const { return token_; }

vector<shared_ptr<Option>> Message::getOptions() const { return options_; }

shared_ptr<PayloadFormat> Message::getBody() const { return body_; }
} // namespace CoAP