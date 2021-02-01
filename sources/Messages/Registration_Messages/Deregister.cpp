#include "Deregister.hpp"

using namespace std;

namespace LwM2M {
DeregisterResponse::DeregisterResponse(EndpointPtr endpoint,
                                       ResponseCode response_code)
    : ServerResponse(endpoint, MessageType::DEREGISTER,
                     InterfaceType::REGISTRATION,
                     unordered_set<ResponseCode>{ResponseCode::DELETED,
                                                 ResponseCode::BAD_REQUEST,
                                                 ResponseCode::NOT_FOUND},
                     response_code) {
  checkResponseCode(response_code);
}

string DeregisterResponse::name() { return "DeregisterResponse"; }

DeregisterRequest::DeregisterRequest(EndpointPtr endpoint, string location)
    : ClientRequest(endpoint, MessageType::DEREGISTER,
                    InterfaceType::REGISTRATION),
      location_(location) {}

string DeregisterRequest::name() { return "DeregisterRequest"; }

DeregisterResponsePtr
DeregisterRequest::makeResponse(ResponseCode response_code) {
  return make_shared<DeregisterResponse>(endpoint_, response_code);
}
} // namespace LwM2M
