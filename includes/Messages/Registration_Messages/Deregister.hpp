#ifndef __LWM2M_REGISTRATION_INTERFACE_DEREGISTER_MESSAGE_HPP
#define __LWM2M_REGISTRATION_INTERFACE_DEREGISTER_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Response to LwM2M::DeregisterRequest, indicates if the request
 * was successfull, or not
 *
 */
struct DeregisterResponse : ServerResponse {
  /**
   * @brief Creates a DeregisterResponse with one of the supported failure
   * codes.
   *
   * Used to indicate failure.
   *
   * Supported response codes:
   * - ResponseCode::BAD_REQUEST - No location was given in the request.
   * - ResponseCode::NOT_FOUND - Given location could not be found in the server
   *
   * @param endpoint
   * @param response_code
   */
  DeregisterResponse(EndpointPtr endpoint, ResponseCode response_code);

  /**
   * @brief Creates a DeregisterResponse with assigned endpoint location.
   *
   * Used to indicate success.
   *
   * @param endpoint
   * @param location
   */
  DeregisterResponse(EndpointPtr endpoint, std::string location);

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