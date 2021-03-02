#include "CoAP_Config.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

namespace nlohmann {

void from_json(const json &j, LwM2M::CoAP_Config &config) {
  config.address_ = j.at("server_address").get<string>();
  config.port_ = j.at("server_port").get<int>();
}
} // namespace nlohmann

namespace LwM2M {
CoAP_Config getCoAP_Config(const std::string filepath) {
  ifstream input_file_stream(filepath);
  if (input_file_stream) {
    json j;
    input_file_stream >> j;
    return j.get<CoAP_Config>();
  } else {
    return CoAP_Config();
  }
}
} // namespace LwM2M
