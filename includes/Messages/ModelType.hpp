#ifndef __LWM2M_MODEL_TYPE_HPP
#define __LWM2M_MODEL_TYPE_HPP

#include <string>

namespace LwM2M {
enum class LwM2M_Version { V1_0, V1_1, Unrecognized };

std::string toString(LwM2M_Version version);

enum class BindingType { UDP, TCP, SMS, Non_IP, Malformed };

std::string toString(BindingType binding);

} // namespace LwM2M

#endif //__LWM2M_MODEL_TYPE_HPP