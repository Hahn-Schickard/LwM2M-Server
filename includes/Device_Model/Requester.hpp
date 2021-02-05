#ifndef __LWM2M_REQUESTER_INTERFACE_HPP
#define __LWM2M_REQUESTER_INTERFACE_HPP

#include "Message.hpp"

#include <future>

namespace LwM2M {

struct Requester {
  virtual std::future<DataFormat> requestData(MessagePtr message) = 0;
  virtual std::future<bool> requestAction(MessagePtr message) = 0;
};

using RequesterPtr = std::shared_ptr<Requester>;

} // namespace LwM2M

#endif //__LWM2M_REQUESTER_INTERFACE_HPP