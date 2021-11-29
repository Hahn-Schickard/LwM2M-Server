#include "Register.hpp"

using namespace std;

namespace LwM2M {
RegisterResponse::RegisterResponse(EndpointPtr endpoint,
                                   ResponseCode response_code)
    : RegistrationInterfaceResponse(
          endpoint, MessageType::REGISTER,
          unordered_set<ResponseCode>{ResponseCode::BAD_REQUEST,
                                      ResponseCode::FORBIDDEN,
                                      ResponseCode::PRECOGNITION_FAILED},
          response_code) {
  checkResponseCode(response_code);
}

RegisterResponse::RegisterResponse(EndpointPtr endpoint, string location)
    : RegistrationInterfaceResponse(
          endpoint, MessageType::REGISTER,
          unordered_set<ResponseCode>{ResponseCode::CREATED},
          ResponseCode::CREATED,
          make_shared<Payload>(
              make_shared<DataFormat>(DataVariant(location)))) {}

string RegisterResponse::name() { return "RegisterResponse"; }

RegisterRequest::RegisterRequest(EndpointPtr endpoint,
                                 DeviceMetaInfo device_info)
    : RegistrationInterfaceRequest(endpoint, MessageType::REGISTER),
      device_info_(device_info) {}

RegisterRequest::RegisterRequest(
    EndpointPtr endpoint, size_t life_time,
    DeviceMetaInfo::ObjectInstancesMap object_instances_map,
    optional<string> endpoint_name, LwM2M_Version version,
    optional<BindingType> binding, optional<bool> queue_mode,
    optional<string> sms_number)
    : RegistrationInterfaceRequest(endpoint, MessageType::REGISTER),
      device_info_(DeviceMetaInfo(life_time, object_instances_map,
                                  endpoint_name, version, binding, queue_mode,
                                  sms_number)) {}

string RegisterRequest::name() { return "RegisterRequest"; }

RegisterResponsePtr RegisterRequest::makeResponse(ResponseCode response_code) {
  return make_shared<RegisterResponse>(endpoint_, response_code);
}

RegisterResponsePtr RegisterRequest::makeResponse(string location) {
  return make_shared<RegisterResponse>(endpoint_, location);
}
} // namespace LwM2M
