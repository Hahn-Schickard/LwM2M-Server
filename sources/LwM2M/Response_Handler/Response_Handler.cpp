#include "Response_Handler.hpp"
#include "Hashers.hpp"

using namespace std;

namespace LwM2M {
ResponseHandler::ResponseHandler(shared_ptr<MessageEncoder> encoder)
    : encoder_(encoder) {}

ResponseFuture
ResponseHandler::generateRequest(unique_ptr<Read_Request> request) {
  auto result_promise = ResponsePromise();
  auto result_future = result_promise.get_future();
  responses_.emplace(request->token_, move(result_promise));
  encoder_->encode(move(request));
  return result_future;
}

void ResponseHandler::setResponse(unique_ptr<Response> response) {
  auto it = responses_.find(response->token_);
  if (it != responses_.end()) {
    auto value = response->payload_;
    it->second.set_value(value);
  }
}
} // namespace LwM2M