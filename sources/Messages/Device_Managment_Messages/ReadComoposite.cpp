#include "ReadComoposite.hpp"

using namespace std;

namespace LwM2M {

ReadComopositeRequest::ReadComopositeRequest(EndpointPtr endpoint,
                                             vector<EelmentIdVariant> targets)
    : DeviceManagmentRequest(endpoint, MessageType::READ_COMPOSITE),
      targets_(targets) {}

string ReadComopositeRequest::name() { return "ReadComopositeRequest"; }

ReadComopositeResponse::ReadComopositeResponse(
    EndpointPtr endpoint, ResponseCode response_code,
    std::vector<TargetContent> content)
    : DeviceManagmentResponse(
          endpoint, MessageType::READ_COMPOSITE,
          unordered_set<ResponseCode>{
              ResponseCode::CONTENT, ResponseCode::BAD_REQUEST,
              ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
              ResponseCode::METHOD_NOT_ALLOWED, ResponseCode::NOT_ACCEPTABLE},
          response_code),
      content_(content) {
  checkResponseCode(response_code);
}

string ReadComopositeResponse::name() { return "ReadComopositeResponse"; }

} // namespace LwM2M
