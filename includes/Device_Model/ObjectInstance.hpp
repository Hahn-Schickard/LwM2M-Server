#ifndef __LWM2M_MODEL_OBJECT_INSTANCE_HPP
#define __LWM2M_MODEL_OBJECT_INSTANCE_HPP

#include "DataFormat.hpp"
#include "ElementID.hpp"
#include "Endpoint.hpp"
#include "Observable.hpp"
#include "Requester.hpp"
#include "Resource.hpp"

#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

namespace LwM2M {
using Resources = std::unordered_map<uint32_t, ResourcePtr>;

struct ResourceDoesNotExist : public std::runtime_error {
  ResourceDoesNotExist(ElementID id)
      : runtime_error("Resource " + id.toString() + " does not exist.") {}
};

class ObjectInstance {
  RequesterPtr requester_;
  EndpointPtr endpoint_;
  ElementID id_;
  Resources resources_;

public:
  ObjectInstance(Observable::ExceptionHandler handler, RequesterPtr requester,
      EndpointPtr endpoint, ElementID id,
      std::unordered_map<uint32_t, std::shared_ptr<ResourceDescriptor>>
          resource_descriptors);
  ElementID getId();

  ResourcePtr getResource(ElementID id);
  Resources getResources();
  ResourceInstance getResourceInstance(ElementID id);
  ResourceInstances getResourceInstances(ElementID id);
};

using ObjectInstacePtr = std::shared_ptr<ObjectInstance>;
} // namespace LwM2M

#endif //__LWM2M_MODEL_OBJECT_INSTANCE_HPP