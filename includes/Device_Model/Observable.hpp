#ifndef __LWM2M_OBSERVABLE_HPP
#define __LWM2M_OBSERVABLE_HPP

#include "Event_Model/EventListenerInterface.hpp"
#include "Event_Model/EventSource.hpp"

#include "CallableEntity.hpp"

namespace LwM2M {
class Observable : public Event_Model::EventSource<PayloadData>,
                   public CallableEntity {
  size_t observer_id_ = 0;

  void requestObserver();

  void cancelObserver();

public:
  using ExceptionHandler = std::function<void(std::exception_ptr)>;

  Observable(ExceptionHandler handler, RequesterPtr requester,
             EndpointPtr endpoint, ElementID id);

  ~Observable();

  size_t attach(
      Event_Model::HandleEventCallback<PayloadData> &&listener_callback) final;

  void detach(size_t callback_id) final;
};

using ObservablePtr = std::shared_ptr<Observable>;

struct ObserverInterface
    : Event_Model::EventListenerInterface<LwM2M::PayloadData> {
  ObserverInterface(ObservablePtr source)
      : EventListenerInterface(source),
        oberser_id_(source->getID().toString()) {}

  std::string getId() { return oberser_id_; }

protected:
  std::string oberser_id_;
};

using ObserverInterfacePtr = std::shared_ptr<ObserverInterface>;
} // namespace LwM2M
#endif //__LWM2M_OBSERVABLE_HPP