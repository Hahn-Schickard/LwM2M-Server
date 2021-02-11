#include "Read.hpp"

using namespace std;

namespace LwM2M {

ReadRequest::ReadRequest(EndpointPtr endpoint, ElmentIdVariant target_id)
    : ServerRequest(endpoint, MessageType::READ,
                    InterfaceType::DEVICE_MANAGMENT),
      target_id_(target_id) {}

string ReadRequest::name() { return "ReadRequest"; }

ReadResponse::ReadResponse(EndpointPtr endpoint, ResponseCode response_code,
                           const DataFormat &content)
    : ClientResponse(
          endpoint, MessageType::READ, InterfaceType::DEVICE_MANAGMENT,
          unordered_set<ResponseCode>{
              ResponseCode::CONTENT, ResponseCode::BAD_REQUEST,
              ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
              ResponseCode::METHOD_NOT_ALLOWED, ResponseCode::NOT_ACCEPTABLE},
          response_code, make_shared<Payload>(content)) {
  checkResponseCode(response_code);
}

ReadResponse::ReadResponse(EndpointPtr endpoint, ResponseCode response_code)
    : ClientResponse(
          endpoint, MessageType::READ, InterfaceType::DEVICE_MANAGMENT,
          unordered_set<ResponseCode>{
              ResponseCode::CONTENT, ResponseCode::BAD_REQUEST,
              ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
              ResponseCode::METHOD_NOT_ALLOWED, ResponseCode::NOT_ACCEPTABLE},
          response_code, PayloadPtr()) {
  checkResponseCode(response_code);
}

string ReadResponse::name() { return "ReadResponse"; }

} // namespace LwM2M
