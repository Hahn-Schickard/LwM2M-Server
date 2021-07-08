#ifndef __LWM2M_COAP_BINDING_REQUESTS_MANAGER_HPP
#define __LWM2M_COAP_BINDING_REQUESTS_MANAGER_HPP

#include "CoAP/Socket.hpp"
#include "Logger.hpp"
#include "RequestsManagerInterface.hpp"

namespace LwM2M {
struct CoAP_RequestsManager : public RequestsManagerInterface {
  using Requester =
      std::function<std::future<CoAP::MessagePtr>(CoAP::MessagePtr)>;

  CoAP_RequestsManager(ResponseHandlerPtr response_handler,
                       Requester requester);
  ~CoAP_RequestsManager();

  uint64_t dispatch(ServerRequestPtr request) override;

private:
  Requester requester_;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_BINDING_REQUESTS_MANAGER_HPP