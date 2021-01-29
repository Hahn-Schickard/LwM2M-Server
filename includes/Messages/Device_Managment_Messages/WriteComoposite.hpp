#ifndef __LWM2M_DEVICE_MANAGMENT_WRITE_COMPOSITE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_WRITE_COMPOSITE_MESSAGE_HPP

#include "DataFormat.hpp"
#include "DeviceManagmentMessage.hpp"
#include "ElementID.hpp"

#include <utility>

namespace LwM2M {
/**
 * @brief Used to selectively write Resources, and/or Resource Instances of
 * different or same Objects/Object Instances in a single request,
 *
 * Content of each write target must be formated in application/senml+json or
 * application/senml+cbor [RFC8428] Sensor Measurement Lists data format
 *
 */
struct WriteComopositeRequest : public DeviceManagmentRequest {
  using TargetContent = std::pair<EelmentIdVariant, DataFormatPtr>;
  std::vector<TargetContent> content_;

  WriteComopositeRequest(EndpointPtr endpoint,
                         std::vector<TargetContent> content);

  void append(EelmentIdVariant target, DataFormatPtr content);
  void append(std::vector<EelmentIdVariant> targets, DataFormatPtr content);

  std::string name() override final;
};

using WriteComopositeRequestPtr = std::shared_ptr<WriteComopositeRequest>;

/**
 * @brief Response to LwM2M::WriteComopositeRequest, indicates wheater the
 * request was succefull, or not
 *
 * Supported response codes:
 * ResponseCode::CHANGED
 * ResponseCode::BAD_REQUEST
 * ResponseCode::UNAUTHORIZED
 * ResponseCode::NOT_FOUND
 * ResponseCode::METHOD_NOT_ALLOWED
 * ResponseCode::NOT_ACCEPTABLE
 */
struct WriteComopositeResponse : DeviceManagmentResponse {
  WriteComopositeResponse(EndpointPtr endpoint, ResponseCode response_code);

  std::string name() override final;
};

using WriteComopositeResponsePtr = std::shared_ptr<WriteComopositeResponse>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_WRITE_COMPOSITE_MESSAGE_HPP