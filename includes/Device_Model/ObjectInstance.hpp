#ifndef __LWM2M_MODEL_OBJECT_INSTANCE_HPP
#define __LWM2M_MODEL_OBJECT_INSTANCE_HPP

#include "Nonempty_Pointer/NonemptyPtr.hpp"

#include "DataFormat.hpp"
#include "ElementID.hpp"
#include "Endpoint.hpp"
#include "Observable.hpp"
#include "RequesterInterfaceFacade.hpp"
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

/**
 * @brief Models a single instance of a linked Object
 *
 */
class ObjectInstance {
  RequesterInterfaceFacadePtr requester_;
  EndpointPtr endpoint_;
  ElementID id_;
  Resources resources_;

public:
  ObjectInstance(Observable::ExceptionHandler handler,
      RequesterInterfaceFacadePtr requester, EndpointPtr endpoint, ElementID id,
      std::unordered_map<uint32_t, ResourceDescriptorPtr> resource_descriptors);

  /**
   * @brief Returns the id of this object instance
   *
   * @return ElementID
   */
  ElementID getId();

  /**
   * @brief Looks for a given Resource based on a given ElementID
   *
   * @throws ResourceDoesNotExist if given ElementID does not address a
   * contained Resource
   *
   * @param id - id of the requested Resource.
   *
   * @return ResourcePtr
   */
  ResourcePtr getResource(ElementID id);

  /**
   * @brief Returns all Resources linked to the current Object Instance
   *
   * @return Resources
   */
  Resources getResources();

  /**
   * @brief Looks for a given ResourceInstance based on a given ElementID
   *
   * @throws ResourceDoesNotExist if given ElementID does not address a
   * contained Resource
   * @throws ResourceInstanceDoesNotExist if given ElementID could not be found
   * within current ResourceInstances field
   *
   * @param id - id of the requested ResourceInstance. If the current resource
   * does not support multiple resource instances, Resource ID will be used
   * instead of a Resource instance ID
   *
   * @return ResourceInstance - @see ResourceInstance on how to obtain stored
   * information
   */
  ResourceInstance getResourceInstance(ElementID id);

  /**
   * @brief  Returns all ResourceInstances linked to given Resource
   *
   * @throws ResourceDoesNotExist if given ElementID does not address a
   * contained Resource
   *
   * @param id - id of a given Resource, from which ResourceInstances should be
   * obtained
   *
   * @return ResourceInstances
   */
  ResourceInstances getResourceInstances(ElementID id);
};

using ObjectInstanceSharedPtr = std::shared_ptr<ObjectInstance>;
using ObjectInstancePtr = NonemptyPointer::NonemptyPtr<ObjectInstanceSharedPtr>;
} // namespace LwM2M

#endif //__LWM2M_MODEL_OBJECT_INSTANCE_HPP