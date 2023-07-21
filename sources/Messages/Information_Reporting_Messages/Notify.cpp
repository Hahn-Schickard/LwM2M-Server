#include "Notify.hpp"

using namespace std;

namespace LwM2M {
ValueUpdated::ValueUpdated(EndpointPtr endpoint, // NOLINT
    DataFormatPtr content) // NOLINT
    : ClientNotification(endpoint, // NOLINT
          MessageType::NOTIFY, InterfaceType::INFORMATION_REPORTING,
          make_shared<Payload>(content)) {}

string ValueUpdated::name() const { return "ValueUpdated"; }
} // namespace LwM2M
