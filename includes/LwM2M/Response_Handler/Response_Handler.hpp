#ifndef __LWM2M_RESPONSE_HANDLER_HPP
#define __LWM2M_RESPONSE_HANDLER_HPP

#include "LwM2M_DataFormat.hpp"
#include "Message_Encoder.hpp"
#include "RegistrationMessages.hpp"

#include <cstdint>
#include <future>
#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

namespace LwM2M {

using ReturnType = std::shared_ptr<DataFormat>;
using ResponseFuture = std::future<ReturnType>;

class ResponseHandler {
  using Token = std::vector<uint8_t>;
  using ResponsePromise = std::promise<ReturnType>;
  std::unordered_map<Token, ResponsePromise> responses_;
  std::shared_ptr<MessageEncoder> encoder_;

public:
  ResponseHandler(std::shared_ptr<MessageEncoder> encoder);

  ResponseFuture setRequest(std::unique_ptr<Read_Request> request);
  void setResponse(std::unique_ptr<Response> response);
};
} // namespace LwM2M

#endif //__LWM2M_RESPONSE_HANDLER_HPP