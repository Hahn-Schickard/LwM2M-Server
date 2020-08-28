#ifndef __LWM2M_RESPONSE_HANDLER_HPP
#define __LWM2M_RESPONSE_HANDLER_HPP

#include "LwM2M_DataFormat.hpp"
#include "LwM2M_Message.hpp"

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

public:
  ResponseFuture getFuture(std::vector<uint8_t> token);
  void setFuture(std::unique_ptr<Response> response);
};
} // namespace LwM2M

#endif //__LWM2M_RESPONSE_HANDLER_HPP