#ifndef __LWM2M_REQUESTER_TEST_IMPLEMENTATION_HPP
#define __LWM2M_REQUESTER_TEST_IMPLEMENTATION_HPP

#include "Requester.hpp"

#include <exception>
#include <optional>
#include <variant>

namespace LwM2M {
class TestRequester : public Requester {
  using DataPromise = std::promise<DataFormatPtr>;
  using MultiDataPromise = std::promise<TargetContentVector>;
  using ActionPromise = std::promise<bool>;

  std::optional<DataPromise> data_promise_;
  std::optional<MultiDataPromise> multi_data_promise_;
  std::optional<ActionPromise> action_promise_;

  std::unordered_map<std::size_t, std::function<void(PayloadDataPtr)>>
      observed_elements_;

public:
  std::future<DataFormatPtr>
  requestData(DeviceManagementRequestPtr /*message*/) override final {
    if (!data_promise_) {
      DataPromise data_promise;
      auto result = data_promise.get_future();
      data_promise_ = std::move(data_promise);
      return result;
    } else {
      throw std::runtime_error(
          "Could not despatch requestData call. TestRequester "
          "does not support multiple calls. "
          "Respond to the first call first!");
    }
  }

  std::future<TargetContentVector> requestMultiTargetData(
      DeviceManagementRequestPtr /*message*/) override final {
    if (!multi_data_promise_) {
      MultiDataPromise multi_data_promise;
      auto result = multi_data_promise.get_future();
      multi_data_promise_ = std::move(multi_data_promise);
      return result;
    } else {
      throw std::runtime_error(
          "Could not despatch requestMultiTargetData call. TestRequester "
          "does not support multiple calls. "
          "Respond to the first call first!");
    }
  }

  std::future<bool>
  requestAction(DeviceManagementRequestPtr /*message*/) override final {
    if (!action_promise_) {
      ActionPromise action_promise;
      auto result = action_promise.get_future();
      action_promise_ = std::move(action_promise);
      return result;
    } else {
      throw std::runtime_error(
          "Could not despatch requestAction call. TestRequester "
          "does not support multiple calls. "
          "Respond to the first call first!");
    }
  }

  void cancelRequest(ServerRequestPtr /*message*/) override final {
    if (data_promise_) {
      auto data_promise = std::move(data_promise_.value());
      data_promise_ = std::nullopt;
      data_promise.set_exception(std::make_exception_ptr(
          std::runtime_error("requestData call was canceled")));
    }
    if (multi_data_promise_) {
      auto multi_data_promise = std::move(multi_data_promise_.value());
      multi_data_promise_ = std::nullopt;
      multi_data_promise.set_exception(std::make_exception_ptr(
          std::runtime_error("requestMultiTargetData call was canceled")));
    }
    if (action_promise_) {
      auto action_promise = std::move(action_promise_.value());
      action_promise_ = std::nullopt;
      action_promise.set_exception(std::make_exception_ptr(
          std::runtime_error("requestAction call was canceled")));
    }
  }

  size_t requestObservation(
      std::function<void(PayloadDataPtr)> notify_cb,
      InformationReportingRequestPtr /*message*/) override final {
    auto id = observed_elements_.size();
    observed_elements_.emplace(id, notify_cb);
    return id;
  }

  void
  cancelObservation(size_t observer_id,
                    InformationReportingRequestPtr /*message*/) override final {
    auto observer = observed_elements_.find(observer_id);
    if (observer != observed_elements_.end()) {
      observed_elements_.erase(observer);
    }
  }

  bool respond(const DataFormat &result) {
    bool success = false;
    if (data_promise_) {
      try {
        auto data_promise = std::move(data_promise_.value());
        data_promise_ = std::nullopt;
        data_promise.set_value(std::make_shared<DataFormat>(result));
        success = true;
      } catch (std::exception &ex) {
        success = false;
      }
    }
    return success;
  }

  bool respond(TargetContentVector result) {
    bool success = false;
    if (multi_data_promise_) {
      try {
        auto multi_data_promise = std::move(multi_data_promise_.value());
        multi_data_promise_ = std::nullopt;
        multi_data_promise.set_value(result);
        success = true;
      } catch (std::exception &ex) {
        success = false;
      }
    }
    return success;
  }

  bool respond(bool enacted) {
    bool success = false;
    if (action_promise_) {
      try {
        auto action_promise = std::move(action_promise_.value());
        action_promise_ = std::nullopt;
        action_promise.set_value(enacted);
        success = true;
      } catch (std::exception &ex) {
        success = false;
      }
    }
    return success;
  }

  bool notify(size_t observer_id, PayloadDataPtr value) {
    bool success = false;
    auto callback = observed_elements_.find(observer_id);
    if (callback != observed_elements_.end()) {
      try {
        (callback->second)(value);
        success = true;
      } catch (std::exception &ex) {
        success = false;
      }
    }
    return success;
  }
};

using TestRequesterPtr = std::shared_ptr<TestRequester>;
} // namespace LwM2M

#endif //__LWM2M_REQUESTER_TEST_IMPLEMENTATION_HPP