#ifndef __LWM2M_MODEL_RESOURCE_HPP
#define __LWM2M_MODEL_RESOURCE_HPP

#include "Executable.hpp"
#include "Operationless.hpp"
#include "ReadAndWritable.hpp"
#include "Readable.hpp"
#include "RequesterInterfaceFacade.hpp"
#include "Writable.hpp"

namespace LwM2M {
struct ResourceInstanceDoesNotExist : public std::runtime_error {
  ResourceInstanceDoesNotExist(ElementID id)
      : runtime_error(
            "Resource instance " + id.toString() + " does not exist") {}
};

struct ResourceInstanceCouldNotBeResolved : public std::runtime_error {
  ResourceInstanceCouldNotBeResolved(ElementID id)
      : runtime_error("Resource instance for resource " + id.toString() +
            " could not be automatically resolved, since resource contains "
            "multiple instances") {}
};

using ResourceInstance = std::variant<ReadablePtr, WritablePtr,
    ReadAndWritablePtr, ExecutablePtr, OperationlessPtr>;
using ResourceInstances = std::unordered_map<ElementID, ResourceInstance>;

class Resource {
  ResourceDescriptorPtr descriptor_;
  ElementID id_;
  ResourceInstances instances_;

public:
  Resource() = default;
  Resource(Observable::ExceptionHandler handler,
      RequesterInterfaceFacadePtr requester, EndpointPtr endpoint,
      ResourceDescriptorPtr descriptor, ElementID id,
      std::optional<uint16_t> instance_id = std::nullopt);
  virtual ~Resource() = default;

  ResourceDescriptorPtr getDescriptor();
  ResourceInstance getResourceInstance(bool ignore_multiple_instances = false);
  ResourceInstance getResourceInstance(ElementID id);
  ResourceInstances getResourceInstances();
};

using ResourcePtr = std::shared_ptr<Resource>;
} // namespace LwM2M

#endif //__LWM2M_MODEL_RESOURCE_HPP