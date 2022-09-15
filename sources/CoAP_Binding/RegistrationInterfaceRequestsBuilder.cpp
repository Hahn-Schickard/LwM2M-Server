#include "RegistrationInterfaceRequestsBuilder.hpp"

#include "CoAPS4Cpp/CoRE_Link.hpp"
#include "CoAPS4Cpp/SupportedContentFormats.hpp"
#include "HSCUL/Integer.hpp"

#include <optional>
#include <sstream>
#include <unordered_map>
#include <utility>

using namespace std;
using namespace CoAP;
using namespace HSCUL;

namespace LwM2M {

optional<BindingType> getBindingType(Options options) {
  auto uri_queries = options.equal_range(OptionNumber::URI_QUERY);
  for (auto it = uri_queries.first; it != uri_queries.second; it++) {
    auto option_value = it->second->getValueAsString();
    if (option_value.substr(0, 2) == "b=") {
      auto binding_type = toupper(option_value.at(2));
      switch (binding_type) {
      case 'U': {
        return BindingType::UDP;
      }
      case 'T': {
        return BindingType::TCP;
      }
      case 'S': {
        return BindingType::SMS;
      }
      case 'N': {
        return BindingType::NON_IP;
      }
      }
    }
  }
  return {};
}

optional<LwM2M_Version> getLwM2M_Version(Options options) {
  auto uri_queries = options.equal_range(OptionNumber::URI_QUERY);
  for (auto it = uri_queries.first; it != uri_queries.second; it++) {
    auto option = it->second;
    auto lwm2m_version_tag = option->getValueAsString().substr(0, 6);
    if (lwm2m_version_tag == "lwm2m=") {
      auto version_string = option->getValueAsString().substr(6, 7);
      if (version_string == "1.0") {
        return LwM2M_Version::V1_0;
      }
      // @TODO: enable this block when LwM2M v1.1 is fully implemented
      // else if (version_string == "1.1") {
      //   return LwM2M_Version::V1_1;
      // }
    }
  }
  return {};
}

optional<size_t> getLifetime(Options options) {
  auto uri_queries = options.equal_range(OptionNumber::URI_QUERY);
  for (auto it = uri_queries.first; it != uri_queries.second; it++) {
    auto option = it->second;
    auto lifetime_tag = option->getValueAsString().substr(0, 3);
    if (lifetime_tag == "lt=") {
      return toUnsignedInteger(option->getValueAsString().substr(4));
    }
  }
  return {};
}

optional<string> getEndpointName(Options options) {
  auto uri_queries = options.equal_range(OptionNumber::URI_QUERY);
  for (auto it = uri_queries.first; it != uri_queries.second; it++) {
    auto option = it->second;
    auto endpoint_tag = option->getValueAsString().substr(0, 3);
    if (endpoint_tag == "ep=") {
      return option->getValueAsString().substr(3);
    }
  }
  return {};
}

optional<bool> getQueueMode(Options options) {
  auto uri_queries = options.equal_range(OptionNumber::URI_QUERY);
  for (auto it = uri_queries.first; it != uri_queries.second; it++) {
    auto option = it->second;
    auto queue_tag = option->getValueAsString().substr(0, 1);
    if (queue_tag == "Q") {
      return true;
    }
  }
  return {};
}

optional<string> getSMS(Options options) {
  auto uri_queries = options.equal_range(OptionNumber::URI_QUERY);
  for (auto it = uri_queries.first; it != uri_queries.second; it++) {
    auto option = it->second;
    auto sms_tag = option->getValueAsString().substr(0, 4);
    if (sms_tag == "sms=") {
      return option->getValueAsString().substr(4);
    }
  }
  return {};
}

vector<string> split(const string& s, char delimiter) {
  vector<string> tokens;
  string token;
  istringstream token_stream(s);
  while (getline(token_stream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

pair<unsigned int, optional<unsigned int>> makeObjectInstancePair(
    CoRE_Link link) {
  vector<string> uri_targets = split(link.getTarget(), '/');
  // ignore empty root element
  if (uri_targets.begin()->empty()) {
    uri_targets.erase(uri_targets.begin());
  }
  if (uri_targets.size() == 1) {
    // object without an instance (should be created)
    return make_pair<unsigned int, optional<unsigned int>>(
        toUnsignedInteger(uri_targets.at(0)), nullopt);
  } else if (uri_targets.size() == 2) {
    // object with an instance
    return make_pair<unsigned int, optional<unsigned int>>(
        toUnsignedInteger(uri_targets.at(0)),
        toUnsignedInteger(uri_targets.at(1)));
  } else {
    string error_msg = "CoRE Link contains more than 2 uri targets";
    throw logic_error(error_msg);
  }
}

void addToMap(unordered_map<unsigned int, vector<unsigned int>>& map,
    pair<unsigned int, optional<unsigned int>> instance) {
  // Ignore object without an instance for now
  if (instance.second.has_value()) {
    auto it = map.find(instance.first);
    if (it != map.end()) {
      it->second.push_back(instance.second.value());
    } else {
      map.emplace(
          instance.first, vector<unsigned int>{instance.second.value()});
    }
  }
}

unordered_map<unsigned int, vector<unsigned int>> getObjectList(
    const CoAP::PayloadPtr& payload) {
  unordered_map<unsigned int, vector<unsigned int>> result;
  if (payload) {
    CoRE_Links core_links;
    try {
      core_links = decode<CoRE_Links>(payload);
    } catch (CoAP::WrongContentFormatTypeDecoder& ex) {
      auto content_format = ex.format_type_;
      uint16_t octect_stream_index =
          CoAP::ContentFormatEncodings::Octet_Stream::index;
      if (content_format->getIndex() == octect_stream_index) {
        // do manual decoding, because someone was naughty and did NOT set the
        // proper Content Format option
        auto buffer = payload->getBytes();
        core_links = CoRE_Links(string(buffer.begin(), buffer.end()));
      } else {
        string error_msg = to_string(content_format->getIndex()) +
            " is not a supported CoAP Content Format type for "
            "registration interface!";
        throw runtime_error(error_msg);
      }
    }
    for (auto core_link : core_links.getLinks()) {
      // ignore root and empty targets
      if (core_link.getTarget() != "/" && !core_link.getTarget().empty()) {
        auto object_instance = makeObjectInstancePair(core_link);
        addToMap(result, object_instance);
      }
    }
  }
  return result;
}

RegisterRequestPtr buildRegisterRequest(const CoAP::MessagePtr& message) {
  auto endpoint =
      make_shared<Endpoint>(message->getAddressIP(), message->getAddressPort());

  try {
    auto version = getLwM2M_Version(message->getOptions()).value();
    try {
      auto life_time = getLifetime(message->getOptions()).value();
      auto object_instances_map = getObjectList(message->getPayload());
      auto endpoint_name = getEndpointName(message->getOptions());
      auto binding = getBindingType(message->getOptions());
      auto queue_mode = getQueueMode(message->getOptions());
      auto sms_number = getSMS(message->getOptions());

      return make_shared<RegisterRequest>(endpoint, life_time,
          object_instances_map, endpoint_name, version, binding, queue_mode,
          sms_number);
    } catch (bad_optional_access& ex) {
      throw RegistrationInterfaceError(
          make_shared<RegisterResponse>(endpoint, ResponseCode::BAD_REQUEST));
    }
  } catch (bad_optional_access& ex) {
    throw RegistrationInterfaceError(make_shared<RegisterResponse>(
        endpoint, ResponseCode::PRECOGNITION_FAILED));
  }
}

optional<string> getLocation(Options options) {
  auto uri_paths = options.equal_range(OptionNumber::URI_PATH);
  for (auto it = uri_paths.first; it != uri_paths.second; it++) {
    auto option = it->second;
    if (option->getValueAsString() == "rd") {
      auto location_uri = it;
      location_uri++;
      if (location_uri != uri_paths.second) {
        return location_uri->second->getValueAsString();
      }
    }
  }
  return {};
}

UpdateRequestPtr buildUpdateRequest(const CoAP::MessagePtr& message) {
  auto endpoint =
      make_shared<Endpoint>(message->getAddressIP(), message->getAddressPort());
  try {
    auto location = getLocation(message->getOptions()).value();
    auto object_instances_map = getObjectList(message->getPayload());
    auto life_time = getLifetime(message->getOptions()).value();
    auto binding = getBindingType(message->getOptions());
    auto sms_number = getSMS(message->getOptions());
    return make_shared<UpdateRequest>(endpoint, location, object_instances_map,
        life_time, binding, sms_number);
  } catch (bad_optional_access& ex) {
    throw RegistrationInterfaceError(
        make_shared<UpdateResponse>(endpoint, ResponseCode::BAD_REQUEST));
  }
}

DeregisterRequestPtr buildDeregisterRequest(const CoAP::MessagePtr& message) {
  auto endpoint =
      make_shared<Endpoint>(message->getAddressIP(), message->getAddressPort());
  try {
    auto location = getLocation(message->getOptions()).value();
    return make_shared<DeregisterRequest>(endpoint, location);
  } catch (bad_optional_access& ex) {
    throw RegistrationInterfaceError(
        make_shared<DeregisterResponse>(endpoint, ResponseCode::BAD_REQUEST));
  }
}

} // namespace LwM2M
