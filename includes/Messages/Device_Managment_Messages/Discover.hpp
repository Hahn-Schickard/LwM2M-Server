#ifndef __LWM2M_DEVICE_MANAGMENT_DISCOVER_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_DISCOVER_MESSAGE_HPP

#include "DataFormat.hpp"
#include "DeviceManagmentMessage.hpp"
#include "ElementID.hpp"

namespace LwM2M {
/**
 * @brief Used to discover LwM2M Attributes attached to an Object, Object
 * Instances, and Resources as well as what Resources are instantiated in a
 * given Object Instance.
 *
 */
struct DiscoverRequest : DeviceManagmentRequest {
  EelmentIdVariant target_id_;

  DiscoverRequest(EndpointPtr endpoint, EelmentIdVariant target_id);

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
 * ResponseCode::CONTENT
 * ResponseCode::BAD_REQUEST
 * ResponseCode::UNAUTHORIZED
 * ResponseCode::NOT_FOUND
 * ResponseCode::METHOD_NOT_ALLOWED
 *
 */
struct DiscoverResponse : DeviceManagmentResponse {
  DataFormatPtr content_;

  DiscoverResponse(EndpointPtr endpoint, ResponseCode response_code,
                   DataFormatPtr content);

  std::string name() override final;
};

using DiscoverResponsePtr = std::shared_ptr<DiscoverResponse>;

} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_DISCOVER_MESSAGE_HPP