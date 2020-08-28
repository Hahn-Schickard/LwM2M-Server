#include "Response_Handler.hpp"
#include "Hashers.hpp"

using namespace std;

namespace LwM2M {

ResponseFuture ResponseHandler::getFuture(vector<uint8_t> token) {
  auto result_promise = ResponsePromise();
  auto result_future = result_promise.get_future();
  responses_.emplace(token, move(result_promise));
  return result_future;
}

void ResponseHandler::setFuture(unique_ptr<Response> response) {
  auto it = responses_.find(response->token_);
  if (it != responses_.end()) {
    auto value = response->payload_;
    it->second.set_value(value);
  }
}
} // namespace LwM2M