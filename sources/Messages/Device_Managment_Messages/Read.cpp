#include "Read.hpp"

using namespace std;

namespace LwM2M {

ReadRequest::ReadRequest(EndpointPtr endpoint, EelmentIdVariant target_id)
    : DeviceManagmentRequest(endpoint, MessageType::READ),
      target_id_(target_id) {}

string ReadRequest::name() { return "ReadRequest"; }

ReadResponse::ReadResponse(EndpointPtr endpoint, ResponseCode response_code,
                           DataFormatPtr content)
    : DeviceManagmentResponse(
          endpoint, MessageType::READ,
          unordered_set<ResponseCode>{
              ResponseCode::CONTENT, ResponseCode::BAD_REQUEST,
              ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
              ResponseCode::METHOD_NOT_ALLOWED, ResponseCode::NOT_ACCEPTABLE},
          response_code),
      content_(content) {
  checkResponseCode(response_code);
}

string ReadResponse::name() { return "ReadResponse"; }

} // namespace LwM2M
