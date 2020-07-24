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

struct OptionIsNotImplemented : public std::runtime_error {
  OptionIsNotImplemented(std::string const &option_name)
      : std::runtime_error(std::string("Option " + option_name +
                                       " building is not implemented!")) {}
};

std::vector<uint8_t> encode(std::vector<std::shared_ptr<Option>> options);

std::shared_ptr<Option> build(OptionNumber option, std::string value);

std::shared_ptr<Option> build(std::shared_ptr<Option> previous,
                              std::deque<uint8_t> &options);
} // namespace CoAP

#endif //__COAP_OPTION_BUILDER_HPP