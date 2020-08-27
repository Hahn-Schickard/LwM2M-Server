#ifndef __LWM2M_MODEL_OBJECT_INSTANCE_HPP
#define __LWM2M_MODEL_OBJECT_INSTANCE_HPP

#include "LwM2M_Endpoint.hpp"
#include "LwM2M_ObjectLink.hpp"
#include "LwM2M_Resource.hpp"
#include "Response_Handler.hpp"

#include <memory>
#include <variant>
#include <vector>

namespace LwM2M {
using ResourceVariant = std::variant<
    std::shared_ptr<Resource<bool>>, std::shared_ptr<Resource<int64_t>>,
    std::shared_ptr<Resource<double>>, std::shared_ptr<Resource<std::string>>,
    std::shared_ptr<Resource<uint64_t>>, std::shared_ptr<Resource<ObjectLink>>,
    std::shared_ptr<Resource<std::vector<uint8_t>>>>;

class ObjectInstance {
  std::shared_ptr<Endpoint> endpoint_;
  uint32_t parent_id_;
  uint32_t instance_id_;
  std::shared_ptr<ResponseHandler> response_handler_;
  std::unordered_map<uint32_t, ResourceVariant> resources_;

public:
  ObjectInstance(
      std::shared_ptr<Endpoint> endpoint, uint32_t parent_id_,
      uint32_t instance_id, std::shared_ptr<ResponseHandler> response_handler,
      std::unordered_map<uint32_t, std::shared_ptr<ResourceDescriptor>>
          resource_descriptors);

  ResourceVariant getResource(uint32_t id);
};
} // namespace LwM2M

#endif //__LWM2M_MODEL_OBJECT_INSTANCE_HPP