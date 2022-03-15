#ifndef __LWM2M_COAP_BINDING_REGISTRATION_REQUESTS_BUILDER_HPP
#define __LWM2M_COAP_BINDING_REGISTRATION_REQUESTS_BUILDER_HPP

#include "CoAPS4Cpp/Message.hpp"
#include "Deregister.hpp"
#include "Register.hpp"
#include "Update.hpp"

#include <stdexcept>

namespace LwM2M {
RegisterRequestPtr buildRegisterRequest(CoAP::MessagePtr message);

UpdateRequestPtr buildUpdateRequest(CoAP::MessagePtr message);

DeregisterRequestPtr buildDeregisterRequest(CoAP::MessagePtr message);

} // namespace LwM2M

#endif //__LWM2M_COAP_BINDING_REGISTRATION_REQUESTS_BUILDER_HPP
