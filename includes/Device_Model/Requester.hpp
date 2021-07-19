#ifndef __LWM2M_REQUESTER_INTERFACE_HPP
#define __LWM2M_REQUESTER_INTERFACE_HPP

#include "Message.hpp"

#include <future>
#include <stdexcept>

namespace LwM2M {

struct ResponseReturnedAnErrorCode : public std::runtime_error {
  ResponseCode response_code_;

  ResponseReturnedAnErrorCode(ClientResponsePtr response,
                              ServerRequestPtr request)
      : runtime_error(response->name() + " returned " +
                      toString(response->response_code_) + " for " +
                      request->name()),
        response_code_(response->response_code_) {}
};

struct ResponseReturnedAnEmptyPayload : public std::runtime_error {
  ResponseReturnedAnEmptyPayload(ClientResponsePtr response,
                                 ServerRequestPtr request)
      : runtime_error(response->name() + " has no payload " + " for " +
                      request->name()) {}
};

/**
 * @brief Implemented by LwM2M::DispatcherInterface
 *
 */
struct Requester {
  virtual ~Requester() = default;

  /**
   * @brief Requests a single instance of LwM2M::DataFormat from the
   * LwM2M::Device
   *
   * @throws LwM2M::ResponseReturnedAnErrorCode if the corresponding
   * LwM2M::Response contained an error code
   * @throws LwM2M::RequestCanceled if request was canceled by the
   * LwM2M::Dispatcher
   * @throws LwM2M::RequestAlreadyDispatched if this request was already
   * dispatched
   * @throws std::logic_error if an internal failure occurred
   *
   * @param message
   * @return std::future<DataFormat>
   */
  virtual std::future<DataFormatPtr> requestData(ServerRequestPtr message) {
    throw std::runtime_error("Called based requestData implementation.");
  }

  /**
   * @brief Requests multiple instances of LwM2M:DataFormat objects, wrapped in
   * a LwM2M::TargetContentVector from the LwM2M::Device
   *
   * @throws LwM2M::ResponseReturnedAnErrorCode if the corresponding
   * LwM2M::Response contained an error code
   * @throws LwM2M::RequestCanceled if request was canceled by the
   * LwM2M::Dispatcher
   * @throws LwM2M::RequestAlreadyDispatched if this request was already
   * dispatched
   * @throws std::logic_error if an internal failure occurred
   *
   * @param message
   * @return std::future<TargetContentVector>
   */
  virtual std::future<TargetContentVector>
  requestMultiTargetData(ServerRequestPtr message) {
    throw std::runtime_error(
        "Called based requestMultiTargetData implementation.");
  }

  /**
   * @brief Request LwM2M::Device to do an Action
   *
   * @throws LwM2M::ResponseReturnedAnErrorCode if the corresponding
   * LwM2M::Response contained an error code
   * @throws LwM2M::RequestCanceled if request was canceled by the
   * LwM2M::Dispatcher
   * @throws LwM2M::RequestAlreadyDispatched if this request was already
   * dispatched
   * @throws std::logic_error if an internal failure occurred
   *
   * @param message
   * @return std::future<bool>
   */
  virtual std::future<bool> requestAction(ServerRequestPtr message) {
    throw std::runtime_error("Called based requestAction implementation.");
  }
};

using RequesterPtr = std::shared_ptr<Requester>;
} // namespace LwM2M

#endif //__LWM2M_REQUESTER_INTERFACE_HPP