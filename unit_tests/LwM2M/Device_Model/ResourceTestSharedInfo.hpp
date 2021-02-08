#ifndef __RESOURCE_TEST_SHARED_INFROMATION_HPP
#define __RESOURCE_TEST_SHARED_INFROMATION_HPP

#include "Executable.hpp"
#include "ReadAndWritable.hpp"
#include "Readable.hpp"
#include "TestRequester.hpp"
#include "Writable.hpp"

#include <chrono>
#include <memory>
#include <thread>

namespace LwM2M {
struct ResourceExpectations {
  TestRequesterPtr requester_;
  EndpointPtr endpoint_;
  ObjectInstanceID parent_;
  ResourceDescriptorPtr descriptor_;
  const DataFormat &result_;
};

using ResourceExpectationsPtr = std::shared_ptr<ResourceExpectations>;

template <typename T> struct ResourceTestParameter {
  ResourcePtr<T> tested_;
  ResourceExpectationsPtr expected_;

  ResourceTestParameter(ResourcePtr<T> tested, ResourceExpectationsPtr expected)
      : tested_(tested), expected_(expected) {}
};

template <typename T> struct RespondWithDelay {
  TestRequesterPtr requester_;
  int delay_;
  T response_;

  RespondWithDelay(TestRequesterPtr requester, int delay, T response)
      : requester_(requester), delay_(delay), response_(response) {}

  void operator()() {
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_));
    requester_->respond(response_);
  }
};

template <typename T>
ResourceTestParameter<T>
makeTestParameter(const ResourceExpectations &valid_expectations) {
  ResourcePtr<T> resource;
  switch (valid_expectations.descriptor_->operations_) {
  case OperationsType::READ: {
    resource = std::make_shared<Readable<T>>(
        valid_expectations.requester_, valid_expectations.endpoint_,
        valid_expectations.parent_, valid_expectations.descriptor_);
    break;
  }
  case OperationsType::READ_AND_WRITE: {
    resource = std::make_shared<ReadAndWritable<T>>(
        valid_expectations.requester_, valid_expectations.endpoint_,
        valid_expectations.parent_, valid_expectations.descriptor_);
    break;
  }
  case OperationsType::WRITE: {
    resource = std::make_shared<Writable<T>>(
        valid_expectations.requester_, valid_expectations.endpoint_,
        valid_expectations.parent_, valid_expectations.descriptor_);
    break;
  }
  case OperationsType::EXECUTE: {
    resource = std::make_shared<Executable<T>>(
        valid_expectations.requester_, valid_expectations.endpoint_,
        valid_expectations.parent_, valid_expectations.descriptor_);
    break;
  }
  default: {
    throw std::invalid_argument(
        "Resource operation type is invalid. Unit tests "
        "only support valid resources!");
  }
  }
  auto expectations =
      std::make_shared<ResourceExpectations>(valid_expectations);

  return ResourceTestParameter<T>(resource, expectations);
}

static inline TestRequesterPtr test_requester =
    std::make_shared<TestRequester>();
static inline EndpointPtr tested_endpoint = std::make_shared<Endpoint>();
} // namespace LwM2M

#endif //__RESOURCE_TEST_SHARED_INFROMATION_HPP