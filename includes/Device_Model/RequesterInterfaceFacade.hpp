#ifndef __LWM2M_REQUESTER_INTERFACE_FACADE_INTERFACE_HPP
#define __LWM2M_REQUESTER_INTERFACE_FACADE_INTERFACE_HPP

#include "CompositeReadableInterface.hpp"
#include "ExecutableInterface.hpp"
#include "ObservableInterface.hpp"
#include "ReadableInterface.hpp"

#include <future>
#include <stdexcept>

namespace LwM2M {

struct ResponseReturnedAnErrorCode : public std::runtime_error {
  ResponseCode response_code_;

  ResponseReturnedAnErrorCode(
      ClientResponsePtr response, ServerRequestPtr request)
      : runtime_error(response->name() + " returned " +
            toString(response->response_code_) + " for " + request->name()),
        response_code_(response->response_code_) {}
};

struct ResponseReturnedAnEmptyPayload : public std::runtime_error {
  ResponseReturnedAnEmptyPayload(
      ClientResponsePtr response, ServerRequestPtr request)
      : runtime_error(response->name() + " has no payload " + " for " +
            request->name()) {}
};

/**
 * @brief A composition of various Interfaces, that Binding_Interface MUST
 * implement
 *
 */
struct RequesterInterfaceFacade : public ReadableInterface,
                                  public CompositeReadableInterface,
                                  public ExecutableInterface,
                                  public ObservableInterface {
  virtual ~RequesterInterfaceFacade() = default;

  /**
   * @brief Requests LwM2M::Device a generic LwM2M Server Request
   *
   * @throws std::logic_error if an internal failure occurred
   * @throws std::runtime_error exception if called base virtual implementation
   *
   * @param message
   * @return std::future<ClientResponse>
   */
  virtual std::future<ClientResponsePtr> request(
      const ServerRequestPtr& /*message*/) {
    throw std::runtime_error("Called base request implementation.");
  }

  /**
   * @brief Override specifications to resolve multiple CancelableInterface
   * inheritors
   *
   * MUST be implemented by the Binding_Interface inheritor
   */
  virtual void cancelRequest(const ServerRequestPtr& /*message*/) override {
    throw std::runtime_error("Called base cancelRequest implementation within "
                             "RequesterInterfaceFacade.");
  }
};

using RequesterInterfaceFacadePtr = std::shared_ptr<RequesterInterfaceFacade>;
} // namespace LwM2M

#endif //__LWM2M_REQUESTER_INTERFACE_FACADE_INTERFACE_HPP