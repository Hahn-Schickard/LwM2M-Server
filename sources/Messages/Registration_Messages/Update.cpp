#include "Update.hpp"

using namespace std;

namespace LwM2M {

UpdateResponse::UpdateResponse(EndpointPtr endpoint, ResponseCode response_code)
    : RegistrationInterfaceResponse(
          endpoint, MessageType::UPDATE,
          unordered_set<ResponseCode>{ResponseCode::BAD_REQUEST,
                                      ResponseCode::NOT_FOUND},
          response_code) {
  checkResponseCode(response_code);
}

UpdateResponse::UpdateResponse(EndpointPtr endpoint, string location)
    : RegistrationInterfaceResponse(
          endpoint, MessageType::UPDATE,
          unordered_set<ResponseCode>{ResponseCode::CHANGED},
          ResponseCode::CHANGED,
          make_shared<Payload>(
              make_shared<DataFormat>(DataVariant(location)))) {}

string UpdateResponse::name() { return "UpdateResponse"; }

UpdateRequest::UpdateRequest(EndpointPtr endpoint, string location,
                             DeviceMetaInfo device_info)
    : RegistrationInterfaceRequest(endpoint, MessageType::UPDATE),
      location_(location), device_info_(device_info) {}

UpdateRequest::UpdateRequest(
    EndpointPtr endpoint, string location,
    DeviceMetaInfo::ObjectInstancesMap object_instances_map,
    optional<size_t> lifetime, optional<BindingType> binding,
    optional<string> sms_number)
    : RegistrationInterfaceRequest(endpoint, MessageType::UPDATE),
      location_(location),
      device_info_(DeviceMetaInfo(lifetime, object_instances_map, nullopt,
                                  nullopt, binding, nullopt, sms_number)) {}

string UpdateRequest::name() { return "UpdateRequest"; }

UpdateResponsePtr UpdateRequest::makeResponse(ResponseCode response_code) {
  if (response_code == ResponseCode::CHANGED)
    return make_shared<UpdateResponse>(endpoint_, location_);
  else
    return make_shared<UpdateResponse>(endpoint_, response_code);
}
} // namespace LwM2M
