#ifndef __COAP_OPTION_LOCATION_PATH_DEFINITION_HPP
#define __COAP_OPTION_LOCATION_PATH_DEFINITION_HPP

#include "CoAP_Option.hpp"

#include <string>

namespace CoAP {
class LocationPath : public CoAP_Option {
  std::string value_;

public:
  LocationPath();
  LocationPath(std::string value);

  std::string getValue() override;
};
} // namespace CoAP

#endif //__COAP_OPTION_LOCATION_PATH_DEFINITION_HPP