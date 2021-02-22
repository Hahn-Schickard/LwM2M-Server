#ifndef __LWM2M_COAP_CONTENT_FORMAT_TYPES_HPP
#define __LWM2M_COAP_CONTENT_FORMAT_TYPES_HPP

#include "CoAP/ContentFormatType.hpp"
#include "CoAP/SupportedContentFormats.hpp"
#include "TLV.hpp"

namespace CoAP {
namespace ContentFormatEncodings {
/**
 * @brief Supported CoAP Content-Format definitions, encoding id's taken from
 * CoAP Content-Formats table at:
 * https://www.iana.org/assignments/core-parameters/core-parameters.xhtml
 */
typedef ContentFormatEncoding<11542, decltype("LwM2M TLV"_name)> LwM2M_TLV;
typedef ContentFormatEncoding<11543, decltype("LwM2M CBOR"_name)> LwM2M_CBOR;
typedef ContentFormatEncoding<11544, decltype("LwM2M JSON"_name)> LwM2M_JSON;
} // namespace ContentFormatEncodings

template <> LwM2M::TLV_Pack decode(PayloadPtr payload);
template <> PayloadPtr encode(LwM2M::TLV_Pack content);
} // namespace CoAP

#endif //__LWM2M_COAP_CONTENT_FORMAT_TYPES_HPP