#include "WriteAttributes.hpp"

using namespace std;

namespace LwM2M {

vector<TargetAttribute>
makeTargetAtributesVector(ObjectIDs targets, NotifyAttributePtr attribute) {
  vector<TargetAttribute> result;
  for (auto target : targets) {
    auto target_attribute = make_pair(target, attribute);
    result.push_back(target_attribute);
  }
  return result;
}

WriteAttributesRequest::WriteAttributesRequest(EndpointPtr endpoint)
    : ServerRequest(endpoint, MessageType::WRITE_ATTRIBUTES,
                    InterfaceType::DEVICE_MANAGEMENT) {}

WriteAttributesRequest::WriteAttributesRequest(EndpointPtr endpoint,
                                               vector<TargetAttribute> content)
    : ServerRequest(endpoint, MessageType::WRITE_ATTRIBUTES,
                    InterfaceType::DEVICE_MANAGEMENT,
                    make_shared<Payload>(content)) {}

WriteAttributesRequest::WriteAttributesRequest(EndpointPtr endpoint,
                                               ObjectIDs targets,
                                               NotifyAttributePtr attribute)
    : WriteAttributesRequest(endpoint,
                             makeTargetAtributesVector(targets, attribute)) {}

string WriteAttributesRequest::name() { return "WriteAttributesRequest"; }
} // namespace LwM2M
