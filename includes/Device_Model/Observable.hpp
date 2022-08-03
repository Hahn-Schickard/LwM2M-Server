#ifndef __LWM2M_OBSERVABLE_HPP
#define __LWM2M_OBSERVABLE_HPP

#include "Event_Model/EventListenerInterface.hpp"
#include "Event_Model/EventSource.hpp"
#include "Nonempty_Pointer/NonemptyPtr.hpp"

#include "ElementAddress.hpp"
#include "ObservableInterface.hpp"

#include <map>

namespace LwM2M {
/**
 * @addtogroup Device_Model
 * @{
 */

struct ElementNotObserved : public std::runtime_error {
  ElementNotObserved(ElementID id)
      : runtime_error("Element " + id.toString() +
            " is not in the list of compoisite observed elements.") {}
};

/**
 * @brief Models the observation functionality of a resource
 *
 */
struct Observable : public Event_Model::EventSource<PayloadData>,
                    public ElementAddress {
  using ExceptionHandler = std::function<void(std::exception_ptr)>;
  using ObservedDataTypes = std::map<ElementID, DataType>;

  /**
   * @brief Creates an observable element of various sub-elements
   *
   * @param handler - exception handling mechanism
   * @param requester - observation interface to the related binding
   * @param endpoint - endpoint address of the modeled entity
   * @param id - element id of the modeled entity
   * @param data_types - a list of observed data types indexed by associated
   * ElementIDs
   */
  Observable(ExceptionHandler handler, ObservableInterfacePtr requester,
      EndpointPtr endpoint, ElementID id, ObservedDataTypes data_types);

  /**
   * @brief Creates an observable element for a given ResourceInstance
   *
   * @param handler - exception handling mechanism
   * @param requester - observation interface to the related binding
   * @param endpoint - endpoint address of the modeled entity
   * @param id - element id of the modeled entity
   * @param data_type - observed data type
   */
  Observable(ExceptionHandler handler, ObservableInterfacePtr requester,
      EndpointPtr endpoint, ElementID id, DataType data_type);

  ~Observable();

  /**
   * @brief Returns an index list of observable elements and their datatypes
   *
   * @return ObservedDataTypes
   */
  ObservedDataTypes getObservedDataTypes();

  /**
   * @brief Returns the DataType of a given Observable element
   *
   * @param id - id of the requested observable element
   *
   * @throws ElementNotObserved if given id is not found within the list of
   * Observable elements
   *
   * @return DataType
   */
  DataType getDataType(ElementID id);

private:
  /**
   * @brief Internal method used to send an observation request to the client.
   *
   * Issued on first call of attach(), after the internal listeners_ map is
   * empty
   *
   */
  void requestObserver();

  /**
   * @brief Internal method used to send a request to cancel Observation to the
   * Client.
   *
   * Issued when the last ObserverInterface implementer has called the detach()
   * method
   *
   */
  void cancelObserver();

  /**
   * @brief Implementation of Event_Model::EventSource<PayloadData>::attach()
   * method.
   *
   * Used by ObserverInterface constructor to add a new observer
   *
   * @param listener_callback - callback to
   * ObserverInterface::handleEvent(std::shared_ptr<LwM2M::PayloadData>)
   * implementation
   * @return size_t - observer_id
   */
  size_t attach(
      Event_Model::HandleEventCallback<PayloadData>&& listener_callback) final;

  /**
   * @brief Implementation of Event_Model::EventSource<PayloadData>::detach()
   * method.
   *
   * Used by ObserverInterface destructor to remove an observer
   *
   * @param callback_id - observer_id_ of ObserverInterface
   */
  void detach(size_t callback_id) final;

  ObservableInterfacePtr requester_;
  size_t observer_id_ = 0;
  ObservedDataTypes data_types_;
};

using ObservableSharedPtr = std::shared_ptr<Observable>;
using ObservablePtr = NonemptyPointer::NonemptyPtr<ObservableSharedPtr>;

/**
 * @brief A helper class, used to create observers for a given Observable data
 * source
 *
 * Inheritors of this class MUST implement method:
 * void handleEvent(std::shared_ptr<LwM2M::PayloadData>);
 *
 */
struct ObserverInterface
    : Event_Model::EventListenerInterface<LwM2M::PayloadData> {
  ObserverInterface(ObservablePtr source);

  Observable::ObservedDataTypes getObservedDataTypes();
  DataType getDataType(ElementID id);
  std::string getId();

private:
  std::string oberser_id_;
  Observable::ObservedDataTypes data_types_;
};

using ObserverInterfaceSharedPtr = std::shared_ptr<ObserverInterface>;
using ObserverInterfacePtr =
    NonemptyPointer::NonemptyPtr<ObserverInterfaceSharedPtr>;

/** @}*/
} // namespace LwM2M
#endif //__LWM2M_OBSERVABLE_HPP