#ifndef __LWM2M_MODEL_OBJECT_HPP
#define __LWM2M_MODEL_OBJECT_HPP

#include "LwM2M_Endpoint.hpp"
#include "LwM2M_ObjectDescriptor.hpp"
#include "LwM2M_Resource.hpp"

#include <memory>
#include <variant>
#include <vector>

namespace LwM2M {
using ResourceVariant = std::variant<
    std::shared_ptr<Resource<bool>>, std::shared_ptr<Resource<int64_t>>,
    std::shared_ptr<Resource<double>>, std::shared_ptr<Resource<std::string>>,
    std::shared_ptr<Resource<uint64_t>>, std::shared_ptr<Resource<ObjectLink>>,
    std::shared_ptr<Resource<std::vector<uint8_t>>>>;

using ObjectDescriptorPtr = std::shared_ptr<ObjectDescriptor>;

class Object {
  std::shared_ptr<Endpoint> endpoint_;
  ObjectDescriptorPtr descriptor_;
  std::unordered_map<uint32_t, ResourceVariant> resources_;

public:
  Object(std::shared_ptr<Endpoint> endpoint, ObjectDescriptorPtr descriptor);

  ObjectDescriptorPtr getDescriptor();

  template <typename T> std::shared_ptr<Resource<T>> getResource(uint32_t id) {
    auto it = resources_.find(id);
    if (it != resources_.end()) {
      return it->second;
    } else {
      return std::shared_ptr<Resource<T>>();
    }
  }
};
} // namespace LwM2M

#endif // __LWM2M_MODEL_OBJECT_HPP