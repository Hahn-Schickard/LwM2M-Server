#ifndef __LWM2M_READ_AND_WRITABLE_RESOURCE_HPP
#define __LWM2M_READ_AND_WRITABLE_RESOURCE_HPP

#include "Readable.hpp"
#include "Writable.hpp"

namespace LwM2M {
class ReadAndWritable : public Readable, public Writable {
public:
  ReadAndWritable(ResourceDescriptorPtr descriptor,
      Observable::ExceptionHandler handler, RequesterPtr requester,
      EndpointPtr endpoint, ElementID id);
};

using ReadAndWritablePtr = std::shared_ptr<ReadAndWritable>;
} // namespace LwM2M

#endif //__LWM2M_READ_AND_WRITABLE_RESOURCE_HPP