#ifndef __LWM2M_READABLE_RESOURCE_HPP
#define __LWM2M_READABLE_RESOURCE_HPP

#include "Observable.hpp"
#include "ResourceDescriptor.hpp"

namespace LwM2M {

class Readable : public Observable {
  std::future<DataVariant> asyncDataRequest(DeviceManagementRequestPtr message);

public:
  Readable(ResourceDescriptorPtr descriptor,
      Observable::ExceptionHandler handler, RequesterPtr requester,
      EndpointPtr endpoint, ElementID id);

  std::future<DataVariant> read();
};

using ReadablePtr = std::shared_ptr<Readable>;
} // namespace LwM2M

#endif //__LWM2M_READABLE_RESOURCE_HPP