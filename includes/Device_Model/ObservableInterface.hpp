#ifndef __LWM2M_OBSERVABLE_INTERFACE_HPP
#define __LWM2M_OBSERVABLE_INTERFACE_HPP

#include "CancelableInterface.hpp"
#include "InformationReporingMessage.hpp"

#include <future>
#include <stdexcept>

namespace LwM2M {

/**
 * @brief Models the functionality for observable requests. Used by Observable
 * resource traits and object instances
 *
 * MUST be implemented be the Binding
 *
 */
struct ObservableInterface : public CancelableInterface {
  virtual ~ObservableInterface() = default;

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
  virtual size_t requestObservation(
      const std::function<void(PayloadDataPtr)>& /* notify_cb */,
      const InformationReportingRequestPtr& /*message*/) {
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
      const InformationReportingRequestPtr& /*message*/) {
    throw std::runtime_error("Called base requestObservation implementation.");
  }
};

using ObservableInterfacePtr = std::shared_ptr<ObservableInterface>;
} // namespace LwM2M

#endif //__LWM2M_OBSERVABLE_INTERFACE_HPP