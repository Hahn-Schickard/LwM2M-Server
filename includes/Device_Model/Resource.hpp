#ifndef __LWM2M_MODEL_RESOURCE_HPP
#define __LWM2M_MODEL_RESOURCE_HPP

#include "Nonempty/Pointer.hpp"

#include "Executable.hpp"
#include "Operationless.hpp"
#include "ReadAndWritable.hpp"
#include "Readable.hpp"
#include "RequesterInterfaceFacade.hpp"
#include "Writable.hpp"

namespace LwM2M {
/**
 * @addtogroup Device_Model
 * @{
 */

struct ResourceInstanceDoesNotExist : public std::runtime_error {
  ResourceInstanceDoesNotExist(const ElementID& id)
      : runtime_error(
            "Resource instance " + id.toString() + " does not exist") {}
};

struct ResourceInstanceCouldNotBeResolved : public std::runtime_error {
  ResourceInstanceCouldNotBeResolved(const ElementID& id)
      : runtime_error("Resource instance for resource " + id.toString() +
            " could not be automatically resolved, since resource contains "
            "multiple instances") {}
};

/**
 * @brief A heterogeneous container of various resource traits.
 *
 * To access the stored data from ResourceInstance variant; Use:
 * @code {.cpp}
 * 1. std::get<T>(&variant) - throws std::bad_variant_access on type mismatch
 * 2. std::get_if<T>(&variant) - returns False on type mismatch
 * 3. std::visit([](auto && value){ //use value here}, &variant)
 * 4. match(
 *     [](ReadablePtr& value){//use to access ReadablePtr},
 *     [](WritablePtr& value){//use to access WritablePtr},
 *     [](ReadAndWritablePtr& value){//use to access ReadAndWritablePtr},
 *     [](ExecutablePtr& value){//use to access ExecutablePtr},
 *     [](OperationlessPtr& value){//use to access OperationlessPtr},
 *     [](auto& value){//use if value type is not important, but the value is},
 *     [](...){//Fallback case. Similar to default keyword of switch statement},
 *     &variant) - declared in VariantVisitor project
 * @endcode
 *
 * @warning It is recommended to parse pointer references, instead of pointer
 * values to both variant visitor (shown in 3.) and variant matcher (shown
 * in 4.) to improve performance and avoid compilation issues (Type deduction of
 * given NonemptyPtr instances can fail to be resolved and lead to compiler
 * failing with template argument deduction/substitution errors)
 */
using ResourceInstance = std::variant<ReadablePtr, WritablePtr,
    ReadAndWritablePtr, ExecutablePtr, OperationlessPtr>;
using ResourceInstances = std::unordered_map<ElementID, ResourceInstance>;

/**
 * @brief Models a single data point within the Device Model
 *
 */
class Resource {
  ResourceDescriptorPtr descriptor_;
  ElementID id_;
  ResourceInstances instances_;

public:
  /**
   * @brief Default constructor, used for testing purposes only
   *
   */
  Resource() = default;

  Resource(const Observable::ExceptionHandler& handler,
      const RequesterInterfaceFacadePtr& requester, const EndpointPtr& endpoint,
      const ResourceDescriptorPtr& descriptor, const ElementID& id,
      const std::optional<uint16_t>& instance_id = std::nullopt);

  virtual ~Resource() = default;

  /**
   * @brief Returns static information of this resource.
   * Used to display the resource.
   *
   * @return ResourceDescriptorPtr
   */
  ResourceDescriptorPtr getDescriptor() const;

  /**
   * @brief Returns dynamic information of this resource.
   * Used to set/check current resource state.
   *
   * @param ignore_multiple_instances - if set to True, this method returns the
   * first instance of ResourceInstances field instead of throwing
   * ResourceInstanceCouldNotBeResolved
   *
   * @throws ResourceInstanceCouldNotBeResolved if ignore_multiple_instances was
   * set to False and current resource has more than 1 ResourceInstance within
   * ResourceInstances field
   *
   * @return ResourceInstance - @see ResourceInstance on how to obtain stored
   * information
   */
  ResourceInstance getResourceInstance(
      bool ignore_multiple_instances = false) const;

  /**
   * @brief Looks for a given ResourceInstance based on a given ElementID
   *
   * @param id - id of the requested ResourceInstance. If the current resource
   * does not support multiple resource instances, Resource ID will be used
   * instead of a Resource instance ID
   *
   * @throws ResourceInstanceDoesNotExist if given ElementID could not be found
   * within current ResourceInstances field
   *
   * @return ResourceInstance - @see ResourceInstance on how to obtain stored
   * information
   */
  ResourceInstance getResourceInstance(const ElementID& id) const;

  /**
   * @brief Returns all ResourceInstances linked to this Resource
   *
   * @return ResourceInstances
   */
  ResourceInstances getResourceInstances() const;
};

using ResourceSharedPtr = std::shared_ptr<Resource>;
using ResourcePtr = Nonempty::Pointer<ResourceSharedPtr>;

/** @}*/
} // namespace LwM2M

#endif //__LWM2M_MODEL_RESOURCE_HPP