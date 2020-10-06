#ifndef __LWM2M_MESSAGE_ENCODER_HPP
#define __LWM2M_MESSAGE_ENCODER_HPP

#include "DeviceManagmentMessages.hpp"
#include "LwM2M_Message.hpp"
#include "Response_Handler.hpp"
#include "ServerResponse_Register.hpp"

#include <memory>

namespace LwM2M {
using ResponseHandlerPtr = std::shared_ptr<ResponseHandler>;
class MessageEncoder {
  ResponseHandlerPtr response_handler_;

public:
  MessageEncoder(std::shared_ptr<ResponseHandler> response_handler)
      : response_handler_(response_handler) {}
  virtual ~MessageEncoder() = default;

  ResponseHandlerPtr getResponseHandler() { return response_handler_; }

  virtual ResponseFuture encode(std::unique_ptr<ServerRequest_Read> input) = 0;

  virtual void encode(std::unique_ptr<ServerRequest_Write> input) = 0;
  virtual void encode(std::unique_ptr<ServerResponse_Register> input) = 0;
  virtual void encode(std::unique_ptr<Response> input) = 0;
};
} // namespace LwM2M

#endif //__LWM2M_MESSAGE_ENCODER_HPP