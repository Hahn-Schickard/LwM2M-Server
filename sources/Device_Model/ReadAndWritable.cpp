#include "ReadAndWritable.hpp"

namespace LwM2M {
ReadAndWritable::ReadAndWritable(Observable::ExceptionHandler handler, // NOLINT
    ObservableInterfacePtr observe_requester, // NOLINT
    ReadableInterfacePtr read_requester, // NOLINT
    ExecutableInterfacePtr write_requester, // NOLINT
    EndpointPtr endpoint, // NOLINT
    ElementID id, DataType data_type)
    : Readable(handler, // NOLINT
          observe_requester, read_requester, endpoint, // NOLINT
          id, data_type),
      Writable(write_requester, endpoint, id) {} // NOLINT
} // namespace LwM2M