#ifndef __LWM2M_UNIT_TEST_MOCK_DISPATCHER_HPP
#define __LWM2M_UNIT_TEST_MOCK_DISPATCHER_HPP

#include "Dispatcher.hpp"

#include "gmock/gmock.h"

namespace LwM2M {
struct MockDispatcher : DispatcherInterface {
  MockDispatcher() : DispatcherInterface(std::make_shared<RequestsManager>()) {}

  MockDispatcher(RequestsManagerPtr manager) : DispatcherInterface(manager) {}

  MOCK_METHOD(uint64_t, dispatch, (ServerRequestPtr), (override));
};

using MockDispatcherPtr = std::shared_ptr<MockDispatcher>;
} // namespace LwM2M

#endif //__LWM2M_UNIT_TEST_MOCK_DISPATCHER_HPP