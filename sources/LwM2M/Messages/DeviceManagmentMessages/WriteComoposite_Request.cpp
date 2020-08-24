#include "WriteComoposite_Request.hpp"

using namespace std;

namespace LwM2M {
WriteComoposite_Request::WriteComoposite_Request(string endpoint_address,
                                                 unsigned int endpoint_port)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,
                                        MessageType::WRITE_COMPOSITE) {
  throw UnsupportedOperation("Write-Composite operation is not supported!");
}
} // namespace LwM2M