
#include "Option_Builder.hpp"

#include "Accept.hpp"
#include "ContentFormat.hpp"
#include "ETag.hpp"
#include "IfMatch.hpp"
#include "IfNoneMatch.hpp"
#include "LocationPath.hpp"
#include "LocationQuery.hpp"
#include "MaxAge.hpp"
#include "ProxyScheme.hpp"
#include "ProxyUri.hpp"
#include "Size1.hpp"
#include "UriHost.hpp"
#include "UriPath.hpp"
#include "UriPort.hpp"
#include "UriQuery.hpp"

#include <stdexcept>

#define BYTE_MSB_MASK 0xF0
#define BYTE_LSB_MASK 0xF
#define BYTE_LONG 0xD
#define BYTE_LONG_OFFSET 0xD
#define SHORT_LONG 0xE
#define SHORT_LONG_OFFSET 0x10D
#define PAYLOAD_DELTA 0xF
#define PAYLOAD_MARKER 0xFF

using namespace std;
namespace CoAP {
string toHexString(deque<uint8_t> bytes, size_t ammount) {
  char const hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                              '8', '9', 'A', 'B', 'C', 'D', 'F'};
  string hex;
  for (size_t i = 0; i < ammount; i++) {
    hex += hex_chars[(bytes[i] & BYTE_MSB_MASK) >> 4];
    hex += hex_chars[(bytes[i] & BYTE_LSB_MASK)];
  }
  return hex;
}

string makeAsString(deque<uint8_t> bytes, unsigned short lenght) {
  string tmp;
  for (unsigned short i = 0; i < lenght; i++) {
    tmp += (char)bytes[i];
  }
  return tmp;
}

string makeAsOpaque(deque<uint8_t> bytes, unsigned short lenght) {
  return string(bytes.begin(), bytes.begin() + lenght);
}

vector<uint8_t> makeAsInt(deque<uint8_t> bytes, unsigned short lenght) {
  vector<uint8_t> tmp;
  for (int i = 0; i < lenght; i++) {
    tmp.push_back(bytes[i]);
  }
  return tmp;
}

OptionNumber makeOptionNumber(unsigned int number) {
  OptionNumber option_number = OptionNumber::RESERVED;
  switch (number) {
  case 1: {
    option_number = OptionNumber::IF_MATCH;
    break;
  }
  case 3: {
    option_number = OptionNumber::URI_HOST;
    break;
  }
  case 4: {
    option_number = OptionNumber::ETAG;
    break;
  }
  case 5: {
    option_number = OptionNumber::IF_NONE_MATCH;
    break;
  }
  case 7: {
    option_number = OptionNumber::URI_PORT;
    break;
  }
  case 8: {
    option_number = OptionNumber::LOCATION_PATH;
    break;
  }
  case 11: {
    option_number = OptionNumber::URI_PATH;
    break;
  }
  case 12: {
    option_number = OptionNumber::CONTENT_FORMAT;
    break;
  }
  case 15: {
    option_number = OptionNumber::MAX_AGE;
    break;
  }
  case 17: {
    option_number = OptionNumber::ACCEPT;
    break;
  }
  case 20: {
    option_number = OptionNumber::LOCATION_QUERY;
    break;
  }
  case 35: {
    option_number = OptionNumber::PROXY_URI;
    break;
  }
  case 39: {
    option_number = OptionNumber::PROXY_SCHEME;
    break;
  }
  case 60: {
    option_number = OptionNumber::SIZE_1;
    break;
  }
  default: {
    string error_msg = "Received an unhandled CoAP option: " + number;
    throw domain_error(error_msg);
  }
  }
  return option_number;
}

shared_ptr<CoAP_Option> build(shared_ptr<CoAP_Option> previous,
                              deque<uint8_t> option) {
  unsigned short delta;
  unsigned short lenght;
  size_t option_size;
  OptionNumber option_number;

  if (!option.empty()) {
    if (option[0] != PAYLOAD_MARKER) {
      option_size = 1;

      uint8_t msb = option[0] & BYTE_MSB_MASK;
      switch (msb) {
      case BYTE_LONG: {
        delta = option[1] + BYTE_LONG_OFFSET;
        option_size++;
        break;
      }
      case SHORT_LONG: {
        delta = option[1] + option[2] + SHORT_LONG_OFFSET;
        option_size += 2;
        break;
      }
      case PAYLOAD_DELTA: {
        string error_msg =
            "Malformated CoAP option: Delta set to 0xF, but the byte " +
            toHexString(option, 2) + " is not equal to 0xFF";
        throw domain_error(error_msg);
      }
      default: {
        delta = msb;
        break;
      }
      }

      uint8_t lsb = option[0] & BYTE_LSB_MASK;
      switch (lsb) {
      case BYTE_LONG: {
        lenght = option[1] + BYTE_LONG_OFFSET;
        option_size++;
        break;
      }
      case SHORT_LONG: {
        lenght = option[1] + option[2] + SHORT_LONG_OFFSET;
        option_size += 2;
        break;
      }
      case PAYLOAD_DELTA: {
        string error_msg =
            "Malformated CoAP option: Lenght is set to 0xF, but the byte " +
            toHexString(option, 2) + " is not equal to 0xFF";
        throw domain_error(error_msg);
      }
      default: {
        lenght = lsb;
        break;
      }
      }

      if (previous) {
        option_number = makeOptionNumber(delta + previous->getOptionNumber());
      } else {
        option_number = makeOptionNumber(delta);
      }

      shared_ptr<CoAP_Option> result;
      switch (option_number) {
      case OptionNumber::IF_MATCH: {
        string option_value = string(option.begin() + option_size,
                                     option.begin() + option_size + lenght);
        result = make_shared<IfMatch>(option_value);
        break;
      }
      case OptionNumber::URI_HOST: {
        string option_value = string(option.begin() + option_size,
                                     option.begin() + option_size + lenght);
        result = make_shared<UriHost>(option_value);
        break;
      }
      case OptionNumber::ETAG: {
        string option_value = string(option.begin() + option_size,
                                     option.begin() + option_size + lenght);
        result = make_shared<ETag>(option_value);
        break;
      }
      case OptionNumber::IF_NONE_MATCH: {
        result = make_shared<IfNoneMatch>();
        break;
      }
      case OptionNumber::URI_PORT: {
        vector<uint8_t> option_value(option.begin() + option_size,
                                     option.begin() + option_size + lenght);
        result = make_shared<UriPort>(option_value);
        break;
      }
      case OptionNumber::LOCATION_PATH: {
        string option_value = string(option.begin() + option_size,
                                     option.begin() + option_size + lenght);
        result = make_shared<LocationPath>(option_value);
        break;
      }
      case OptionNumber::URI_PATH: {
        string option_value = string(option.begin() + option_size,
                                     option.begin() + option_size + lenght);
        result = make_shared<UriPath>(option_value);
        break;
      }
      case OptionNumber::CONTENT_FORMAT: {
        vector<uint8_t> option_value(option.begin() + option_size,
                                     option.begin() + option_size + lenght);
        result = make_shared<ContentFormat>(option_value);
        break;
      }
      case OptionNumber::MAX_AGE: {
        vector<uint8_t> option_value(option.begin() + option_size,
                                     option.begin() + option_size + lenght);
        result = make_shared<MaxAge>(option_value);
        break;
      }
      case OptionNumber::URI_QUERY: {
        string option_value = string(option.begin() + option_size,
                                     option.begin() + option_size + lenght);
        result = make_shared<UriQuery>(option_value);
        break;
      }
      case OptionNumber::ACCEPT: {
        vector<uint8_t> option_value(option.begin() + option_size,
                                     option.begin() + option_size + lenght);
        result = make_shared<Accept>(option_value);
        break;
      }
      case OptionNumber::LOCATION_QUERY: {
        string option_value = string(option.begin() + option_size,
                                     option.begin() + option_size + lenght);
        result = make_shared<LocationQuery>(option_value);
        break;
      }
      case OptionNumber::PROXY_URI: {
        string option_value = string(option.begin() + option_size,
                                     option.begin() + option_size + lenght);
        result = make_shared<ProxyUri>(option_value);
        break;
      }
      case OptionNumber::PROXY_SCHEME: {
        string option_value = string(option.begin() + option_size,
                                     option.begin() + option_size + lenght);
        result = make_shared<ProxyScheme>(option_value);
        break;
      }
      case OptionNumber::SIZE_1: {
        vector<uint8_t> option_value(option.begin() + option_size,
                                     option.begin() + option_size + lenght);
        result = make_shared<Size1>(option_value);
        break;
      }
      case OptionNumber::RESERVED:
      default: { throw runtime_error("Option number was not set"); }
      }
      return result;
    } else {
      throw PayloadMarkerDetected();
    }
  } else {
    return shared_ptr<CoAP_Option>();
  }
}
} // namespace CoAP
