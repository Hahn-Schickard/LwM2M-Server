#ifndef __LWM2M_MODEL_OBJECT_HPP
#define __LWM2M_MODEL_OBJECT_HPP

#include "Endpoint.hpp"
#include "ObjectDescriptor.hpp"
#include "ObjectInstance.hpp"
#include "Requester.hpp"
#include "Resource.hpp"

#include <memory>
#include <vector>

namespace LwM2M {

using ObjectInstances = std::unordered_map<uint32_t, ObjectInstacePtr>;

struct ObjectInstanceDoesNotExitst : public std::runtime_error {
  ObjectInstanceDoesNotExitst(ObjectInstanceID id)
      : runtime_error("Object instance " + id.toString() + " does not exist") {}
};

class Object {
  RequesterPtr requester_;
  EndpointPtr endpoint_;
  ObjectInstances instances_;
  ObjectDescriptorPtr descriptor_;

public:
  Object(RequesterPtr requester, EndpointPtr endpoint,
         std::vector<uint32_t> instances, ObjectDescriptorPtr descriptor);

  ObjectDescriptorPtr getDescriptor();
  ObjectInstances getInstances();

  ResourceVariant getResource(uint32_t instance_id, uint32_t resource_id);
};

using ObjectPtr = std::shared_ptr<Object>;
} // namespace LwM2M

#endif // __LWM2M_MODEL_OBJECT_HPP