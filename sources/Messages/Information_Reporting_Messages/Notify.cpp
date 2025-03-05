#include "Notify.hpp"

using namespace std;

namespace LwM2M {
ValueUpdated::ValueUpdated(EndpointPtr endpoint, // NOLINT
    DataFormatPtr content) // NOLINT
    : ClientNotification(endpoint, // NOLINT
          MessageType::Notify, InterfaceType::Information_Reporting,
          make_shared<Payload>(content)) {}

string ValueUpdated::name() const { return "ValueUpdated"; }
} // namespace LwM2M
