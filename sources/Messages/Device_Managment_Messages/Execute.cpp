#include "Execute.hpp"

using namespace std;

namespace LwM2M {
ExecuteRequest::ExecuteRequest(EndpointPtr endpoint, ResourceID target)
    : DeviceManagmentRequest(endpoint, MessageType::EXECUTE), target_(target) {}

ExecuteRequest::ExecuteRequest(EndpointPtr endpoint, ResourceID target,
                               vector<DataFormatPtr> arguments)
    : DeviceManagmentRequest(endpoint, MessageType::EXECUTE), target_(target),
      arguments_(arguments) {}

string ExecuteRequest::name() { return "WriteRequest"; }

ExecuteResponse::ExecuteResponse(EndpointPtr endpoint,
                                 ResponseCode response_code)
    : DeviceManagmentResponse(
          endpoint, MessageType::EXECUTE,
          unordered_set<ResponseCode>{
              ResponseCode::CHANGED, ResponseCode::BAD_REQUEST,
              ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
              ResponseCode::METHOD_NOT_ALLOWED},
          response_code) {
  checkResponseCode(response_code);
}

string ExecuteResponse::name() { return "ExecuteResponse"; }

} // namespace LwM2M
