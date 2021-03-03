#include "CoAP_ContentTypes.hpp"

using namespace std;

namespace CoAP {

template <> LwM2M::TLV_Pack decode(PayloadPtr payload) {
  if (payload->getContentFormat()->getIndex() !=
      ContentFormatEncodings::LwM2M_TLV::index) {
    throw WrongContentFormatTypeDecoder(
        string(ContentFormatEncodings::LwM2M_TLV::toString()),
        payload->getContentFormat());
  }
  auto buffer = payload->getBytes();

  return LwM2M::TLV_Pack(buffer);
}

template <> PayloadPtr encode(LwM2M::TLV_Pack content) {
  auto index = ContentFormatEncodings::LwM2M_TLV::index;
  return make_shared<Payload>(index, content.getBytes());
}
} // namespace CoAP
