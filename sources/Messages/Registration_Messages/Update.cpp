#include "Update.hpp"

using namespace std;

namespace LwM2M {

UpdateResponse::UpdateResponse(EndpointPtr endpoint, // NOLINT
    ResponseCode response_code)
    : RegistrationInterfaceResponse(endpoint, // NOLINT
          MessageType::Update,
          unordered_set<ResponseCode>{
              ResponseCode::Bad_Request, ResponseCode::Not_Found},
          response_code) {
  checkResponseCode(response_code);
}

UpdateResponse::UpdateResponse(EndpointPtr endpoint, // NOLINT
    string location)
    : RegistrationInterfaceResponse(endpoint, // NOLINT
          MessageType::Update,
          unordered_set<ResponseCode>{ResponseCode::Changed},
          ResponseCode::Changed,
          make_shared<Payload>(
              make_shared<DataFormat>(DataVariant(location)))) {}

string UpdateResponse::name() const { return "UpdateResponse"; }

UpdateRequest::UpdateRequest(EndpointPtr endpoint, std::string location)
    : RegistrationInterfaceRequest(endpoint, // NOLINT
          MessageType::Update),
      location_(move(location)), device_info_(nullopt), keep_alive_(true) {}

UpdateRequest::UpdateRequest(EndpointPtr endpoint, // NOLINT
    string location, DeviceMetaInfo device_info) // NOLINT
    : RegistrationInterfaceRequest(endpoint, // NOLINT
          MessageType::Update),
      location_(move(location)), device_info_(device_info) {} // NOLINT

UpdateRequest::UpdateRequest(EndpointPtr endpoint, // NOLINT
    string location, DeviceMetaInfo::ObjectInstancesMap object_instances_map,
    optional<size_t> lifetime, optional<BindingType> binding,
    optional<string> sms_number)
    : RegistrationInterfaceRequest(endpoint, // NOLINT
          MessageType::Update),
      location_(move(location)),
      device_info_(DeviceMetaInfo(lifetime, // NOLINT
          object_instances_map, nullopt, // NOLINT
          nullopt, binding, nullopt, sms_number)) // NOLINT
{}

string UpdateRequest::name() const { return "UpdateRequest"; }

bool UpdateRequest::isKeepAlive() const { return keep_alive_; }

UpdateResponsePtr UpdateRequest::makeResponse(ResponseCode response_code) {
  if (response_code == ResponseCode::Changed) {
    return make_shared<UpdateResponse>(endpoint_, location_);
  } else {
    return make_shared<UpdateResponse>(endpoint_, response_code);
  }
}
} // namespace LwM2M
