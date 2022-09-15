#include "Deregister.hpp"

using namespace std;

namespace LwM2M {
DeregisterResponse::DeregisterResponse(EndpointPtr endpoint, // NOLINT
    ResponseCode response_code)
    : RegistrationInterfaceResponse(endpoint, // NOLINT
          MessageType::DEREGISTER,
          unordered_set<ResponseCode>{
              ResponseCode::BAD_REQUEST, ResponseCode::NOT_FOUND},
          response_code) {
  checkResponseCode(response_code);
}

DeregisterResponse::DeregisterResponse(EndpointPtr endpoint, // NOLINT
    string location)
    : RegistrationInterfaceResponse(endpoint, // NOLINT
          MessageType::DEREGISTER,
          unordered_set<ResponseCode>{ResponseCode::DELETED},
          ResponseCode::DELETED,
          make_shared<Payload>(
              make_shared<DataFormat>(DataVariant(location)))) {}

string DeregisterResponse::name() { return "DeregisterResponse"; }

DeregisterRequest::DeregisterRequest(EndpointPtr endpoint, // NOLINT
    string location)
    : RegistrationInterfaceRequest(endpoint, // NOLINT
          MessageType::DEREGISTER),
      location_(move(location)) {}

string DeregisterRequest::name() { return "DeregisterRequest"; }

DeregisterResponsePtr DeregisterRequest::makeResponse(
    ResponseCode response_code) {
  if (response_code == ResponseCode::DELETED) {
    return make_shared<DeregisterResponse>(endpoint_, location_);
  } else {
    return make_shared<DeregisterResponse>(endpoint_, response_code);
  }
}
} // namespace LwM2M
