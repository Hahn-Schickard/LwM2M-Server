#ifndef __LWM2M_COAP_BINDING_REQUESTS_MANAGER_HPP
#define __LWM2M_COAP_BINDING_REQUESTS_MANAGER_HPP

#include "CoAP/Socket.hpp"
#include "Logger.hpp"
#include "RequestsManagerInterface.hpp"

namespace LwM2M {
class CoAP_RequestsManager : public RequestsManagerInterface {
  CoAP::SocketPtr socket_;

public:
  CoAP_RequestsManager(ResponseHandlerPtr response_handler,
                       CoAP::SocketPtr socket);
  ~CoAP_RequestsManager();

  uint64_t dispatch(ServerRequestPtr request) override;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_BINDING_REQUESTS_MANAGER_HPP