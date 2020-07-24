#include "CoAP_Option.hpp"

#include <stdexcept>

using namespace std;
namespace CoAP {
string toString(OptionNumber option) {
  string result;
  switch (option) {
  case OptionNumber::RESERVED: {
    result = "Reserved";
    break;
  }
  case OptionNumber::IF_MATCH: {
    result = "If Match";
    break;
  }
  case OptionNumber::URI_HOST: {
    result = "URI Host";
    break;
  }
  case OptionNumber::ETAG: {
    result = "ETag";
    break;
  }
  case OptionNumber::IF_NONE_MATCH: {
    result = "If None Match";
    break;
  }
  case OptionNumber::OBSERVE: {
    result = "Observe";
    break;
  }
  case OptionNumber::URI_PORT: {
    result = "URI Port";
    break;
  }
  case OptionNumber::LOCATION_PATH: {
    result = "Location Path";
    break;
  }
  case OptionNumber::OSCORE: {
    result = "Oscore";
    break;
  }
  case OptionNumber::URI_PATH: {
    result = "URI Path";
    break;
  }
  case OptionNumber::CONTENT_FORMAT: {
    result = "Content Format";
    break;
  }
  case OptionNumber::MAX_AGE: {
    result = "Max Age";
    break;
  }
  case OptionNumber::URI_QUERY: {
    result = "URI Query";
    break;
  }
  case OptionNumber::ACCEPT: {
    result = "Accept";
    break;
  }
  case OptionNumber::LOCATION_QUERY: {
    result = "Location Query";
    break;
  }
  case OptionNumber::BLOCK_2: {
    result = "Block 2";
    break;
  }
  case OptionNumber::BLOCK_1: {
    result = "Block 1";
    break;
  }
  case OptionNumber::SIZE_2: {
    result = "Size 2";
    break;
  }
  case OptionNumber::PROXY_URI: {
    result = "Proxy URI";
    break;
  }
  case OptionNumber::PROXY_SCHEME: {
    result = "Proxy Scheme";
    break;
  }
  case OptionNumber::SIZE_1: {
    result = "Size 1";
    break;
  }
  case OptionNumber::NO_RESPONSE: {
    result = "No Response";
    break;
  }
  default: {
    result = "Malformated";
    break;
  }
  }
  return result;
}

Option::Option() : Option(OptionNumber::RESERVED, 0, false, false, false, 0) {}

Option::Option(OptionNumber option_number, size_t option_size, bool critical,
               bool repeatable, bool unsafe, size_t max_size)
    : option_number_(option_number), option_size_(option_size),
      critical_(critical), repeatable_(repeatable), unsafe_(unsafe),
      max_size_(max_size) {
  if (max_size_ < option_size_) {
    string error_msg = "Given option exceeds maximum lenght value of " +
                       to_string(max_size_) + " bytes";
    throw out_of_range(error_msg);
  }
}

OptionNumber Option::getOptionNumber() { return option_number_; }

string Option::getValue() { return string(); }

size_t Option::size() { return option_size_; }

bool Option::isCritical() { return critical_; }

bool Option::isRepeatable() { return repeatable_; }

bool Option::isUnsafe() { return unsafe_; }
} // namespace CoAP