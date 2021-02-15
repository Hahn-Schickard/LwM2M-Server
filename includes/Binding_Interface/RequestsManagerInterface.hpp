#ifndef __LWM2M_REQUESTER_HPP
#define __LWM2M_REQUESTER_HPP

#include "Requester.hpp"
#include "ResponseHandler.hpp"

#include <cstdint>

namespace LwM2M {

struct ResponseReturnedAnErrorCode : protected std::runtime_error {
  ResponseReturnedAnErrorCode(ClientResponsePtr response,
                              ServerRequestPtr request);
};

/**
 * @brief Protocol specifc LwM2M::ServerRequest Dispatcher, that converts
 * outgoing LwM2M::ServerRequest instances into protocol specifc messages and
 * dispatches them to the correct endpoint.
 *
 */
struct RequestsManagerInterface : Requester {
  RequestsManagerInterface(ResponseHandlerPtr response_handler);
  ~RequestsManagerInterface();

  /**
   * @brief Converts a given request into the protocol specifc message and
   * assigns a unique id, which will be used to set the LwM2M::ClientResponse
   *
   * @param message
   * @return uint64_t - unique request id
   */
  virtual uint64_t dispatch(ServerRequestPtr request) = 0;

protected:
  void cleanup();

private:
  /**
   * @brief Dispatches a given request by calling dispatch(), sets the given
   * request in LwM2M::ResponseHandler and waits for a response. Once a response
   * was received, frees up the assign request identifier for reuse and returns
   * the LwM2M::Response for processing
   *
   * @param request
   * @return ClientResponsePtr
   */
  ClientResponsePtr dispatchAndGet(ServerRequestPtr request);

  /**
   * @brief Dispatches a given LwM2M::ServerRequest and handles the associated
   * LwM2M::ClientResponse payload as LwM2M::TargetContentVector.
   *
   * @param message
   * @return std::future<TargetContentVector>
   */
  std::future<TargetContentVector>
  handleResponseWithTargetContentVector(ServerRequestPtr message);

  /**
   * @brief Dispatches a given LwM2M::ServerRequest and handles the associated
   * LwM2M::ClientResponse payload as LwM2M::DataFormat.
   *
   * @param message
   * @return std::future<DataFormat>
   */
  std::future<DataFormat>
  handleResponseWithDataFormat(ServerRequestPtr message);

  /**
   * @brief Dispatches a given LwM2M::ServerRequest and handles the associated
   * LwM2M::ClientResponse LwM2M::ResponseCode. Sets the promised std::future
   * value to false if LwM2M::ResponseCode is an Error Code.
   *
   * @param message
   * @return std::future<bool>
   */
  std::future<bool> handleResponse(ServerRequestPtr message);

  std::future<TargetContentVector>
  requestMultiTargetData(ServerRequestPtr message) override;

  std::future<DataFormat> requestData(ServerRequestPtr message) override;

  std::future<bool> requestAction(ServerRequestPtr message) override;

  ResponseHandlerPtr requests_manager_;
  std::vector<uint64_t> dispatched_;
};

using RequestsManagerInterfacePtr = std::shared_ptr<RequestsManagerInterface>;
} // namespace LwM2M

#endif //__LWM2M_REQUESTER_HPP