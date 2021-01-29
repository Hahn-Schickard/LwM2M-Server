#include "Discover.hpp"

using namespace std;

namespace LwM2M {

DiscoverRequest::DiscoverRequest(EndpointPtr endpoint,
                                 EelmentIdVariant target_id)
    : DeviceManagmentRequest(endpoint, MessageType::DISCOVER),
      target_id_(target_id) {}

string DiscoverRequest::name() { return "DiscoverRequest"; }

DiscoverResponse::DiscoverResponse(EndpointPtr endpoint,
                                   ResponseCode response_code,
                                   DataFormatPtr content)
    : DeviceManagmentResponse(
          endpoint, MessageType::DISCOVER,
          unordered_set<ResponseCode>{
              ResponseCode::CONTENT, ResponseCode::BAD_REQUEST,
              ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
              ResponseCode::METHOD_NOT_ALLOWED},
          response_code),
      content_(content) {
  checkResponseCode(response_code);
}

string DiscoverResponse::name() { return "DiscoverResponse"; }

} // namespace LwM2M
