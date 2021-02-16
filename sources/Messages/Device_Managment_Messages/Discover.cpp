#include "Discover.hpp"

using namespace std;

namespace LwM2M {

DiscoverRequest::DiscoverRequest(EndpointPtr endpoint,
                                 ElmentIdVariant target_id)
    : ServerRequest(endpoint, MessageType::DISCOVER,
                    InterfaceType::DEVICE_MANAGMENT,
                    make_shared<Payload>(ElmentIdVariant(target_id))) {}

string DiscoverRequest::name() { return "DiscoverRequest"; }

DiscoverResponse::DiscoverResponse(EndpointPtr endpoint,
                                   ResponseCode response_code,
                                   DataFormatPtr content)
    : ClientResponse(endpoint, MessageType::DISCOVER,
                     InterfaceType::DEVICE_MANAGMENT,
                     unordered_set<ResponseCode>{
                         ResponseCode::CONTENT, ResponseCode::BAD_REQUEST,
                         ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
                         ResponseCode::METHOD_NOT_ALLOWED},
                     response_code, make_shared<Payload>(content)) {
  checkResponseCode(response_code);
}

DiscoverResponse::DiscoverResponse(EndpointPtr endpoint,
                                   ResponseCode response_code)
    : ClientResponse(endpoint, MessageType::DISCOVER,
                     InterfaceType::DEVICE_MANAGMENT,
                     unordered_set<ResponseCode>{
                         ResponseCode::CONTENT, ResponseCode::BAD_REQUEST,
                         ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
                         ResponseCode::METHOD_NOT_ALLOWED},
                     response_code, PayloadPtr()) {
  checkResponseCode(response_code);
}

string DiscoverResponse::name() { return "DiscoverResponse"; }

} // namespace LwM2M
