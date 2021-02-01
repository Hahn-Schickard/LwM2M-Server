#include "Read.hpp"

using namespace std;

namespace LwM2M {

ReadRequest::ReadRequest(EndpointPtr endpoint, EelmentIdVariant target_id)
    : ServerRequest(endpoint, MessageType::READ,
                    InterfaceType::DEVICE_MANAGMENT),
      target_id_(target_id) {}

string ReadRequest::name() { return "ReadRequest"; }

ReadResponse::ReadResponse(EndpointPtr endpoint, ResponseCode response_code,
                           DataFormatPtr content)
    : ClientResponse(
          endpoint, MessageType::READ, InterfaceType::DEVICE_MANAGMENT,
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
