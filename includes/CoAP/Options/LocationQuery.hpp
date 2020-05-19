#ifndef __COAP_OPTION_LOCATION_QUERY_DEFINITION_HPP
#define __COAP_OPTION_LOCATION_QUERY_DEFINITION_HPP

#include "CoAP_Option.hpp"

#include <string>

namespace CoAP {
class LocationQuery : public CoAP_Option {
  std::string value_;

public:
  LocationQuery();
  LocationQuery(std::string value);

  std::string getValue() override;
};
} // namespace CoAP

#endif //__COAP_OPTION_LOCATION_QUERY_DEFINITION_HPP