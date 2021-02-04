#ifndef __LWM2M_MODEL_OBJECT_INSTANCE_HPP
#define __LWM2M_MODEL_OBJECT_INSTANCE_HPP

#include "DataFormat.hpp"
#include "Endpoint.hpp"
#include "Resource.hpp"

#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

namespace LwM2M {
using ResourceVariant = std::variant<
    std::shared_ptr<Resource<bool>>, std::shared_ptr<Resource<int64_t>>,
    std::shared_ptr<Resource<double>>, std::shared_ptr<Resource<std::string>>,
    std::shared_ptr<Resource<uint64_t>>, std::shared_ptr<Resource<ObjectLink>>,
    std::shared_ptr<Resource<std::vector<uint8_t>>>>;
using Resources = std::unordered_map<uint32_t, ResourceVariant>;

class ObjectInstance {
  EndpointPtr endpoint_;
  uint32_t parent_id_;
  uint32_t instance_id_;
  Resources resources_;

public:
  ObjectInstance(
      EndpointPtr endpoint, uint32_t parent_id_, uint32_t instance_id,
      std::unordered_map<uint32_t, std::shared_ptr<ResourceDescriptor>>
          resource_descriptors);

  ResourceVariant getResource(uint32_t id);
  Resources getResources();
};
} // namespace LwM2M

#endif //__LWM2M_MODEL_OBJECT_INSTANCE_HPP