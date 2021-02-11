#ifndef __LWM2M_REQUESTS_MANAGER_HPP
#define __LWM2M_REQUESTS_MANAGER_HPP

#include "Message.hpp"

#include <future>
#include <unordered_map>

namespace LwM2M {

struct RequestNotFound : protected std::runtime_error {
  RequestNotFound(uint64_t request_identifier);
};

struct RequestAlreadyDispatched : protected std::runtime_error {
  RequestAlreadyDispatched(uint64_t request_identifier);
};

struct RequestCanceled : protected std::runtime_error {
  RequestCanceled();
};

class RequestsManager {
  std::unordered_map<uint64_t, std::promise<ClientResponsePtr>> requests_;

  /**
   * @brief Removes a given request identifier and associated std::promise pair
   * form the map. If a given request identifier does not have an associated
   * std::promise, silentlly ignores it.
   *
   * @param request_identifier
   */
  void cancelRequest(uint64_t request_identifier);

public:
  /**
   * @brief Removes given request ids from the requests map.
   * Called by LwM2M::Dispatcher destructor
   *
   * @param request_identifiers
   */
  void cleanup(std::vector<uint64_t> request_identifiers);

  /**
   * @brief Assigns an LwM2M::Response future to a given identifier
   *
   * @throws RequestCanceled if the request was cancel by calling cleanup()
   * @throws RequestAlreadyDispatched if a given identfier already has an
   * associated LwM2M::Response future
   *
   * @param request_identifier
   * @return std::future<ClientResponsePtr>
   */
  std::future<ClientResponsePtr> request(uint64_t request_identifier);

  /**
   * @brief Sets the promised value of request() method
   *
   * @throws RequestNotFound if identifier does not have an associated
   * std::promise
   *
   * @param request_identifier
   * @param response
   */
  void respond(uint64_t request_identifier, ClientResponsePtr response);
};

using RequestsManagerPtr = std::shared_ptr<RequestsManager>;
} // namespace LwM2M

#endif //__LWM2M_REQUESTS_MANAGER_HPP