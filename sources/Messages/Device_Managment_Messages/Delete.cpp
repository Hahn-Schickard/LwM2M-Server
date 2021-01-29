#include "Delete.hpp"

using namespace std;

namespace LwM2M {
DeleteRequest::DeleteRequest(EndpointPtr endpoint, ObjectInstanceID target_id)
    : DeviceManagmentRequest(endpoint, MessageType::DELETE),
      target_id_(target_id) {}

string DeleteRequest::name() { return "DeleteRequest"; }

DeleteResponse::DeleteResponse(EndpointPtr endpoint, ResponseCode response_code)
    : DeviceManagmentResponse(
          endpoint, MessageType::DELETE,
          unordered_set<ResponseCode>{
              ResponseCode::DELETED, ResponseCode::BAD_REQUEST,
              ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
              ResponseCode::METHOD_NOT_ALLOWED},
          response_code) {
  checkResponseCode(response_code);
}

string DeleteResponse::name() { return "DeleteResponse"; }
}; // namespace LwM2M
