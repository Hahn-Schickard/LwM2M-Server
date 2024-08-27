#ifndef __LWM2M_DEVICE_HPP
#define __LWM2M_DEVICE_HPP

#include "Nonempty/Pointer.hpp"

#include "Endpoint.hpp"
#include "ModelType.hpp"
#include "Object.hpp"
#include "ObjectDescriptor.hpp"
#include "RequesterInterfaceFacade.hpp"

#include <string>
#include <unordered_map>

struct ObjectIDHasher {
  std::size_t operator()(const LwM2M::ElementID& id) const {
    using std::hash;
    using std::size_t;

    return hash<uint16_t>{}(id.getObjectID());
  }
};

struct ObjectIDComparator {
  bool operator()(
      const LwM2M::ElementID& lhs, const LwM2M::ElementID& rhs) const {
    return (lhs.getObjectID() == rhs.getObjectID());
  }
};

namespace LwM2M {
/**
 * @defgroup Device_Model Device Model
 * Contains All of the modeling elements for the LwM2M Resource Model
 * implementation
 * @addtogroup Device_Model
 * @{
 */

using ObjectsMap = std::unordered_map<uint16_t, ObjectPtr>;
using ObjectDescriptorsMap = std::unordered_multimap<ElementID,
    ObjectDescriptorPtr, ObjectIDHasher, ObjectIDComparator>;

struct ObjectDoesNotExist : public std::runtime_error {
  ObjectDoesNotExist(ElementID id)
      : runtime_error("Object " + id.toString() + " does not exits") {}
};

/**
 * @brief Models LwM2M Client instance
 *
 */
class Device {
  Observable::ExceptionHandler exception_handler_;
  RequesterInterfaceFacadePtr requester_;
  EndpointPtr endpoint_;
  ObjectsMap object_instances_;
  std::string device_id_;
  size_t life_time_;
  std::string name_;
  LwM2M_Version version_;
  BindingType binding_;
  bool queue_mode_;

  void makeObjects(const ObjectDescriptorsMap& object_descriptors_map);

public:
  /**
   * @brief Creates a default LwM2M Device, used for testing only.
   *
   */
  Device() = default;
  Device(Observable::ExceptionHandler handler,
      RequesterInterfaceFacadePtr requester_, EndpointPtr endpoint,
      ObjectDescriptorsMap object_descriptors_map, std::string device_id,
      size_t life_time, std::string name = std::string(),
      LwM2M_Version version = LwM2M_Version::V1_0,
      BindingType binding = BindingType::UDP, bool queue_mode = false);

  /**
   * @brief Returns the assigned LwM2M Client ID
   *
   * @return std::string
   */
  std::string getDeviceId();

  /**
   * @brief Returns the name of LwM2M Client
   *
   * @return std::string
   */
  std::string getName();

  /**
   * @brief Looks for a given Object based on a given ObjectID
   *
   * @param id - id of the requested Object
   *
   * @throws ObjectDoesNotExist if given ElementID does not address a contained
   * Object
   *
   * @return ObjectPtr
   */
  ObjectPtr getObject(ElementID id);

  /**
   * @brief Returns all linked objects of this Device
   *
   * @return ObjectsMap
   */
  ObjectsMap getObjects();

  /**
   * @brief Looks for a given ObjectInstance based on a given ELementID
   *
   * @param id - id of the requested ObjectInstance
   *
   * @throws ObjectDoesNotExist if given ElementID does not address a contained
   * Object
   * @throws ObjectInstanceDoesNotExist if given ElementID does not address a
   * contained Object Instance
   *
   * @return ObjectInstancePtr
   */
  ObjectInstancePtr getObjectInstance(ElementID id);

  /**
   * @brief Returns all Object instances linked to the given Object
   *
   * @param id - id of the requested Object
   *
   * @throws ObjectDoesNotExist if given ElementID does not address a contained
   * Object
   *
   * @return Resources
   */
  ObjectInstances getObjectInstances(ElementID id);

  /**
   * @brief Looks for a given Resource based on a given ElementID
   *
   * @param id - id of the requested Resource.
   *
   * @throws ObjectDoesNotExist if given ElementID does not address a contained
   * Object
   * @throws ObjectInstanceDoesNotExist if given ElementID does not address a
   * contained Object Instance
   * @throws ResourceDoesNotExist if given ElementID does not address a
   * contained Resource
   *
   * @return ResourcePtr
   */
  ResourcePtr getResource(ElementID id);

  /**
   * @brief Returns all Resources linked to the given Object Instance
   *
   * @param id - id of the requested ObjectInstance
   *
   * @throws ObjectDoesNotExist if given ElementID does not address a contained
   * Object
   * @throws ObjectInstanceDoesNotExist if given ElementID does not address a
   * contained Object Instance
   *
   * @return Resources
   */
  Resources getResources(ElementID id);

  /**
   * @brief Looks for a given ResourceInstance based on a given ElementID
   *
   * @param id - id of the requested ResourceInstance. If the current resource
   * does not support multiple resource instances, Resource ID will be used
   * instead of a Resource instance ID
   *
   * @throws ObjectDoesNotExist if given ElementID does not address a contained
   * Object
   * @throws ObjectInstanceDoesNotExist if given ElementID does not address a
   * contained Object Instance
   * @throws ResourceDoesNotExist if given ElementID does not address a
   * contained Resource
   * @throws ResourceInstanceDoesNotExist if given ElementID could not be found
   * within current ResourceInstances field
   *
   * @return ResourceInstance - @see ResourceInstance on how to obtain stored
   * information
   */
  ResourceInstance getResourceInstance(ElementID id);

  /**
   * @brief  Returns all ResourceInstances linked to given Resource
   *
   * @param id - id of a given Resource, from which ResourceInstances should be
   * obtained
   *
   * @throws ObjectDoesNotExist if given ElementID does not address a contained
   * Object
   * @throws ObjectInstanceDoesNotExist if given ElementID does not address a
   * contained Object Instance
   * @throws ResourceDoesNotExist if given ElementID does not address a
   * contained Resource
   *
   * @return ResourceInstances
   */
  ResourceInstances getResourceInstances(ElementID id);

  /**
   * @brief Used by the managing binding to change the Client BindingType
   *
   * @param binding
   */
  void updateBinding(BindingType binding);

  /**
   * @brief Used by the managing binding to change the Client lifetime
   *
   * @param life_time
   */
  void updateLifetime(size_t life_time);

  /**
   * @brief Used by the managing binding to change the assigned object
   * descriptors
   *
   * @param object_instances
   */
  void updateObjectsMap(const ObjectDescriptorsMap& object_instances);
};

using DeviceSharedPtr = std::shared_ptr<Device>;
using DevicePtr = Nonempty::Pointer<DeviceSharedPtr>;

/** @}*/
}; // namespace LwM2M

#endif //__LWM2M_DEVICE_HPP