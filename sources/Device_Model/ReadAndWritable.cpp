#include "ReadAndWritable.hpp"

namespace LwM2M {
ReadAndWritable::ReadAndWritable(Observable::ExceptionHandler handler,
    ObservableInterfacePtr observe_requester,
    ReadableInterfacePtr read_requester, ExecutableInterfacePtr write_requester,
    EndpointPtr endpoint, ElementID id, DataType data_type)
    : Readable(
          handler, observe_requester, read_requester, endpoint, id, data_type),
      Writable(write_requester, endpoint, id) {}
} // namespace LwM2M