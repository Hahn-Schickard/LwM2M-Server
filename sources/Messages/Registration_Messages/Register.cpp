#include "Register.hpp"

using namespace std;

namespace LwM2M {
RegisterResponse::RegisterResponse(EndpointPtr endpoint, // NOLINT
    ResponseCode response_code)
    : RegistrationInterfaceResponse(endpoint, // NOLINT
          MessageType::REGISTER,
          unordered_set<ResponseCode>{ResponseCode::BAD_REQUEST,
              ResponseCode::FORBIDDEN, ResponseCode::PRECOGNITION_FAILED},
          response_code) {
  checkResponseCode(response_code);
}

RegisterResponse::RegisterResponse(EndpointPtr endpoint, // NOLINT
    string location)
    : RegistrationInterfaceResponse(endpoint, // NOLINT
          MessageType::REGISTER,
          unordered_set<ResponseCode>{ResponseCode::CREATED},
          ResponseCode::CREATED,
          make_shared<Payload>(
              make_shared<DataFormat>(DataVariant(location)))) {}

string RegisterResponse::name() const { return "RegisterResponse"; }

RegisterRequest::RegisterRequest(EndpointPtr endpoint, // NOLINT
    DeviceMetaInfo device_info) // NOLINT
    : RegistrationInterfaceRequest(endpoint, // NOLINT
          MessageType::REGISTER),
      device_info_(device_info) {} // NOLINT

RegisterRequest::RegisterRequest(EndpointPtr endpoint, // NOLINT
    size_t life_time, DeviceMetaInfo::ObjectInstancesMap object_instances_map,
    optional<string> endpoint_name, LwM2M_Version version,
    optional<BindingType> binding, optional<bool> queue_mode,
    optional<string> sms_number)
    : RegistrationInterfaceRequest(endpoint, // NOLINT
          MessageType::REGISTER),
      device_info_(DeviceMetaInfo(life_time, object_instances_map, // NOLINT
          endpoint_name, version, binding, queue_mode, sms_number)) {} // NOLINT

string RegisterRequest::name() const { return "RegisterRequest"; }

RegisterResponsePtr RegisterRequest::makeResponse(ResponseCode response_code) {
  return make_shared<RegisterResponse>(endpoint_, response_code);
}

RegisterResponsePtr RegisterRequest::makeResponse(string location) { // NOLINT
  return make_shared<RegisterResponse>(endpoint_, location);
}
} // namespace LwM2M
