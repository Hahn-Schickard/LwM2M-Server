#ifndef __LWM2M_COMPOSITEREADABLE_INTERFACE_HPP
#define __LWM2M_COMPOSITEREADABLE_INTERFACE_HPP

#include "CancelableInterface.hpp"
#include "DeviceManagmentMessage.hpp"

#include <future>
#include <stdexcept>

namespace LwM2M {

/**
 * @brief Models the functionality of reading multiple values in a single
 * request. Used for Object Instance Reading
 *
 */
struct CompositeReadableInterface : public CancelableInterface {
  virtual ~CompositeReadableInterface() = default;

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
  virtual std::future<TargetContentVector> requestMultiTargetData(
      DeviceManagementRequestPtr /*message*/) {
    throw std::runtime_error(
        "Called base requestMultiTargetData implementation.");
  }
};

using CompositeReadableInterfacePtr =
    std::shared_ptr<CompositeReadableInterface>;
} // namespace LwM2M

#endif //__LWM2M_COMPOSITEREADABLE_INTERFACE_HPP