#ifndef __LWM2M_MODEL_OBJECT_HPP
#define __LWM2M_MODEL_OBJECT_HPP

#include "Nonempty_Pointer/NonemptyPtr.hpp"

#include "Endpoint.hpp"
#include "ObjectDescriptor.hpp"
#include "ObjectInstance.hpp"
#include "RequesterInterfaceFacade.hpp"
#include "Resource.hpp"

#include <memory>
#include <unordered_map>
#include <unordered_set>

struct ObjectInstanceIDHasher {
  std::size_t operator()(const LwM2M::ElementID& id) const {
    using std::hash;
    using std::size_t;

    return hash<uint16_t>{}(id.getObjectInstanceID());
  }
};

struct ObjectInstanceComparator {
  bool operator()(
      const LwM2M::ElementID& lhs, const LwM2M::ElementID& rhs) const {
    return (lhs.getObjectInstanceID() == rhs.getObjectInstanceID());
  }
};

namespace LwM2M {
/**
 * @addtogroup Device_Model
 * @{
 */

using RequiredObjectInstances = std::unordered_multiset<ElementID,
    ObjectInstanceIDHasher, ObjectInstanceComparator>;

using ObjectInstances = std::unordered_map<uint16_t, ObjectInstancePtr>;

struct ObjectInstanceDoesNotExist : public std::runtime_error {
  ObjectInstanceDoesNotExist(ElementID id)
      : runtime_error("Object instance " + id.toString() + " does not exist") {}
};

/**
 * @brief Models a collection of data points and their groupings, all described
 * by a same object descriptor
 *
 */
class Object {
  RequesterInterfaceFacadePtr requester_;
  EndpointPtr endpoint_;
  ObjectInstances instances_;
  ObjectDescriptorPtr descriptor_;

public:
  Object(Observable::ExceptionHandler handler,
      RequesterInterfaceFacadePtr requester, EndpointPtr endpoint,
      RequiredObjectInstances instances, ObjectDescriptorPtr descriptor);

  /**
   * @brief Returns static information of this object.
   *
   * Used to display the linked object instances and set resource descriptors.
   *
   * @return ObjectDescriptorPtr
   */
  ObjectDescriptorPtr getDescriptor();

  /**
   * @brief Looks for a given Object instance based on a given ELementID
   *
   * @param id - id of the requested ObjectInstance
   *
   * @throws ObjectInstanceDoesNotExist if given ElementID does not address a
   * contained Object Instance
   *
   * @return ObjectInstancePtr
   */
  ObjectInstancePtr getObjectInstance(ElementID id);

  /**
   * @brief Returns all linked Object instances of this Object
   *
   * @return ObjectInstances
   */
  ObjectInstances getObjectInstances();

  /**
   * @brief Looks for a given Resource based on a given ElementID
   *
   * @param id - id of the requested Resource.
   *
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
   * @throws ObjectInstanceDoesNotExist if given ElementID does not address a
   * contained Object Instance
   * @throws ResourceDoesNotExist if given ElementID does not address a
   * contained Resource
   *
   * @return ResourceInstances
   */
  ResourceInstances getResourceInstances(ElementID id);
};

using ObjectSharedPtr = std::shared_ptr<Object>;
using ObjectPtr = NonemptyPointer::NonemptyPtr<ObjectSharedPtr>;

/** @}*/
} // namespace LwM2M

#endif // __LWM2M_MODEL_OBJECT_HPP