#ifndef __LWM2M_OBSERVABLE_HPP
#define __LWM2M_OBSERVABLE_HPP

#include "Event_Model/EventSource.hpp"

#include "CallableEntity.hpp"
#include "CancelObservation.hpp"
#include "Observe.hpp"

namespace LwM2M {
class Observable : public Event_Model::EventSource<PayloadData>,
                   public CallableEntity {
  size_t observer_id_ = 0;

  void requestObserver() {
    auto request = std::make_shared<ObserveRequest>(endpoint_, id_);
    observer_id_ = requester_->requestObservation(
        std::bind(&Observable::notify, this, std::placeholders::_1), request);
  }

  void cancelObserver() {
    auto request = std::make_shared<CancelObservationRequest>(endpoint_, id_);
    requester_->cancelObservation(observer_id_, request);
    observer_id_ = 0;
  }

public:
  using ExceptionHandler = std::function<void(std::exception_ptr)>;

  Observable(ExceptionHandler handler, RequesterPtr requester,
             EndpointPtr endpoint, ElementID id)
      : Event_Model::EventSource<PayloadData>(handler),
        CallableEntity(requester, endpoint, id) {}

  ~Observable() {
    if (hasListeners()) {
      try {
        cancelObserver();
      } catch (...) {
        auto exception_ptr = std::current_exception();
        this->handler_(exception_ptr);
      }
    }
  }

  virtual size_t attach(Event_Model::HandleEventCallback<PayloadData>
                            &&listener_callback) override final {
    if (!hasListeners()) {
      requestObserver();
    }

    return this->attach(move(listener_callback));
  }

  virtual void detach(size_t callback_id) override final {
    this->detach(callback_id);

    if (!hasListeners()) {
      cancelObserver();
    }
  }
};
} // namespace LwM2M
#endif //__LWM2M_OBSERVABLE_HPP