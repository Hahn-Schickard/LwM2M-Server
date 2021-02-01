#include "WriteAttributes.hpp"

using namespace std;

namespace LwM2M {

WriteAttributesRequest::WriteAttributesRequest(EndpointPtr endpoint)
    : ServerRequest(endpoint, MessageType::WRITE_ATTRIBUTES,
                    InterfaceType::DEVICE_MANAGMENT) {}

WriteAttributesRequest::WriteAttributesRequest(EndpointPtr endpoint,
                                               vector<TargetAttribute> content)
    : ServerRequest(endpoint, MessageType::WRITE_ATTRIBUTES,
                    InterfaceType::DEVICE_MANAGMENT),
      content_(content) {}

WriteAttributesRequest::WriteAttributesRequest(EndpointPtr endpoint,
                                               vector<EelmentIdVariant> targets,
                                               NotifyAttributePtr attribute)
    : WriteAttributesRequest(endpoint) {
  append(targets, attribute);
}

void WriteAttributesRequest::append(EelmentIdVariant target,
                                    NotifyAttributePtr attribute) {
  content_.emplace_back(target, attribute);
}

void WriteAttributesRequest::append(vector<EelmentIdVariant> targets,
                                    NotifyAttributePtr attribute) {
  for (auto target : targets) {
    append(target, attribute);
  }
}

// void append(vector<TargetAttribute> content) {
//   content_.reserve(content_.size() +
//                    distance(content.begin(), content.end()));
//   content_.insert(content_.end(), content.begin(), content.end());
// }

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
