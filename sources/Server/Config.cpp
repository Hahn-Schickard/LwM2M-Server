#include "Config.hpp"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

using namespace std;
using json = nlohmann::json;

namespace nlohmann {
NLOHMANN_JSON_SERIALIZE_ENUM(LwM2M::ServerBindingType,
    {{LwM2M::ServerBindingType::CoAP, "CoAP"},
        {LwM2M::ServerBindingType::UNKOWN, "Unknown"}})
} // namespace nlohmann

namespace LwM2M {

string getCanonicalPath(const filesystem::path& base, const string& file_loc) {
  if (auto filepath = filesystem::path(file_loc); filepath.is_relative()) {
    return filesystem::canonical(base / filepath).string();
  } else {
    return file_loc;
  }
}

Configuration getConfig(const string& filepath) {
  if (filepath.empty()) {
    throw invalid_argument("Configuration filepath can not be empty when "
                           "calling getConfig(const std::string&)");
  }

  ifstream input_file_stream(filepath);
  if (input_file_stream) {
    json j;
    input_file_stream >> j;

    auto config_loc = filesystem::canonical(filepath).parent_path();
    auto descriptors =
        getCanonicalPath(config_loc, j.at("model_descriptors").get<string>());

    LwM2M::Bindings bindings;
    for (const auto& item : j.at("bindings").items()) {
      auto binding_json = item.value();
      auto type = binding_json.at("type").get<LwM2M::ServerBindingType>();
      auto location = getCanonicalPath(
          config_loc, binding_json.at("location").get<string>());
      bindings.emplace(type, location);
    }

    return Configuration(descriptors, move(bindings));
  } else {
    throw runtime_error(filepath + " does not exist");
  }
}
} // namespace LwM2M
