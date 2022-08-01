#ifndef __LWM2M_READABLE_INTERFACE_HPP
#define __LWM2M_READABLE_INTERFACE_HPP

#include "CancelableInterface.hpp"
#include "DeviceManagmentMessage.hpp"

#include <future>
#include <stdexcept>

namespace LwM2M {

/**
 * @brief Models the functionality for read requests. Used by Readable and
 * ReadAndWritable resource traits only
 *
 * MUST be implemented be the Binding
 *
 */
struct ReadableInterface : public CancelableInterface {
  virtual ~ReadableInterface() = default;

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
  virtual std::future<DataFormatPtr> requestData(
      DeviceManagementRequestPtr /*message*/) {
    throw std::runtime_error("Called base requestData implementation.");
  }
};

using ReadableInterfacePtr = std::shared_ptr<ReadableInterface>;
} // namespace LwM2M

#endif //__LWM2M_READABLE_INTERFACE_HPP