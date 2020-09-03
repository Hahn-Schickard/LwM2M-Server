
#include "Option_Builder.hpp"
#include "Accept.hpp"
#include "ContentFormat.hpp"
#include "ETag.hpp"
#include "IfMatch.hpp"
#include "IfNoneMatch.hpp"
#include "LocationPath.hpp"
#include "LocationQuery.hpp"
#include "MaxAge.hpp"
#include "PrimitiveConverter.hpp"
#include "ProxyScheme.hpp"
#include "ProxyUri.hpp"
#include "Size1.hpp"
#include "UriHost.hpp"
#include "UriPath.hpp"
#include "UriPort.hpp"
#include "UriQuery.hpp"

#include <algorithm>
#include <netinet/in.h>
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
  case 6: {
    option_number = OptionNumber::OBSERVE;
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
  case 9: {
    option_number = OptionNumber::OSCORE;
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
  case 14: {
    option_number = OptionNumber::MAX_AGE;
    break;
  }
  case 15: {
    option_number = OptionNumber::URI_QUERY;
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
  case 23: {
    option_number = OptionNumber::BLOCK_2;
    break;
  }
  case 27: {
    option_number = OptionNumber::BLOCK_1;
    break;
  }
  case 28: {
    option_number = OptionNumber::SIZE_2;
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
  case 258: {
    option_number = OptionNumber::NO_RESPONSE;
    break;
  }
  default: {
    string error_msg = "Received an unhandled CoAP option: " + number;
    throw domain_error(move(error_msg));
  }
  }
  return option_number;
}

size_t oneAtATimeHash(const string &value) {
  string buffer(value);
  transform(buffer.begin(), buffer.end(), buffer.begin(), ::toupper);
  // Bob Jenkins One at a Time hash
  size_t hash = 0;
  for (unsigned int i = 0; i < buffer.size(); i++) {
    hash += buffer[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

class ContentFormatHash {
  size_t short_hash_;
  size_t long_hash_;

public:
  ContentFormatHash(const string &short_hash, const string &long_hash)
      : short_hash_(oneAtATimeHash(short_hash)),
        long_hash_(oneAtATimeHash(long_hash)) {}

  size_t constexpr short_hash() { return short_hash_; }
  size_t constexpr long_hash() { return long_hash_; }
};

static ContentFormatHash PLAIN_TEXT =
    ContentFormatHash("PLAIN_TEXT", toString(ContentFormatType::PLAIN_TEXT));
static ContentFormatHash CORE_LINK =
    ContentFormatHash("CORE_LINK", toString(ContentFormatType::CORE_LINK));
static ContentFormatHash OPAQUE =
    ContentFormatHash("OPAQUE", toString(ContentFormatType::OPAQUE));
static ContentFormatHash CBOR =
    ContentFormatHash("CBOR", toString(ContentFormatType::CBOR));
static ContentFormatHash SENML_JSON =
    ContentFormatHash("SENML_JSON", toString(ContentFormatType::SENML_JSON));
static ContentFormatHash SENML_CBOR =
    ContentFormatHash("SENML_CBOR", toString(ContentFormatType::SENML_CBOR));

ContentFormatType getContentTypeFromString(const string &value) {
  size_t value_hash = oneAtATimeHash(value);
  if (value_hash == PLAIN_TEXT.long_hash() ||
      value_hash == PLAIN_TEXT.short_hash()) {
    return ContentFormatType::PLAIN_TEXT;
  } else if (value_hash == CORE_LINK.long_hash() ||
             value_hash == CORE_LINK.short_hash()) {
    return ContentFormatType::CORE_LINK;
  } else if (value_hash == OPAQUE.long_hash() ||
             value_hash == OPAQUE.short_hash()) {
    return ContentFormatType::OPAQUE;
  } else if (value_hash == CBOR.long_hash() ||
             value_hash == CBOR.short_hash()) {
    return ContentFormatType::CBOR;
  } else if (value_hash == SENML_JSON.long_hash() ||
             value_hash == SENML_JSON.short_hash()) {
    return ContentFormatType::SENML_JSON;
  } else if (value_hash == SENML_CBOR.long_hash() ||
             value_hash == SENML_CBOR.short_hash()) {
    return ContentFormatType::SENML_CBOR;
  } else {
    string error_msg = "Unrecognized content format type: " + value;
    throw domain_error(move(error_msg));
  }
}

shared_ptr<Option> build(OptionNumber option, string value) {
  shared_ptr<Option> result;
  switch (option) {
  case OptionNumber::IF_MATCH: {
    result = make_shared<IfMatch>(move(value));
    break;
  }
  case OptionNumber::URI_HOST: {
    result = make_shared<UriHost>(move(value));
    break;
  }
  case OptionNumber::ETAG: {
    result = make_shared<ETag>(move(value));
    break;
  }
  case OptionNumber::IF_NONE_MATCH: {
    result = make_shared<IfNoneMatch>();
    break;
  }
  case OptionNumber::OBSERVE: {
    throw OptionIsNotImplemented("Observe");
  }
  case OptionNumber::URI_PATH: {
    result = make_shared<UriPath>(move(value));
    break;
  }
  case OptionNumber::CONTENT_FORMAT: {
    result = make_shared<ContentFormat>(stoi(move(value)));
    break;
  }
  case OptionNumber::MAX_AGE: {
    result = make_shared<MaxAge>((uint64_t)stoi(value));
    break;
  }
  case OptionNumber::URI_QUERY: {
    result = make_shared<UriQuery>(move(value));
    break;
  }
  case OptionNumber::ACCEPT: {
    result = make_shared<Accept>(stoi(move(value)));
    break;
  }
  case OptionNumber::LOCATION_QUERY: {
    result = make_shared<LocationQuery>(move(value));
    break;
  }
  case OptionNumber::BLOCK_2: {
    throw OptionIsNotImplemented("Block 2");
  }
  case OptionNumber::BLOCK_1: {
    throw OptionIsNotImplemented("Block 1");
  }
  case OptionNumber::SIZE_2: {
    throw OptionIsNotImplemented("Size 2");
  }
  case OptionNumber::PROXY_URI: {
    result = make_shared<ProxyUri>(move(value));
    break;
  }
  case OptionNumber::PROXY_SCHEME: {
    result = make_shared<ProxyScheme>(move(value));
    break;
  }
  case OptionNumber::SIZE_1: {
    result = make_shared<Size1>((uint64_t)stoi(value));
    break;
  }
  case OptionNumber::NO_RESPONSE: {
    throw OptionIsNotImplemented("No Response");
  }
  case OptionNumber::OSCORE: {
    throw OptionIsNotImplemented("Oscore");
  }
  case OptionNumber::URI_PORT: {
    result = make_shared<UriPort>((uint16_t)stoi(value));
    break;
  }
  case OptionNumber::LOCATION_PATH: {
    result = make_shared<LocationPath>(move(value));
    break;
  }
  case OptionNumber::RESERVED:
  default: {
    string error_msg = toString(option) + " is not handled";
    throw domain_error(error_msg);
  }
  }
  return move(result);
}

vector<uint8_t> makeOptionHeader(uint16_t delta, uint16_t lenght = 0) {
  uint8_t first_byte = 0;
  vector<uint8_t> extentions;
  if (delta < BYTE_LONG)
    first_byte += delta << 4;
  else if (delta < 255) {
    first_byte += BYTE_LONG << 4;
    extentions.push_back(delta - BYTE_LONG_OFFSET);
  } else {
    first_byte += SHORT_LONG << 4;
    vector<uint8_t> short_pack =
        utility::toBytes((uint16_t)(delta - SHORT_LONG_OFFSET));
    extentions.insert(extentions.end(), short_pack.begin(), short_pack.end());
  }

  if (lenght <= BYTE_LONG)
    first_byte += lenght;
  else if (lenght < 255) {
    first_byte += BYTE_LONG;
    extentions.push_back(lenght - BYTE_LONG_OFFSET);
  } else {
    first_byte += SHORT_LONG << 4;
    vector<uint8_t> short_pack =
        utility::toBytes((uint16_t)(lenght - SHORT_LONG_OFFSET));
    extentions.insert(extentions.end(), short_pack.begin(), short_pack.end());
  }

  vector<uint8_t> result;
  result.push_back(first_byte);
  if (!extentions.empty())
    result.insert(result.end(), extentions.begin(), extentions.end());

  return result;
}

vector<uint8_t> encode(vector<shared_ptr<Option>> options) {
  vector<uint8_t> result;
  if (!options.empty()) {
    auto previous_option = make_shared<Option>();
    for (auto option : options) {
      vector<uint8_t> header = makeOptionHeader(
          static_cast<uint16_t>(option->getOptionNumber()) -
              static_cast<uint16_t>(previous_option->getOptionNumber()),
          option->size());
      result.insert(result.end(), header.begin(), header.end());
      vector<uint8_t> value = option->getValue();
      result.insert(result.end(), value.begin(), value.end());
      previous_option = option;
    }
  }
  return result;
}

shared_ptr<Option> build(shared_ptr<Option> previous, deque<uint8_t> &option) {
  unsigned short delta;
  unsigned short lenght;
  size_t option_size;
  OptionNumber option_number;

  if (!option.empty()) {
    if (option[0] != PAYLOAD_MARKER) {
      option_size = 1;

      uint8_t msb = option[0] >> 4;
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
        throw domain_error(move(error_msg));
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
        throw domain_error(move(error_msg));
      }
      default: {
        lenght = lsb;
        break;
      }
      }

      if (previous) {
        option_number = makeOptionNumber(
            delta + static_cast<int>(previous->getOptionNumber()));
      } else {
        option_number = makeOptionNumber(delta);
      }

      shared_ptr<Option> result;
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
      option.erase(option.begin(),
                   option.begin() + option_size + result->size());
      return result;
    } else {
      throw PayloadMarkerDetected();
    }
  } else {
    return shared_ptr<Option>();
  }
}
} // namespace CoAP
