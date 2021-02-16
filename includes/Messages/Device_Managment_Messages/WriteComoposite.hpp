#ifndef __LWM2M_DEVICE_MANAGMENT_WRITE_COMPOSITE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_WRITE_COMPOSITE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Used to selectively write Resources, and/or Resource Instances of
 * different or same Objects/Object Instances in a single request,
 *
 * Content of each write target must be formated in application/senml+json or
 * application/senml+cbor [RFC8428] Sensor Measurement Lists data format
 *
 */
struct WriteComopositeRequest : public ServerRequest {
  WriteComopositeRequest(
      EndpointPtr endpoint,
      std::vector<TargetContent> content = std::vector<TargetContent>());

  std::string name() override final;
};

using WriteComopositeRequestPtr = std::shared_ptr<WriteComopositeRequest>;

/**
 * @brief Response to LwM2M::WriteComopositeRequest, indicates wheater the
 * request was succefull, or not
 *
 * Supported response codes:
 * ResponseCode::CHANGED - Operation was a success.
 * ResponseCode::BAD_REQUEST - Data format is written in wrong Content Format.
 * ResponseCode::UNAUTHORIZED - Access rights permission denied.
 * ResponseCode::NOT_FOUND - None of target LwM2M::ElmentIdVariant instances
 * point to a valid element within the client. ResponseCode::METHOD_NOT_ALLOWED
 * - No target LwM2M::ElmentIdVariant instances are allowed to use
 * WriteComposite operation. ResponseCode::NOT_ACCEPTABLE - None of preferred
 * Content Formats are supported by the Client
 */
struct WriteComopositeResponse : ClientResponse {
  WriteComopositeResponse(EndpointPtr endpoint, ResponseCode response_code =
                                                    ResponseCode::BAD_REQUEST);

  std::string name() override final;
};

using WriteComopositeResponsePtr = std::shared_ptr<WriteComopositeResponse>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_WRITE_COMPOSITE_MESSAGE_HPP