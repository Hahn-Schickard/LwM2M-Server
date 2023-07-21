#ifndef __LWM2M_TESTS_MOCK_EXCEPTION_HANDLER_HPP
#define __LWM2M_TESTS_MOCK_EXCEPTION_HANDLER_HPP
#include "gmock/gmock.h"

#include <exception>
#include <memory>

struct ExceptionHandlerInterface {
  virtual ~ExceptionHandlerInterface() {}

  virtual void handleDeviceException(
      const std::exception_ptr& /* exception_ptr */) {
    throw std::runtime_error(
        "Called base ExceptionHandlerInterface implementation");
  }
};

struct MockExceptionHandler : public ExceptionHandlerInterface {
  MOCK_METHOD(
      void, handleDeviceException, (const std::exception_ptr&), (override));
};

using MockExceptionHandlerPtr = std::shared_ptr<MockExceptionHandler>;
#endif //__LWM2M_TESTS_MOCK_EXCEPTION_HANDLER_HPP
