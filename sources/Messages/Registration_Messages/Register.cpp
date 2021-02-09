#include "Register.hpp"

using namespace std;

namespace LwM2M {
RegisterResponse::RegisterResponse(EndpointPtr endpoint,
                                   ResponseCode response_code, string location)
    : ServerResponse(
          endpoint, MessageType::REGISTER, InterfaceType::REGISTRATION,
          unordered_set<ResponseCode>{
              ResponseCode::CREATED, ResponseCode::BAD_REQUEST,
              ResponseCode::FORBIDDEN, ResponseCode::PRECOGNITION_FAILED},
          response_code),
      location_(location) {
  checkResponseCode(response_code);
}

string RegisterResponse::name() { return "RegisterResponse"; }

RegisterRequest::RegisterRequest(
    EndpointPtr endpoint, size_t life_time, LwM2M_Version version,
    unordered_map<unsigned int, vector<unsigned int>> object_instances_map)
    : ClientRequest(endpoint, MessageType::REGISTER,
                    InterfaceType::REGISTRATION),
      life_time_(life_time), version_(version),
      object_instances_map_(object_instances_map) {}

string RegisterRequest::name() { return "RegisterRequest"; }

RegisterResponsePtr RegisterRequest::makeResponse(ResponseCode response_code,
                                                  string location) {
  return make_shared<RegisterResponse>(endpoint_, response_code, location);
}
} // namespace LwM2M
