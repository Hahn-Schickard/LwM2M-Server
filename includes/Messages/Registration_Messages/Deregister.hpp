#ifndef __LWM2M_REGISTRATION_INTERFACE_DEREGISTER_MESSAGE_HPP
#define __LWM2M_REGISTRATION_INTERFACE_DEREGISTER_MESSAGE_HPP

#include "RegistrationInterfaceMessage.hpp"

namespace LwM2M {
/**
 * @addtogroup Message_Model
 * @{
 */

/**
 * @brief Response to LwM2M::DeregisterRequest, indicates if the request
 * was successful, or not
 *
 */
struct DeregisterResponse : RegistrationInterfaceResponse {
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

  std::string name() const final;
};

using DeregisterResponsePtr = std::shared_ptr<DeregisterResponse>;

/**
 * @brief Used to deregister a certain LwM2M::Device from the server, identified
 * by unique DeviceID, specified in the location argument
 *
 */
struct DeregisterRequest : RegistrationInterfaceRequest {
  const std::string location_;

  DeregisterRequest(EndpointPtr endpoint, std::string location);

  std::string name() const final;

  DeregisterResponsePtr makeResponse(ResponseCode response_code);
};

using DeregisterRequestPtr = std::shared_ptr<DeregisterRequest>;

/** @}*/
} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_DEREGISTER_MESSAGE_HPP