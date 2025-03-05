#include "Deregister.hpp"

using namespace std;

namespace LwM2M {
DeregisterResponse::DeregisterResponse(EndpointPtr endpoint, // NOLINT
    ResponseCode response_code)
    : RegistrationInterfaceResponse(endpoint, // NOLINT
          MessageType::Deregister,
          unordered_set<ResponseCode>{
              ResponseCode::Bad_Request, ResponseCode::Not_Found},
          response_code) {
  checkResponseCode(response_code);
}

DeregisterResponse::DeregisterResponse(EndpointPtr endpoint, // NOLINT
    string location)
    : RegistrationInterfaceResponse(endpoint, // NOLINT
          MessageType::Deregister,
          unordered_set<ResponseCode>{ResponseCode::Deleted},
          ResponseCode::Deleted,
          make_shared<Payload>(
              make_shared<DataFormat>(DataVariant(location)))) {}

string DeregisterResponse::name() const { return "DeregisterResponse"; }

DeregisterRequest::DeregisterRequest(EndpointPtr endpoint, // NOLINT
    string location)
    : RegistrationInterfaceRequest(endpoint, // NOLINT
          MessageType::Deregister),
      location_(move(location)) {}

string DeregisterRequest::name() const { return "DeregisterRequest"; }

DeregisterResponsePtr DeregisterRequest::makeResponse(
    ResponseCode response_code) {
  if (response_code == ResponseCode::Deleted) {
    return make_shared<DeregisterResponse>(endpoint_, location_);
  } else {
    return make_shared<DeregisterResponse>(endpoint_, response_code);
  }
}
} // namespace LwM2M
