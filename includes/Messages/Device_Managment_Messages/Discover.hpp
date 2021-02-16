#ifndef __LWM2M_DEVICE_MANAGMENT_DISCOVER_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_DISCOVER_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Used to discover LwM2M Attributes attached to an Object, Object
 * Instances, and Resources as well as what Resources are instantiated in a
 * given Object Instance.
 *
 */
struct DiscoverRequest : ServerRequest {
  DiscoverRequest(EndpointPtr endpoint,
                  ElmentIdVariant target_id = ObjectID(0));

  std::string name() override final;
};

using DiscoverRequestPtr = std::shared_ptr<DiscoverRequest>;

/**
 * @brief Response to LwM2M::DiscoverRequest, contains a list of
 * application/link-format CoRE Links [RFC6690] for each targeted Object, Object
 * Instance, or Resource, alongwith their assigned or attached Attributes
 * including the Object Version attribute if required
 *
 * Supported response codes:
 * - ResponseCode::CONTENT - Operation was a success.
 * - ResponseCode::BAD_REQUEST - Client encountered an undetermened error, while
 * processing the request.
 * - ResponseCode::UNAUTHORIZED - Access rights permission denied.
 * - ResponseCode::NOT_FOUND - Target LwM2M::ElmentIdVariant does not point to a
 * valid element within the client.
 * - ResponseCode::METHOD_NOT_ALLOWED - Target LwM2M::ElmentIdVariant is not
 * allowed to use Discover operation.
 *
 */
struct DiscoverResponse : ClientResponse {
  DiscoverResponse(EndpointPtr endpoint, ResponseCode response_code,
                   DataFormatPtr content);
  DiscoverResponse(EndpointPtr endpoint,
                   ResponseCode response_code = ResponseCode::BAD_REQUEST);

  std::string name() override final;
};

using DiscoverResponsePtr = std::shared_ptr<DiscoverResponse>;

} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_DISCOVER_MESSAGE_HPP