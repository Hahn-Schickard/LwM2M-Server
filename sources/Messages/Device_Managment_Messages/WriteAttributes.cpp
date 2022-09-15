#include "WriteAttributes.hpp"

using namespace std;

namespace LwM2M {

vector<TargetAttribute> makeTargetAtributesVector(
    ElementIDs targets, NotifyAttributePtr attribute) { // NOLINT
  vector<TargetAttribute> result;
  for (auto target : targets) {
    auto target_attribute = make_pair(target, attribute);
    result.push_back(target_attribute);
  }
  return result;
}

WriteAttributesRequest::WriteAttributesRequest(EndpointPtr endpoint) // NOLINT
    : DeviceManagementRequest(endpoint, // NOLINT
          MessageType::WRITE_ATTRIBUTES) {}

WriteAttributesRequest::WriteAttributesRequest(EndpointPtr endpoint, // NOLINT
    vector<TargetAttribute> content) // NOLINT
    : DeviceManagementRequest(endpoint, // NOLINT
          MessageType::WRITE_ATTRIBUTES, make_shared<Payload>(content)) {}

WriteAttributesRequest::WriteAttributesRequest(EndpointPtr endpoint, // NOLINT
    ElementIDs targets, NotifyAttributePtr attribute) // NOLINT
    : WriteAttributesRequest(endpoint, // NOLINT
          makeTargetAtributesVector(targets, attribute)) {} // NOLINT

string WriteAttributesRequest::name() { return "WriteAttributesRequest"; }
} // namespace LwM2M
