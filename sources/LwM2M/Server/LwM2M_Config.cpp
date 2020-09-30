#include "LwM2M_Config.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <system_error>

using namespace std;
using json = nlohmann::json;

namespace nlohmann {
void from_json(const json &j, LwM2M::Configuration &config) {
  config.object_descriptors_location =
      j.at("descriptors_location").get<string>();
  config.ip_address = j.at("ip_address").get<string>();
  config.server_port = j.at("server_port").get<unsigned int>();
  config.read_timeout = j.at("read_timeout").get<unsigned int>();
}
} // namespace nlohmann

namespace LwM2M {
Configuration deserialize(const std::string filepath) {
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