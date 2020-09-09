#include "ServerRequest_ReadComoposite.hpp"

using namespace std;

namespace LwM2M {
ServerRequest_ReadComoposite::ServerRequest_ReadComoposite(
    string endpoint_address, unsigned int endpoint_port)
    : DeviceManagmentMessage(endpoint_address, endpoint_port,
                             MessageType::READ_COMPOSITE) {
  throw UnsupportedOperation("Read-Composite operation is not supported!");
}
} // namespace LwM2M