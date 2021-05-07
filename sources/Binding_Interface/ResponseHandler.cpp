#include "ResponseHandler.hpp"
#include "LoggerRepository.hpp"

using namespace std;
using namespace HaSLL;

namespace LwM2M {

RequestNotFound::RequestNotFound(uint64_t response_identifier)
    : runtime_error("Request " + to_string(response_identifier) +
                    " not found") {}

RequestAlreadyDispatched::RequestAlreadyDispatched(uint64_t response_identifier)
    : runtime_error("Request " + to_string(response_identifier) +
                    " was already dispatched.") {}

RequestCanceled::RequestCanceled() : runtime_error("Request was cancled") {}

ResponseHandler::ResponseHandler()
    : logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {}

ResponseHandler::~ResponseHandler() {
  LoggerRepository::getInstance().deregisterLoger(logger_->getName());
}

void ResponseHandler::cancelRequest(uint64_t response_identifier) {
  logger_->log(SeverityLevel::TRACE, "Canceling request with id {}",
               response_identifier);
  auto response_promise = response_promises_.find(response_identifier);
  if (response_promise != response_promises_.end()) {
    response_promise->second.set_exception(
        make_exception_ptr(RequestCanceled()));
    response_promises_.erase(response_promise);
  } else {
    logger_->log(SeverityLevel::WARNNING, "No request with id {} was found",
                 response_identifier);
  }
}

void ResponseHandler::cleanup(vector<uint64_t> response_identifiers) {
  logger_->log(SeverityLevel::TRACE, "Cleaning up unhandled requests.");
  for (auto identifier : response_identifiers) {
    cancelRequest(identifier);
  }
}

future<ClientResponsePtr>
ResponseHandler::request(uint64_t response_identifier) {
  logger_->log(SeverityLevel::TRACE, "Requesting a new request with id {}",
               response_identifier);
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

bool ResponseHandler::exists(uint64_t identifier) {
  return response_promises_.find(identifier) != response_promises_.end()
             ? true
             : false;
}

void ResponseHandler::respond(uint64_t response_identifier,
                              ClientResponsePtr response) {
  logger_->log(SeverityLevel::TRACE, "Responding to request with id {}",
               response_identifier);
  auto response_promise = response_promises_.find(response_identifier);
  if (response_promise != response_promises_.end()) {
    response_promise->second.set_value(response);
    response_promises_.erase(response_promise);
  } else {
    throw RequestNotFound(response_identifier);
  }
}
} // namespace LwM2M