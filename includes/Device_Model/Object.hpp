#ifndef __LWM2M_MODEL_OBJECT_HPP
#define __LWM2M_MODEL_OBJECT_HPP

#include "Endpoint.hpp"
#include "ObjectDescriptor.hpp"
#include "ObjectInstance.hpp"
#include "Requester.hpp"
#include "Resource.hpp"

#include <memory>
#include <unordered_map>
#include <unordered_set>

struct ObjectInstanceIDHasher {
  std::size_t operator()(const LwM2M::ElementID &id) const {
    using std::hash;
    using std::size_t;

    return hash<uint16_t>{}(id.getObjectInstanceID());
  }
};

struct ObjectInstanceComparator {
  bool operator()(const LwM2M::ElementID &lhs,
                  const LwM2M::ElementID &rhs) const {
    return (lhs.getObjectInstanceID() == rhs.getObjectInstanceID());
  }
};

namespace LwM2M {

using RequiredObjectInstances =
    std::unordered_multiset<ElementID, ObjectInstanceIDHasher,
                            ObjectInstanceComparator>;

using ObjectInstances = std::unordered_map<uint16_t, ObjectInstacePtr>;

struct ObjectInstanceDoesNotExist : public std::runtime_error {
  ObjectInstanceDoesNotExist(ElementID id)
      : runtime_error("Object instance " + id.toString() + " does not exist") {}
};

class Object {
  RequesterPtr requester_;
  EndpointPtr endpoint_;
  ObjectInstances instances_;
  ObjectDescriptorPtr descriptor_;

public:
  Object(Observable::ExceptionHandler handler, RequesterPtr requester,
         EndpointPtr endpoint, RequiredObjectInstances instances,
         ObjectDescriptorPtr descriptor);

  ObjectDescriptorPtr getDescriptor();
  ObjectInstances getInstances();

  ResourceVariant getResource(ElementID id);
};

using ObjectPtr = std::shared_ptr<Object>;
} // namespace LwM2M

#endif // __LWM2M_MODEL_OBJECT_HPP