#include "RequestsManagerInterface.hpp"

#include <algorithm>

#define ERROR_CODES_VALUE 0x80

using namespace std;

namespace LwM2M {
ResponseReturnedAnErrorCode::ResponseReturnedAnErrorCode(
    ClientResponsePtr response, ServerRequestPtr request)
    : runtime_error(response->name() + " returned " +
                    toString(response->response_code_) + " for " +
                    request->name()) {}

RequestsManagerInterface::RequestsManagerInterface(
    ResponseHandlerPtr requests_manager)
    : requests_manager_(requests_manager) {}

RequestsManagerInterface::~RequestsManagerInterface() {
  requests_manager_->cleanup(dispatched_);
}

ClientResponsePtr
RequestsManagerInterface::dispatchAndGet(ServerRequestPtr request) {
  auto id = dispatch(request);
  auto result_future = requests_manager_->request(id);
  auto result = result_future.get();
  dispatched_.erase(remove(dispatched_.begin(), dispatched_.end(), id),
                    dispatched_.end());
  return move(result);
}

future<TargetContentVector>
RequestsManagerInterface::handleResponseWithTargetContentVector(
    ServerRequestPtr message) {
  return async(launch::async,
               [&](ServerRequestPtr msg) -> TargetContentVector {
                 auto result = dispatchAndGet(msg);
                 if (result->response_code_ == ResponseCode::CONTENT &&
                     result->payload_) {
                   return std::get<TargetContentVector>(
                       result->payload_->data_);
                 } else {
                   throw ResponseReturnedAnErrorCode(result, msg);
                 }
               },
               message);
}

future<DataFormat> RequestsManagerInterface::handleResponseWithDataFormat(
    ServerRequestPtr message) {
  return async(launch::async,
               [&](ServerRequestPtr msg) -> DataFormat {
                 auto result = dispatchAndGet(msg);
                 if (result->response_code_ == ResponseCode::CONTENT &&
                     result->payload_) {
                   return std::get<DataFormat>(result->payload_->data_);
                 } else {
                   throw ResponseReturnedAnErrorCode(result, msg);
                 }
               },
               message);
}

future<bool>
RequestsManagerInterface::handleResponse(ServerRequestPtr message) {
  return async(launch::async,
               [&](ServerRequestPtr msg) -> bool {
                 auto result = dispatchAndGet(msg);
                 return static_cast<uint8_t>(result->response_code_) <
                                ERROR_CODES_VALUE
                            ? true
                            : false;
               },
               message);
}

future<TargetContentVector>
RequestsManagerInterface::requestMultiTargetData(ServerRequestPtr message) {
  return handleResponseWithTargetContentVector(message);
}

future<DataFormat>
RequestsManagerInterface::requestData(ServerRequestPtr message) {
  return handleResponseWithDataFormat(message);
}

future<bool> RequestsManagerInterface::requestAction(ServerRequestPtr message) {
  return handleResponse(message);
}
} // namespace LwM2M
