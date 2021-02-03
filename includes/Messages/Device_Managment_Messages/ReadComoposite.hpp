#ifndef __LWM2M_DEVICE_MANAGMENT_READ_COMPOSITE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_READ_COMPOSITE_MESSAGE_HPP

#include "Message.hpp"

#include <vector>

namespace LwM2M {
/**
 * @brief Used to selectively read any combination of Objects,Object
 * Instance(s), Resources, and/or Resource Instances of different or same
 * Objects/Object Instances in a single request
 *
 */
struct ReadComopositeRequest : ServerRequest {
  const std::vector<EelmentIdVariant> targets_;

  /**
   * @brief Construct a new Read Comoposite Request object
   *
   * @param endpoint
   * @param uri_targets - a list of Object/Object Instance/Resource/Resoruce
   * Instance URIs that will be read
   */
  ReadComopositeRequest(
      EndpointPtr endpoint,
      std::vector<EelmentIdVariant> targets = std::vector<EelmentIdVariant>());

  std::string name() override final;
};

using ReadComopositeRequestPtr = std::shared_ptr<ReadComopositeRequest>;

/**
 * @brief Response to LwM2M::ReadComopositeRequest, contains a list of
 * application/senml+json or application/senml+cbor [RFC8428] Sensor Measurement
 * Lists where the records contain Base Name and/or Name Fields, but no Value
 * fields.
 *
 * Supported response codes:
 * - ResponseCode::CONTENT - Operation was a success.
 * - ResponseCode::BAD_REQUEST - Client encountered an undetermened error, while
 * processing the request.
 * - ResponseCode::UNAUTHORIZED - Access rights permission denied.
 * - ResponseCode::NOT_FOUND - None of target EelmentIdVariant instances point
 * to a valid element within the client
 * - ResponseCode::METHOD_NOT_ALLOWED - None of target EelmentIdVariant
 * instances are allowed to use Read operation.
 * - ResponseCode::NOT_ACCEPTABLE - None of preferred Content Formats are
 * supported by the Client
 */
struct ReadComopositeResponse : ClientResponse {
  const std::vector<TargetContent> content_;

  ReadComopositeResponse(EndpointPtr endpoint, ResponseCode response_code,
                         std::vector<TargetContent> content);

  std::string name() override final;
};

using ReadComopositeResponsePtr = std::shared_ptr<ReadComopositeResponse>;

} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_READ_COMPOSITE_MESSAGE_HPP