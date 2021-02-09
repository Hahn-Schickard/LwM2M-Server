#include "Execute.hpp"

using namespace std;

namespace LwM2M {
ExecuteRequest::ExecuteRequest(EndpointPtr endpoint, ResourceID target)
    : ServerRequest(endpoint, MessageType::EXECUTE,
                    InterfaceType::DEVICE_MANAGMENT),
      target_(target) {}

ExecuteRequest::ExecuteRequest(EndpointPtr endpoint, ResourceID target,
                               vector<DataVariant> arguments)
    : ServerRequest(endpoint, MessageType::EXECUTE,
                    InterfaceType::DEVICE_MANAGMENT),
      target_(target), arguments_(arguments) {}

string ExecuteRequest::name() { return "ExecuteRequest"; }

ExecuteResponse::ExecuteResponse(EndpointPtr endpoint,
                                 ResponseCode response_code)
    : ClientResponse(endpoint, MessageType::EXECUTE,
                     InterfaceType::DEVICE_MANAGMENT,
                     unordered_set<ResponseCode>{
                         ResponseCode::CHANGED, ResponseCode::BAD_REQUEST,
                         ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
                         ResponseCode::METHOD_NOT_ALLOWED},
                     response_code) {
  checkResponseCode(response_code);
}

string ExecuteResponse::name() { return "ExecuteResponse"; }

} // namespace LwM2M
