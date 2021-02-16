#include "Execute.hpp"
#include "Variant_Visitor.hpp"

using namespace std;

namespace LwM2M {

ExecuteRequest::ExecuteRequest(EndpointPtr endpoint, ResourceID target)
    : ServerRequest(endpoint, MessageType::EXECUTE,
                    InterfaceType::DEVICE_MANAGMENT,
                    make_shared<Payload>(ElmentIdVariant(target))) {}

ExecuteRequest::ExecuteRequest(EndpointPtr endpoint, ResourceID target,
                               string arguments)
    : ServerRequest(endpoint, MessageType::EXECUTE,
                    InterfaceType::DEVICE_MANAGMENT,
                    make_shared<Payload>(make_pair(
                        ElmentIdVariant(target),
                        make_shared<DataFormat>(DataVariant(arguments),
                                                DataType::STRING)))) {}

string ExecuteRequest::name() { return "ExecuteRequest"; }

ExecuteResponse::ExecuteResponse(EndpointPtr endpoint,
                                 ResponseCode response_code)
    : ClientResponse(endpoint, MessageType::EXECUTE,
                     InterfaceType::DEVICE_MANAGMENT,
                     unordered_set<ResponseCode>{
                         ResponseCode::CHANGED, ResponseCode::BAD_REQUEST,
                         ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
                         ResponseCode::METHOD_NOT_ALLOWED},
                     response_code) {
  checkResponseCode(response_code);
}

string ExecuteResponse::name() { return "ExecuteResponse"; }

} // namespace LwM2M
