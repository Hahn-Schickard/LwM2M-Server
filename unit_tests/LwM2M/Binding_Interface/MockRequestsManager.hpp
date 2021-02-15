#ifndef __LWM2M_UNIT_TEST_MOCK_DISPATCHER_HPP
#define __LWM2M_UNIT_TEST_MOCK_DISPATCHER_HPP

#include "RequestsManagerInterface.hpp"

#include "gmock/gmock.h"

namespace LwM2M {
struct MockRequestsManager : public RequestsManagerInterface {
  MockRequestsManager()
      : RequestsManagerInterface(std::make_shared<ResponseHandler>()) {}

  MockRequestsManager(ResponseHandlerPtr manager)
      : RequestsManagerInterface(manager) {}

  MOCK_METHOD(uint64_t, dispatch, (ServerRequestPtr), (override));

  void doCleanup() { cleanup(); }
};

using MockRequestsManagerPtr = std::shared_ptr<MockRequestsManager>;
} // namespace LwM2M

#endif //__LWM2M_UNIT_TEST_MOCK_DISPATCHER_HPP