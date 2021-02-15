#include "ResponseHandler.hpp"

using namespace std;

namespace LwM2M {

RequestNotFound::RequestNotFound(uint64_t request_identifier)
    : runtime_error("Request " + to_string(request_identifier) + " not found") {
}

RequestAlreadyDispatched::RequestAlreadyDispatched(uint64_t request_identifier)
    : runtime_error("Request " + to_string(request_identifier) +
                    " was alread dispatched.") {}

RequestCanceled::RequestCanceled() : runtime_error("Request was cancled") {}

void ResponseHandler::cancelRequest(uint64_t request_identifier) {
  auto request = requests_.find(request_identifier);
  if (request != requests_.end()) {
    request->second.set_exception(make_exception_ptr(RequestCanceled()));
    requests_.erase(request);
  }
}

void ResponseHandler::cleanup(vector<uint64_t> request_identifiers) {
  for (auto identifier : request_identifiers) {
    cancelRequest(identifier);
  }
}

future<ClientResponsePtr>
ResponseHandler::request(uint64_t request_identifier) {
  auto request = requests_.find(request_identifier);
  if (request == requests_.end()) {
    promise<ClientResponsePtr> response_promise;
    auto response_future = response_promise.get_future();
    requests_.emplace(request_identifier, move(response_promise));
    return move(response_future);
  } else {
    throw RequestAlreadyDispatched(request_identifier);
  }
}

void ResponseHandler::respond(uint64_t request_identifier,
                              ClientResponsePtr response) {
  auto request = requests_.find(request_identifier);
  if (request != requests_.end()) {
    request->second.set_value(response);
    requests_.erase(request);
  } else {
    throw RequestNotFound(request_identifier);
  }
}
} // namespace LwM2M