#include "Create.hpp"

using namespace std;

namespace LwM2M {

CreateRequest::CreateRequest(EndpointPtr endpoint, ObjectID target_id,
                             DataFormatPtr content)
    : ServerRequest(endpoint, MessageType::CREATE,
                    InterfaceType::DEVICE_MANAGMENT),
      target_id_(target_id), content_(content) {}

string CreateRequest::name() { return "CreateRequest"; }

CreateResponse::CreateResponse(EndpointPtr endpoint, ResponseCode response_code)
    : ClientResponse(
          endpoint, MessageType::CREATE, InterfaceType::DEVICE_MANAGMENT,
          unordered_set<ResponseCode>{
              ResponseCode::CREATED, ResponseCode::BAD_REQUEST,
              ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
              ResponseCode::METHOD_NOT_ALLOWED, ResponseCode::NOT_ACCEPTABLE},
          response_code) {
  checkResponseCode(response_code);
}

string CreateResponse::name() { return "CreateResponse"; }
} // namespace LwM2M
