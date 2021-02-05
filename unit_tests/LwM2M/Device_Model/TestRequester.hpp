#ifndef __LWM2M_REQUESTER_TEST_IMPLEMENTATION_HPP
#define __LWM2M_REQUESTER_TEST_IMPLEMENTATION_HPP

#include "Requester.hpp"

namespace LwM2M {
class TestRequester : public Requester {
  std::promise<DataFormat> data_promise_;
  std::promise<bool> action_promise_;

public:
  std::future<DataFormat> requestData(MessagePtr /*message*/) override final {
    return data_promise_.get_future();
  }

  std::future<bool> requestAction(MessagePtr /*message*/) override final {
    return action_promise_.get_future();
  }

  bool respond(const DataFormat &result) {
    bool success = false;
    try {
      data_promise_.set_value(result);
      success = true;
    } catch (std::exception &ex) {
      success = false;
    }
    return success;
  }

  bool respond(bool enacted) {
    bool success = false;
    try {
      action_promise_.set_value(enacted);
      success = true;
    } catch (std::exception &ex) {
      success = false;
    }
    return success;
  }
};

using TestRequesterPtr = std::shared_ptr<TestRequester>;
} // namespace LwM2M

#endif //__LWM2M_REQUESTER_TEST_IMPLEMENTATION_HPP