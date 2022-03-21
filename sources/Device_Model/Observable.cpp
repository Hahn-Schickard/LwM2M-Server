#include "Observable.hpp"

#include "CancelObservation.hpp"
#include "Observe.hpp"

namespace LwM2M {

void Observable::requestObserver() {
  auto request = std::make_shared<ObserveRequest>(endpoint_, id_);
  observer_id_ = requester_->requestObservation(
      std::bind(&Observable::notify, this, std::placeholders::_1), request);
}

void Observable::cancelObserver() {
  auto request = std::make_shared<CancelObservationRequest>(endpoint_, id_);
  requester_->cancelObservation(observer_id_, request);
  observer_id_ = 0;
}

Observable::Observable(Observable::ExceptionHandler handler,
                       RequesterPtr requester, EndpointPtr endpoint,
                       ElementID id)
    : Event_Model::EventSource<PayloadData>(handler),
      CallableEntity(requester, endpoint, id) {}

Observable::~Observable() {
  if (hasListeners()) {
    try {
      cancelObserver();
    } catch (...) {
      auto exception_ptr = std::current_exception();
      this->handler_(exception_ptr);
    }
  }
}

size_t Observable::attach(
    Event_Model::HandleEventCallback<PayloadData> &&listener_callback) {
  if (!hasListeners()) {
    requestObserver();
  }

  return this->attach(move(listener_callback));
}

void Observable::detach(size_t callback_id) {
  this->detach(callback_id);

  if (!hasListeners()) {
    cancelObserver();
  }
}
} // namespace LwM2M