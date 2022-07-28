#ifndef __LWM2M_READABLE_RESOURCE_HPP
#define __LWM2M_READABLE_RESOURCE_HPP

#include "Nonempty_Pointer/NonemptyPtr.hpp"

#include "Observable.hpp"
#include "ReadableInterface.hpp"
#include "ResourceDescriptor.hpp"

namespace LwM2M {

struct Readable : public Observable {
  using Result = RequestResult<DataVariant>;

  Readable(Observable::ExceptionHandler handler,
      ObservableInterfacePtr observe_requester,
      ReadableInterfacePtr read_requester, EndpointPtr endpoint, ElementID id,
      DataType data_type);

  Result read();

private:
  ReadableInterfacePtr requester_;
};

using ReadableSharedPtr = std::shared_ptr<Readable>;
using ReadablePtr = NonemptyPointer::NonemptyPtr<ReadableSharedPtr>;
} // namespace LwM2M

#endif //__LWM2M_READABLE_RESOURCE_HPP