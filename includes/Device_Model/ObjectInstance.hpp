#ifndef __LWM2M_MODEL_OBJECT_INSTANCE_HPP
#define __LWM2M_MODEL_OBJECT_INSTANCE_HPP

#include "DataFormat.hpp"
#include "ElementID.hpp"
#include "Endpoint.hpp"
#include "Requester.hpp"
#include "Resource.hpp"

#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

namespace LwM2M {
using ResourceVariant =
    std::variant<ResourcePtr<bool>, ResourcePtr<int64_t>, ResourcePtr<double>,
                 ResourcePtr<std::string>, ResourcePtr<uint64_t>,
                 ResourcePtr<TimeStamp>, ResourcePtr<ObjectLink>,
                 ResourcePtr<std::vector<uint8_t>>>;
using Resources = std::unordered_map<uint32_t, ResourceVariant>;

struct ResourceDoesNotExist : public std::runtime_error {
  ResourceDoesNotExist(ResourceID id)
      : runtime_error("Resource " + id.toString() + " does not exist.") {}
};

class ObjectInstance {
  RequesterPtr requester_;
  EndpointPtr endpoint_;
  ObjectInstanceID id_;
  Resources resources_;

public:
  ObjectInstance(
      RequesterPtr requester, EndpointPtr endpoint, ObjectInstanceID id,
      std::unordered_map<uint32_t, std::shared_ptr<ResourceDescriptor>>
          resource_descriptors);

  ResourceVariant getResource(uint32_t id);
  Resources getResources();
};

using ObjectInstacePtr = std::shared_ptr<ObjectInstance>;
} // namespace LwM2M

#endif //__LWM2M_MODEL_OBJECT_INSTANCE_HPP