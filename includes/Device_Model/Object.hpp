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

using ObjectInstances = std::unordered_map<ObjectInstanceID, ObjectInstacePtr>;

struct ObjectInstanceDoesNotExist : public std::runtime_error {
  ObjectInstanceDoesNotExist(ObjectInstanceID id)
      : runtime_error("Object instance " + id.toString() + " does not exist") {}
};

class Object {
  RequesterPtr requester_;
  EndpointPtr endpoint_;
  ObjectInstances instances_;
  ObjectDescriptorPtr descriptor_;

public:
  Object(RequesterPtr requester, EndpointPtr endpoint,
         ObjectID::ObjectInstanceIDs instances, ObjectDescriptorPtr descriptor);

  ObjectDescriptorPtr getDescriptor();
  ObjectInstances getInstances();

  ResourceVariant getResource(ObjectInstanceID instance, ResourceID resource);
};

using ObjectPtr = std::shared_ptr<Object>;
} // namespace LwM2M

#endif // __LWM2M_MODEL_OBJECT_HPP