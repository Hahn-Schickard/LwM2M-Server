#include "Observable.hpp"

#include "CancelObservation.hpp"
#include "Observe.hpp"

namespace LwM2M {

DataType getDataTypeFromMap(Observable::ObservedDataTypes data_types,
                            ElementID id) {
  auto data_type = data_types.find(id);
  if (data_type != data_types.end()) {
    return data_type->second;
  } else {
    throw ElementNotObserved(id);
  }
}

Observable::Observable(Observable::ExceptionHandler handler,
                       RequesterPtr requester, EndpointPtr endpoint,
                       ElementID id, ObservedDataTypes data_types)
    : Event_Model::EventSource<PayloadData>(handler),
      CallableEntity(requester, endpoint, id), data_types_(data_types) {}

Observable::Observable(ExceptionHandler handler, RequesterPtr requester,
                       EndpointPtr endpoint, ElementID id, DataType data_type)
    : Observable(handler, requester, endpoint, id,
                 ObservedDataTypes{{id, data_type}}) {}

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

Observable::ObservedDataTypes Observable::getObservedDataTypes() {
  return data_types_;
}

DataType Observable::getDataType(ElementID id) {
  return getDataTypeFromMap(data_types_, id);
}

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

size_t Observable::attach(
    Event_Model::HandleEventCallback<PayloadData> &&listener_callback) {
  if (!hasListeners()) {
    requestObserver();
  }

  return EventSource::attach(move(listener_callback));
}

void Observable::detach(size_t callback_id) {
  this->detach(callback_id);

  if (!hasListeners()) {
    cancelObserver();
  }
}
} // namespace LwM2M