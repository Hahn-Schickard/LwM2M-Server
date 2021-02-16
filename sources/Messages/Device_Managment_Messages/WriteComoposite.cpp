#include "WriteComoposite.hpp"

using namespace std;

namespace LwM2M {

WriteComopositeRequest::WriteComopositeRequest(EndpointPtr endpoint,
                                               vector<TargetContent> content)
    : ServerRequest(endpoint, MessageType::WRITE_COMPOSITE,
                    InterfaceType::DEVICE_MANAGMENT,
                    make_shared<Payload>(content)) {}

string WriteComopositeRequest::name() { return "WriteComopositeRequest"; }

WriteComopositeResponse::WriteComopositeResponse(EndpointPtr endpoint,
                                                 ResponseCode response_code)
    : ClientResponse(endpoint, MessageType::WRITE_COMPOSITE,
                     InterfaceType::DEVICE_MANAGMENT,
                     unordered_set<ResponseCode>{
                         ResponseCode::CHANGED, ResponseCode::BAD_REQUEST,
                         ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
                         ResponseCode::METHOD_NOT_ALLOWED,
                         ResponseCode::NOT_ACCEPTABLE},
                     response_code) {
  checkResponseCode(response_code);
}

string WriteComopositeResponse::name() { return "WriteComopositeResponse"; }

} // namespace LwM2M
