#ifndef __LWM2M_REQUESTER_INTERFACE_HPP
#define __LWM2M_REQUESTER_INTERFACE_HPP

#include "Message.hpp"

#include <future>

namespace LwM2M {

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
   *
   * @param message
   * @return std::future<DataFormat>
   */
  virtual std::future<DataFormat> requestData(ServerRequestPtr message) = 0;

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
   *
   * @param message
   * @return std::future<TargetContentVector>
   */
  virtual std::future<TargetContentVector>
  requestMultiTargetData(ServerRequestPtr message) = 0;

  /**
   * @brief Request f LwM2M::Device to do an Action
   *
   * @throws LwM2M::ResponseReturnedAnErrorCode if the corresponding
   * LwM2M::Response contained an error code
   * @throws LwM2M::RequestCanceled if request was canceled by the
   * LwM2M::Dispatcher
   * @throws LwM2M::RequestAlreadyDispatched if this request was already
   * dispatched
   *
   * @param message
   * @return std::future<bool>
   */
  virtual std::future<bool> requestAction(ServerRequestPtr message) = 0;
};

using RequesterPtr = std::shared_ptr<Requester>;
} // namespace LwM2M

#endif //__LWM2M_REQUESTER_INTERFACE_HPP