#include "ServerRequest_WriteComoposite.hpp"

using namespace std;

namespace LwM2M {
ServerRequest_WriteComoposite::ServerRequest_WriteComoposite(
    string endpoint_address, unsigned int endpoint_port)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,
                                        MessageType::WRITE_COMPOSITE) {
  throw UnsupportedOperation("Write-Composite operation is not supported!");
}
} // namespace LwM2M