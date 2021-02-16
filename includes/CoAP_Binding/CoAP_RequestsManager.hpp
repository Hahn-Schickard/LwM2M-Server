#ifndef __LWM2M_COAP_BINDING_REQUESTS_MANAGER_HPP
#define __LWM2M_COAP_BINDING_REQUESTS_MANAGER_HPP

#include "CoAP/Server.hpp"
#include "RequestsManagerInterface.hpp"

namespace LwM2M {
class CoAP_RequestsManager : public RequestsManagerInterface {
  std::shared_ptr<CoAP::Server> server_;

public:
  CoAP_RequestsManager(ResponseHandlerPtr response_handler,
                       std::shared_ptr<CoAP::Server> server);

  uint64_t dispatch(ServerRequestPtr request) override;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_BINDING_REQUESTS_MANAGER_HPP