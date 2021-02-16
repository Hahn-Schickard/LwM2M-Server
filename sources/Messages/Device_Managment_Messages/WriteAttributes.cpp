#include "WriteAttributes.hpp"

using namespace std;

namespace LwM2M {

WriteAttributesRequest::WriteAttributesRequest(EndpointPtr endpoint)
    : ServerRequest(endpoint, MessageType::WRITE_ATTRIBUTES,
                    InterfaceType::DEVICE_MANAGMENT) {}

WriteAttributesRequest::WriteAttributesRequest(EndpointPtr endpoint,
                                               vector<TargetAttribute> content)
    : ServerRequest(endpoint, MessageType::WRITE_ATTRIBUTES,
                    InterfaceType::DEVICE_MANAGMENT,
                    make_shared<Payload>(content)) {}

string WriteAttributesRequest::name() { return "WriteAttributesRequest"; }

WriteAttributesResponse::WriteAttributesResponse(EndpointPtr endpoint,
                                                 ResponseCode response_code)
    : ClientResponse(endpoint, MessageType::WRITE_ATTRIBUTES,
                     InterfaceType::DEVICE_MANAGMENT,
                     unordered_set<ResponseCode>{
                         ResponseCode::CHANGED, ResponseCode::BAD_REQUEST,
                         ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
                         ResponseCode::METHOD_NOT_ALLOWED},
                     response_code) {
  checkResponseCode(response_code);
}

string WriteAttributesResponse::name() { return "WriteAttributesResponse"; }
} // namespace LwM2M
