#ifndef __LWM2M_REGISTRATION_INTERFACE_DEREGISTER_MESSAGE_HPP
#define __LWM2M_REGISTRATION_INTERFACE_DEREGISTER_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Response to LwM2M::DeregisterRequest, indicates wheater the request
 * was succefull, or not
 *
 * Supported response codes:
 * - ResponseCode::DELETED - Operation was a success.
 * - ResponseCode::BAD_REQUEST - No location was given in the request.
 * - ResponseCode::NOT_FOUND - Given location could not be found in the server
 */
struct DeregisterResponse : ServerResponse {
  DeregisterResponse(EndpointPtr endpoint, ResponseCode response_code);

  std::string name() override final;
};

using DeregisterResponsePtr = std::shared_ptr<DeregisterResponse>;

/**
 * @brief Used to deregister a certain LwM2M::Device from the server, identified
 * by unique DeviceID, specified in the location argument
 *
 */
struct DeregisterRequest : ClientRequest {
  const std::string location_;

  DeregisterRequest(EndpointPtr endpoint, std::string location);

  std::string name() override final;

  DeregisterResponsePtr makeResponse(ResponseCode response_code);
};

using DeregisterRequestPtr = std::shared_ptr<DeregisterRequest>;
} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_DEREGISTER_MESSAGE_HPP