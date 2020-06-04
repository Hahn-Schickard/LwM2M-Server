#ifndef __COAP_OPTION_BUILDER_HPP
#define __COAP_OPTION_BUILDER_HPP

#include "CoAP_Option.hpp"

#include <cstdint>
#include <deque>
#include <exception>
#include <memory>

namespace CoAP {
struct PayloadMarkerDetected : public std::exception {
  const char *what() const throw() { return "Payload Marker Detected"; }
};

std::shared_ptr<CoAP_Option> build(std::shared_ptr<CoAP_Option> previous,
                                   std::deque<uint8_t> &options);
} // namespace CoAP

#endif //__COAP_OPTION_BUILDER_HPP