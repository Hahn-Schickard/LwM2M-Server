#ifndef __LWM2M_MODEL_RESOURCE_HPP
#define __LWM2M_MODEL_RESOURCE_HPP

#include "Executable.hpp"
#include "Operationless.hpp"
#include "ReadAndWritable.hpp"
#include "Readable.hpp"
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

using ResourceVariant = std::variant<ReadablePtr, WritablePtr,
    ReadAndWritablePtr, ExecutablePtr, OperationlessPtr>;
using ResourceInstances = std::unordered_map<ElementID, ResourceVariant>;

class Resource {
  ResourceDescriptorPtr descriptor_;
  ElementID id_;
  ResourceInstances instances_;

public:
  Resource() = default;
  Resource(Observable::ExceptionHandler handler, RequesterPtr requester,
      EndpointPtr endpoint, ResourceDescriptorPtr descriptor, ElementID id,
      std::optional<uint16_t> instance_id = std::nullopt);
  virtual ~Resource() = default;

  ResourceDescriptorPtr getDescriptor();
  ResourceVariant getInstance(bool ignore_multiple_instances = false);
  ResourceVariant getInstance(ElementID id);
  ResourceInstances getInstances();
};

using ResourcePtr = std::shared_ptr<Resource>;
} // namespace LwM2M

#endif //__LWM2M_MODEL_RESOURCE_HPP