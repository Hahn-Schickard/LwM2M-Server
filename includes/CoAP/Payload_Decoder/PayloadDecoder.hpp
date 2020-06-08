#ifndef __COAP_MESSAGE_PAYLOAD_DECODER_HPP
#define __COAP_MESSAGE_PAYLOAD_DECODER_HPP

#include "ContentFormat.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace CoAP {

std::vector<std::string> decode(std::shared_ptr<ContentFormat> format,
                                std::vector<uint8_t> payload);
std::vector<uint8_t> encode(std::shared_ptr<ContentFormat> format,
                            std::vector<std::string> payload);

} // namespace CoAP

#endif //__COAP_MESSAGE_PAYLOAD_DECODER_HPP