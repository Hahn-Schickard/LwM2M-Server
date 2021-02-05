#include "Readable.hpp"
#include "TestRequester.hpp"

#include "gtest/gtest.h"

#include <chrono>
#include <thread>

using namespace std;
using namespace LwM2M;

class ResourceTest : public testing::Test {
protected:
  void SetUp() override {
    requester = make_shared<TestRequester>();
    auto endpoint = make_shared<Endpoint>("0.0.0.0", 111111);
    auto descriptor = make_shared<ResourceDescriptor>(
        0, "test", OperationsType::READ, true, true, DataType::BOOLEAN, "", "");
    resource = make_shared<Readable<bool>>(requester, endpoint,
                                           ObjectInstanceID(0, 0), descriptor);
  }

  TestRequesterPtr requester;
  ReadablePtr<bool> resource;
};

struct RespondWithDelay {
  TestRequesterPtr requester_;
  int delay_;
  const DataFormat &response_;

  RespondWithDelay(TestRequesterPtr requester, int delay,
                   const DataFormat &response)
      : requester_(requester), delay_(delay), response_(response) {}

  void operator()() {
    std::this_thread::sleep_for(chrono::milliseconds(delay_));
    requester_->respond(response_);
  }
};

TEST_F(ResourceTest, canReadValue) {
  auto result = resource->read();
  int delay = 1;

  auto expected = DataFormat(true, DataType::BOOLEAN);

  async(launch::async, RespondWithDelay(requester, delay, expected));

  auto status = result.wait_for(chrono::milliseconds(delay + 1));
  if (status == std::future_status::timeout) {
    FAIL() << "Response was not sent in time" << endl;
  } else {
    EXPECT_EQ(result.get(), expected.get<bool>());
  }
}