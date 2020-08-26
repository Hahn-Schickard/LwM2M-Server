#include "LwM2M_Object.hpp"

using namespace std;

namespace LwM2M {

Object::Object(std::shared_ptr<Endpoint> endpoint, vector<uint32_t> instances,
               shared_ptr<ResponseHandler> response_handler,
               ObjectDescriptorPtr descriptor)
    : endpoint_(endpoint), descriptor_(descriptor) {
  for (auto instance : instances) {
    instances_.emplace(instance,
                       make_shared<ObjectInstance>(endpoint, descriptor->id_,
                                                   instance, response_handler,
                                                   descriptor_->resources_));
  }
}

ObjectDescriptorPtr Object::getDescriptor() { return descriptor_; }
} // namespace LwM2M
