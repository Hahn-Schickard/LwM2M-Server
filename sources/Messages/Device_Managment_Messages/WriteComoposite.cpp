#include "WriteComoposite.hpp"

using namespace std;

namespace LwM2M {

WriteComopositeRequest::WriteComopositeRequest(EndpointPtr endpoint,
                                               vector<TargetContent> content)
    : ServerRequest(endpoint, MessageType::WRITE_COMPOSITE,
                    InterfaceType::DEVICE_MANAGMENT),
      content_(content) {}

void WriteComopositeRequest::append(EelmentIdVariant target,
                                    DataFormatPtr content) {
  content_.emplace_back(target, content);
}

void WriteComopositeRequest::append(vector<EelmentIdVariant> targets,
                                    DataFormatPtr content) {
  for (auto target : targets) {
    append(target, content);
  }
}

// void append(vector<TargetContent> content) {
//   content_.insert(content_.end(), content.begin(), content.end());
// }

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
