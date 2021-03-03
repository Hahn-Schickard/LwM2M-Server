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
          response_code,
          make_shared<Payload>(
              make_shared<DataFormat>(DataVariant(location)))) {
  checkResponseCode(response_code);
}

string RegisterResponse::name() { return "RegisterResponse"; }

RegisterRequest::RegisterRequest(
    EndpointPtr endpoint, size_t life_time,
    unordered_map<unsigned int, vector<unsigned int>> object_instances_map,
    optional<string> endpoint_name, LwM2M_Version version,
    optional<BindingType> binding, optional<bool> queue_mode,
    optional<string> sms_number)
    : ClientRequest(endpoint, MessageType::REGISTER,
                    InterfaceType::REGISTRATION),
      life_time_(life_time), object_instances_map_(object_instances_map),
      endpoint_name_(endpoint_name), version_(version), binding_(binding),
      queue_mode_(queue_mode), sms_number_(sms_number) {}

string RegisterRequest::name() { return "RegisterRequest"; }

RegisterResponsePtr RegisterRequest::makeResponse(ResponseCode response_code,
                                                  string location) {
  return make_shared<RegisterResponse>(endpoint_, response_code, location);
}
} // namespace LwM2M
