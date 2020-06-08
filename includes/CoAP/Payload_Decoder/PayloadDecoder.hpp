#ifndef __COAP_MESSAGE_PAYLOAD_DECODER_HPP
#define __COAP_MESSAGE_PAYLOAD_DECODER_HPP

#include "ContentFormat.hpp"
#include "PayloadFormat.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace CoAP {

std::shared_ptr<PayloadFormat> decode(std::shared_ptr<ContentFormat> format,
                                      std::vector<uint8_t> payload);
std::vector<uint8_t> encode(std::shared_ptr<PayloadFormat> format);

} // namespace CoAP

#endif //__COAP_MESSAGE_PAYLOAD_DECODER_HPP