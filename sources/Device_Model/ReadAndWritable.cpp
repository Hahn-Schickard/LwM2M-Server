#include "ReadAndWritable.hpp"

namespace LwM2M {
ReadAndWritable::ReadAndWritable(ResourceDescriptorPtr descriptor,
    Observable::ExceptionHandler handler, RequesterPtr requester,
    EndpointPtr endpoint, ElementID id)
    : Readable(descriptor, handler, requester, endpoint, id),
      Writable(descriptor, requester, endpoint, id) {}
} // namespace LwM2M