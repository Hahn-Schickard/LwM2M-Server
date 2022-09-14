#ifndef __LWM2M_MODEL_OBJECT_DESCRIPTOR_HPP
#define __LWM2M_MODEL_OBJECT_DESCRIPTOR_HPP

#include "ResourceDescriptor.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

namespace LwM2M {

struct ResourceDescriptorDoesNotExist : public std::runtime_error {
  ResourceDescriptorDoesNotExist(uint32_t object_id, uint32_t resource_id)
      : runtime_error("Object with id " + std::to_string(object_id) +
            " does not have a Resource Descriptor with id " +
            std::to_string(resource_id)) {}
};

/**
 * @brief Contains static information of an Object or its instances and a list
 * or supported Resources.
 *
 * Used to create a representation of link object instances and their resources.
 *
 */
struct ObjectDescriptor {
  const std::string name_;
  const std::string description_;
  const uint32_t id_;
  const bool multiple_instances_;
  const bool mandatory_;
  const std::string urn_;
  const std::unordered_map<uint32_t, ResourceDescriptorPtr> resources_;

  /**
   * @brief Creates a default object descriptor. Used for testing only
   *
   */
  ObjectDescriptor();

  /**
   * @brief Copy constructor
   *
   * @param instance
   */
  ObjectDescriptor(const ObjectDescriptor& instance) = default;

  /**
   * @brief Move constructor
   *
   * @param instance
   */
  ObjectDescriptor(ObjectDescriptor&& instance) noexcept;

  /**
   * @brief Creates a valid Object descriptor
   *
   * @param name - object name
   * @param description - human readable description of the object
   * @param id - object id
   * @param multiple_instances - does this object support multiple instances of
   * itself?
   * @param mandatory - is this resource mandatory for a valid Device instance?
   * @param urn - OMA LwM2M Registry Uniform Resource Name
   * @param resources - an index list of supported Resources
   */
  ObjectDescriptor(std::string name, std::string description, uint32_t id,
      bool multiple_instances, bool mandatory, std::string urn,
      std::unordered_map<uint32_t, ResourceDescriptorPtr> resources);

  /**
   * @brief A helper to find a given ResourceDescriptorPtr from the contained
   * resources list
   *
   * @param id - index of the requested ResourceDescriptorPtr
   *
   * @throws ResourceDescriptorDoesNotExist if given id does not correspond to a
   * contained resource descriptor
   *
   *  @return ResourceDescriptorPtr
   */
  ResourceDescriptorPtr getResource(uint32_t id);
};

using ObjectDescriptorPtr = std::shared_ptr<ObjectDescriptor>;

} // namespace LwM2M

#endif //__LWM2M_MODEL_OBJECT_DESCRIPTOR_HPP