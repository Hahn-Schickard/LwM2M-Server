#include "Write.hpp"

using namespace std;

namespace LwM2M {

WriteRequest::WriteRequest(EndpointPtr endpoint, EelmentIdVariant target_id,
                           DataFormatPtr content)
    : DeviceManagmentRequest(endpoint, MessageType::WRITE),
      target_id_(target_id), content_(content) {}

string WriteRequest::name() { return "WriteRequest"; }

WriteResponse::WriteResponse(EndpointPtr endpoint, ResponseCode response_code)
    : DeviceManagmentResponse(
          endpoint, MessageType::WRITE,
          unordered_set<ResponseCode>{
              ResponseCode::CHANGED, ResponseCode::CONTINUE,
              ResponseCode::BAD_REQUEST, ResponseCode::UNAUTHORIZED,
              ResponseCode::NOT_FOUND, ResponseCode::METHOD_NOT_ALLOWED,
              ResponseCode::NOT_ACCEPTABLE,
              ResponseCode::REQUEST_ENTITY_INCOMPLETE,
              ResponseCode::REQUEST_ENTITY_TOO_LARGE},
          response_code) {
  checkResponseCode(response_code);
}

string WriteResponse::name() { return "WriteResponse"; }
} // namespace LwM2M
