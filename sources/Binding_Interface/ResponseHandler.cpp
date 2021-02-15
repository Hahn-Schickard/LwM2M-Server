#include "ResponseHandler.hpp"

using namespace std;

namespace LwM2M {

RequestNotFound::RequestNotFound(uint64_t response_identifier)
    : runtime_error("Request " + to_string(response_identifier) +
                    " not found") {}

RequestAlreadyDispatched::RequestAlreadyDispatched(uint64_t response_identifier)
    : runtime_error("Request " + to_string(response_identifier) +
                    " was alread dispatched.") {}

RequestCanceled::RequestCanceled() : runtime_error("Request was cancled") {}

void ResponseHandler::cancelRequest(uint64_t response_identifier) {
  auto response_promise = response_promises_.find(response_identifier);
  if (response_promise != response_promises_.end()) {
    response_promise->second.set_exception(
        make_exception_ptr(RequestCanceled()));
    response_promises_.erase(response_promise);
  }
}

void ResponseHandler::cleanup(vector<uint64_t> response_identifiers) {
  for (auto identifier : response_identifiers) {
    cancelRequest(identifier);
  }
}

future<ClientResponsePtr>
ResponseHandler::request(uint64_t response_identifier) {
  auto response_promise = response_promises_.find(response_identifier);
  if (response_promise == response_promises_.end()) {
    promise<ClientResponsePtr> response_promise;
    auto response_future = response_promise.get_future();
    response_promises_.emplace(response_identifier, move(response_promise));
    return move(response_future);
  } else {
    throw RequestAlreadyDispatched(response_identifier);
  }
}

void ResponseHandler::respond(uint64_t response_identifier,
                              ClientResponsePtr response) {
  auto response_promise = response_promises_.find(response_identifier);
  if (response_promise != response_promises_.end()) {
    response_promise->second.set_value(response);
    response_promises_.erase(response_promise);
  } else {
    throw RequestNotFound(response_identifier);
  }
}
} // namespace LwM2M