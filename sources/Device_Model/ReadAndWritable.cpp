#include "ReadAndWritable.hpp"

namespace LwM2M {
ReadAndWritable::ReadAndWritable(Observable::ExceptionHandler handler,
    RequesterInterfaceFacadePtr requester, EndpointPtr endpoint, ElementID id,
    DataType data_type)
    : Readable(handler, requester, endpoint, id, data_type),
      Writable(requester, endpoint, id) {}
} // namespace LwM2M