#ifndef __LWM2M_REQUESTER_INTERFACE_HPP
#define __LWM2M_REQUESTER_INTERFACE_HPP

#include "Device_Managment_Messages/DeviceManagmentMessage.hpp"
#include "Information_Reporting_Messages/InformationReporingMessage.hpp"

#include <functional>
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
   * @throws LwM2M::ResponseReturnedAnEmptyPayload if response contains no
   * payload
   * @throws std::logic_error if an internal failure occurred
   * @throws std::runtime_error exception if called base virtual implementation
   * or if canceled
   *
   * @param message
   * @return std::future<DataFormat>
   */
  virtual std::future<DataFormatPtr>
  requestData(DeviceManagementRequestPtr /*message*/) {
    throw std::runtime_error("Called base requestData implementation.");
  }

  /**
   * @brief Cancels an issued request. Canceled requests must throw and
   * exception, so issues can clean up the memory of the response future
   *
   * @throws std::runtime_error exception if called base virtual implementation
   *
   */
  virtual void cancelRequest(ServerRequestPtr /*message*/) {
    throw std::runtime_error("Called base cancelRequest implementation.");
  }

  /**
   * @brief Requests multiple instances of LwM2M:DataFormat objects, wrapped in
   * a LwM2M::TargetContentVector from the LwM2M::Device
   *
   * @throws LwM2M::ResponseReturnedAnErrorCode if the corresponding
   * LwM2M::Response contained an error code
   * * @throws LwM2M::ResponseReturnedAnEmptyPayload if response contains no
   * payload
   * @throws std::logic_error if an internal failure occurred
   * @throws std::runtime_error exception if called base virtual implementation
   *
   * @param message
   * @return std::future<TargetContentVector>
   */
  virtual std::future<TargetContentVector>
  requestMultiTargetData(DeviceManagementRequestPtr /*message*/) {
    throw std::runtime_error(
        "Called base requestMultiTargetData implementation.");
  }

  /**
   * @brief Request LwM2M::Device to do an Action
   *
   * @throws LwM2M::ResponseReturnedAnErrorCode if the corresponding
   * LwM2M::Response contained an error code
   * @throws std::logic_error if an internal failure occurred
   * @throws std::runtime_error exception if called base virtual implementation
   *
   * @param message
   * @return std::future<bool>
   */
  virtual std::future<bool>
  requestAction(DeviceManagementRequestPtr /*message*/) {
    throw std::runtime_error("Called base requestAction implementation.");
  }

  /**
   * @brief Assigns a given callback functor to call once a LwM2M::Notify
   * message has been received and requests the appropriate LwM2M::Device to
   * assign an observer for a given element or a composition of elements
   *
   * @throws LwM2M::ResponseReturnedAnErrorCode if the corresponding
   * LwM2M::Response contained an error code
   * @throws std::logic_error if an internal failure occurred
   * @throws std::runtime_error exception if called base virtual implementation
   *
   * @param notify_cb - observer notification callback
   * @param message - appropriate observation request for the appropriate device
   * @return size_t - observer notification callback identifier, can not be 0
   */
  virtual size_t
  requestObservation(std::function<void(PayloadDataPtr)> /* notify_cb */,
                     InformationReportingRequestPtr /*message*/) {
    throw std::runtime_error("Called base requestObservation implementation.");
  }

  /**
   * @brief Searches for a given observer notification callback and cancels
   * observation of LwM2M::Device element or a composition of elements
   *
   * @throws std::out_of_range if a given observer notification callback
   * identifier can not be found
   * @throws std::logic_error if an internal failure occurred
   * @throws std::runtime_error exception if called base virtual implementation
   *
   * @param observer_id - observer notification callback identifier, can not be
   * 0
   * @param message - appropriate cancel observation request for the appropriate
   * device
   */
  virtual void cancelObservation(size_t /* observer_id */,
                                 InformationReportingRequestPtr /*message*/) {
    throw std::runtime_error("Called base requestObservation implementation.");
  }

  /**
   * @brief Requests LwM2M::Device a generic LwM2M Server Request
   *
   * @throws std::logic_error if an internal failure occurred
   * @throws std::runtime_error exception if called base virtual implementation
   *
   * @param message
   * @return std::future<ClientResponse>
   */
  virtual std::future<ClientResponsePtr> request(ServerRequestPtr /*message*/) {
    throw std::runtime_error("Called base request implementation.");
  }
};

using RequesterPtr = std::shared_ptr<Requester>;
} // namespace LwM2M

#endif //__LWM2M_REQUESTER_INTERFACE_HPP