#include "Notify.hpp"

using namespace std;

namespace LwM2M {
ValueUpdated::ValueUpdated(EndpointPtr endpoint, // NOLINT
    DataFormatPtr content)
    : ClientNotification(endpoint, // NOLINT
          MessageType::NOTIFY, InterfaceType::INFORMATION_REPORTING,
          make_shared<Payload>(content)) {}

string ValueUpdated::name() { return "ValueUpdated"; }
} // namespace LwM2M
