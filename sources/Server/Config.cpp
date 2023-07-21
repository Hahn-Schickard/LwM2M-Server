#include "Config.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <system_error>

using namespace std;
using json = nlohmann::json;

namespace nlohmann {
// NOLINTNEXTLINE
NLOHMANN_JSON_SERIALIZE_ENUM(LwM2M::ServerBindingType,
    {{LwM2M::ServerBindingType::CoAP, "CoAP"},
        {LwM2M::ServerBindingType::UNKOWN, "Unknown"}})

// NOLINTBEGIN(readability-identifier-naming)
void from_json(const json& j, LwM2M::Bindings& bindings) {
  for (auto const& elem : j) {
    auto type = elem.at("type").get<LwM2M::ServerBindingType>();
    auto location = elem.at("location").get<string>();
    bindings.emplace(type, location);
  }
}

void from_json(const json& j, LwM2M::Configuration& config) {
  config.model_descriptors_ = j.at("model_descriptors").get<string>();
  config.bindings_ = j.at("bindings").get<LwM2M::Bindings>();
}
} // namespace nlohmann

namespace LwM2M {

string toString(ServerBindingType type) {
  switch (type) {
  case ServerBindingType::CoAP: {
    return "CoAP";
  }
  default: {
    return "Unknown";
  }
  }
}
// NOLINTEND(readability-identifier-naming)

Configuration getConfig(const string& filepath) {
  ifstream input_file_stream(filepath);
  if (input_file_stream) {
    json j;
    input_file_stream >> j;
    return j.get<Configuration>();
  } else {
    throw system_error(make_error_code(errc::no_such_file_or_directory));
  }
}
} // namespace LwM2M
