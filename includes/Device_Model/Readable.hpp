#ifndef __LWM2M_READABLE_RESOURCE_HPP
#define __LWM2M_READABLE_RESOURCE_HPP

#include "Observable.hpp"
#include "ReadableInterface.hpp"
#include "ResourceDescriptor.hpp"

namespace LwM2M {

class Readable : public Observable {
  std::future<DataVariant> asyncDataRequest(DeviceManagementRequestPtr message);

public:
  Readable(Observable::ExceptionHandler handler,
      ObservableInterfacePtr observe_requester,
      ReadableInterfacePtr read_requester, EndpointPtr endpoint, ElementID id,
      DataType data_type);

  std::future<DataVariant> read();

private:
  ReadableInterfacePtr requester_;
};

using ReadablePtr = std::shared_ptr<Readable>;
} // namespace LwM2M

#endif //__LWM2M_READABLE_RESOURCE_HPP