#ifndef __LWM2M_MODEL_TYPE_HPP
#define __LWM2M_MODEL_TYPE_HPP

namespace LwM2M {
enum class LwM2M_Version { V1_0, V1_1, UNRECOGNIZED };
enum class BindingType { UDP, TCP, SMS, NON_IP, MALFORMED };
} // namespace LwM2M

#endif //__LWM2M_MODEL_TYPE_HPP