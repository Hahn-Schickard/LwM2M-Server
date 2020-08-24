#include "ReadComoposite_Request.hpp"

using namespace std;

namespace LwM2M {
ReadComoposite_Request::ReadComoposite_Request(string endpoint_address,
                                               unsigned int endpoint_port)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,
                                        MessageType::READ_COMPOSITE) {
  throw UnsupportedOperation("Read-Composite operation is not supported!");
}
} // namespace LwM2M