#ifndef __LWM2M_OBSERVABLE_HPP
#define __LWM2M_OBSERVABLE_HPP

#include "Event_Model/EventListenerInterface.hpp"
#include "Event_Model/EventSource.hpp"

#include "CallableEntity.hpp"
#include "ObservableInterface.hpp"

#include <map>

namespace LwM2M {
struct ElementNotObserved : public std::runtime_error {
  ElementNotObserved(ElementID id)
      : runtime_error("Element " + id.toString() +
            " is not in the list of compoisite observed elements.") {}
};

struct Observable : public Event_Model::EventSource<PayloadData>,
                    public CallableEntity {
  using ExceptionHandler = std::function<void(std::exception_ptr)>;
  using ObservedDataTypes = std::map<ElementID, DataType>;

  Observable(ExceptionHandler handler, ObservableInterfacePtr requester,
      EndpointPtr endpoint, ElementID id, ObservedDataTypes data_types);
  Observable(ExceptionHandler handler, ObservableInterfacePtr requester,
      EndpointPtr endpoint, ElementID id, DataType data_type);

  ~Observable();

  ObservedDataTypes getObservedDataTypes();
  DataType getDataType(ElementID id);

private:
  void requestObserver();

  void cancelObserver();

  size_t attach(
      Event_Model::HandleEventCallback<PayloadData>&& listener_callback) final;

  void detach(size_t callback_id) final;

  ObservableInterfacePtr requester_;
  size_t observer_id_ = 0;
  ObservedDataTypes data_types_;
};

using ObservablePtr = std::shared_ptr<Observable>;

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

using ObserverInterfacePtr = std::shared_ptr<ObserverInterface>;
} // namespace LwM2M
#endif //__LWM2M_OBSERVABLE_HPP