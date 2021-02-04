#ifndef __LWM2M_MODEL_OBJECT_HPP
#define __LWM2M_MODEL_OBJECT_HPP

#include "Endpoint.hpp"
#include "ObjectDescriptor.hpp"
#include "ObjectInstance.hpp"
#include "Resource.hpp"

#include <memory>
#include <vector>

namespace LwM2M {

using ObjectDescriptorPtr = std::shared_ptr<ObjectDescriptor>;
using ObjectInstacePtr = std::shared_ptr<ObjectInstance>;
using ObjectInstances = std::unordered_map<uint32_t, ObjectInstacePtr>;

class Object {
  EndpointPtr endpoint_;
  ObjectInstances instances_;
  ObjectDescriptorPtr descriptor_;

public:
  Object(EndpointPtr endpoint, std::vector<uint32_t> instances,
         ObjectDescriptorPtr descriptor);

  ObjectDescriptorPtr getDescriptor();
  ObjectInstances getInstances();

  ResourceVariant getResource(uint32_t instance_id, uint32_t resource_id);
};
} // namespace LwM2M

#endif // __LWM2M_MODEL_OBJECT_HPP