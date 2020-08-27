#ifndef __LWM2M_MODEL_OBJECT_HPP
#define __LWM2M_MODEL_OBJECT_HPP

#include "LwM2M_Endpoint.hpp"
#include "LwM2M_ObjectDescriptor.hpp"
#include "LwM2M_ObjectInstance.hpp"
#include "LwM2M_Resource.hpp"
#include "Response_Handler.hpp"

#include <memory>
#include <vector>

namespace LwM2M {

using ObjectDescriptorPtr = std::shared_ptr<ObjectDescriptor>;
using ObjectInstacePtr = std::shared_ptr<ObjectInstance>;

class Object {
  std::shared_ptr<Endpoint> endpoint_;
  std::unordered_map<uint32_t, ObjectInstacePtr> instances_;
  ObjectDescriptorPtr descriptor_;

public:
  Object(std::shared_ptr<Endpoint> endpoint, std::vector<uint32_t> instances,
         std::shared_ptr<ResponseHandler> response_handler,
         ObjectDescriptorPtr descriptor);

  ObjectDescriptorPtr getDescriptor();

  ResourceVariant getResource(uint32_t instance_id, uint32_t resource_id);
};
} // namespace LwM2M

#endif // __LWM2M_MODEL_OBJECT_HPP