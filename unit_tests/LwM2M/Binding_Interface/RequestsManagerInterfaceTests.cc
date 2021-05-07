#include "MockRequestsManager.hpp"
#include "Read.hpp"
#include "ReadComposite.hpp"
#include "Write.hpp"

#include "gtest/gtest.h"

#include <chrono>
#include <iostream>
#include <thread>

using namespace LwM2M;
using namespace std;
using ::testing::Return;

class RequestsManagerInterfaceTests : public ::testing::Test {
protected:
  void SetUp() override {
    response_handler_ = make_shared<ResponseHandler>();
    requests_manager_ = make_shared<MockRequestsManager>(response_handler_);
    requester_ = requests_manager_;
  }

  ResponseHandlerPtr response_handler_;
  MockRequestsManagerPtr requests_manager_;
  RequesterPtr requester_;
};

struct RespondWithContent {
  void operator()(ResponseHandlerPtr responder, EndpointPtr target,
                  uint64_t identifier, PayloadPtr content) {
    auto response =
        make_shared<ClientResponse>(target, ResponseCode::CONTENT, content);
    responder->respond(identifier, move(response));
  }
};

struct RespondWithCode {
  void operator()(ResponseHandlerPtr responder, EndpointPtr target,
                  uint64_t identifier, ResponseCode code) {
    auto response = make_shared<ClientResponse>(target, code);
    responder->respond(identifier, move(response));
  }
};

TEST_F(RequestsManagerInterfaceTests, returnsDataFormatOnRequestData) {
  auto endpoint = make_shared<Endpoint>("1.1.1.1.", 10);
  ServerRequestPtr request = make_shared<ReadRequest>(endpoint);
  uint64_t request_identifier = 1234;

  EXPECT_CALL(*requests_manager_, dispatch(request))
      .Times(1)
      .WillOnce(Return(request_identifier));

  try {
    auto result_future = requester_->requestData(request);
    this_thread::sleep_for(1ms);

    auto data = make_shared<DataFormat>(DataVariant((bool)true));

    auto response_test_result =
        async(launch::async, RespondWithContent(), response_handler_, endpoint,
              request_identifier, make_shared<Payload>(data));
    if (response_test_result.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Response handling test timedout." << endl;
    }

    if (result_future.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Could not get the result in a timely manner" << endl;
    } else {
      auto result = result_future.get();
      EXPECT_EQ(*result.get(), *data.get());
    }
  } catch (exception &ex) {
    FAIL() << "Caught an exception while requesting data. Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RequestsManagerInterfaceTests,
       throwsResponseReturnedAnErrorCodeOnRequestData) {
  auto endpoint = make_shared<Endpoint>("1.1.1.1.", 10);
  ServerRequestPtr request = make_shared<ReadRequest>(endpoint);
  uint64_t request_identifier = 1234;

  EXPECT_CALL(*requests_manager_, dispatch(request))
      .Times(1)
      .WillOnce(Return(request_identifier));

  try {
    auto result_future = requester_->requestData(request);
    this_thread::sleep_for(1ms);

    auto response_test_result =
        async(launch::async, RespondWithCode(), response_handler_, endpoint,
              request_identifier, ResponseCode::NOT_FOUND);
    if (response_test_result.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Response handling test timedout." << endl;
    }

    if (result_future.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Could not get the result in a timely manner" << endl;
    } else {
      EXPECT_THROW({ result_future.get(); }, ResponseReturnedAnErrorCode);
    }
  } catch (exception &ex) {
    FAIL() << "Caught an unhandled exception while requesting data. Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RequestsManagerInterfaceTests, throwsRequestCanceledOnRequestData) {
  auto endpoint = make_shared<Endpoint>("1.1.1.1.", 10);
  ServerRequestPtr request = make_shared<ReadRequest>(endpoint);
  uint64_t request_identifier = 1234;

  EXPECT_CALL(*requests_manager_, dispatch(request))
      .Times(1)
      .WillOnce(Return(request_identifier));

  try {
    auto result_future = requester_->requestData(request);
    this_thread::sleep_for(1ms);

    auto cleanup_test_result =
        async(launch::async,
              [](MockRequestsManagerPtr requests_manager) {
                requests_manager->doCleanup();
              },
              requests_manager_);
    if (cleanup_test_result.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Response cleanup handling test timedout." << endl;
    }

    if (result_future.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Could not get the result in a timely manner" << endl;
    } else {
      EXPECT_THROW({ result_future.get(); }, RequestCanceled);
    }
  } catch (exception &ex) {
    FAIL() << "Caught an unhandled exception while requesting data. Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RequestsManagerInterfaceTests,
       throwsRequestAlreadyDispatchedOnRequestData) {
  auto endpoint = make_shared<Endpoint>("1.1.1.1.", 10);
  ServerRequestPtr request = make_shared<ReadRequest>(endpoint);
  uint64_t request_identifier = 1234;

  EXPECT_CALL(*requests_manager_, dispatch(request))
      .Times(2)
      .WillRepeatedly(Return(request_identifier));

  try {
    auto result_future = requester_->requestData(request);
    this_thread::sleep_for(1ms);

    auto duplicate_test_result =
        async(launch::async,
              [](ServerRequestPtr msg, RequesterPtr requester) {
                EXPECT_THROW(
                    {
                      auto duplicate_future = requester->requestAction(msg);
                      duplicate_future.get();
                    },
                    RequestAlreadyDispatched);
              },
              request, requester_);
    if (duplicate_test_result.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Duplicate message handling test timedout." << endl;
    }

    auto response_test_result =
        async(launch::async, RespondWithCode(), response_handler_, endpoint,
              request_identifier, ResponseCode::NOT_FOUND);
    if (response_test_result.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Response handling test timedout." << endl;
    }

    if (result_future.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Could not get the result in a timely manner" << endl;
    } else {
      EXPECT_THROW({ result_future.get(); }, ResponseReturnedAnErrorCode);
    }
  } catch (exception &ex) {
    FAIL() << "Caught an unhandled exception while requesting data. Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RequestsManagerInterfaceTests,
       returnsMulitTargetOnRequestMultiTargetData) {
  auto endpoint = make_shared<Endpoint>("1.1.1.1.", 10);
  vector<ElmentIdVariant> targets{ElmentIdVariant(ObjectID(1)),
                                  ElmentIdVariant(ObjectID(2))};
  ServerRequestPtr request =
      make_shared<ReadCompositeRequest>(endpoint, targets);
  uint64_t request_identifier = 1234;

  EXPECT_CALL(*requests_manager_, dispatch(request))
      .Times(1)
      .WillOnce(Return(request_identifier));

  try {
    auto result_future = requester_->requestMultiTargetData(request);
    this_thread::sleep_for(1ms);

    vector<TargetContent> data;
    data.emplace_back(ObjectID(1),
                      make_shared<DataFormat>(DataVariant((bool)true)));
    data.emplace_back(ObjectID(2),
                      make_shared<DataFormat>(DataVariant((double)20.2)));
    auto response_test_result =
        async(launch::async, RespondWithContent(), response_handler_, endpoint,
              request_identifier, make_shared<Payload>(data));
    if (response_test_result.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Response handling test timedout." << endl;
    }

    if (result_future.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Could not get the result in a timely manner" << endl;
    } else {
      auto result = result_future.get();
      for (uint8_t i = 0; i < data.size(); i++) {
        auto result_target = result.at(i).first;
        auto data_target = data.at(i).first;
        EXPECT_EQ(result_target, data_target);

        auto result_content = result.at(i).second;
        auto data_content = result.at(i).second;
        EXPECT_EQ(result_content, data_content);
      }
    }
  } catch (exception &ex) {
    FAIL() << "Caught an exception while requesting multi target data. "
              "Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RequestsManagerInterfaceTests,
       throwsResponseReturnedAnErrorCodeOnRequestMultiTargetData) {
  auto endpoint = make_shared<Endpoint>("1.1.1.1.", 10);
  vector<ElmentIdVariant> targets{ElmentIdVariant(ObjectID(1)),
                                  ElmentIdVariant(ObjectID(2))};
  ServerRequestPtr request =
      make_shared<ReadCompositeRequest>(endpoint, targets);
  uint64_t request_identifier = 1234;

  EXPECT_CALL(*requests_manager_, dispatch(request))
      .Times(1)
      .WillOnce(Return(request_identifier));

  try {
    auto result_future = requester_->requestMultiTargetData(request);
    this_thread::sleep_for(1ms);

    auto response_test_result =
        async(launch::async, RespondWithCode(), response_handler_, endpoint,
              request_identifier, ResponseCode::UNAUTHORIZED);
    if (response_test_result.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Response handling test timedout." << endl;
    }

    if (result_future.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Could not get the result in a timely manner" << endl;
    } else {
      EXPECT_THROW({ result_future.get(); }, ResponseReturnedAnErrorCode);
    }
  } catch (exception &ex) {
    FAIL()
        << "Caught an unhandled exception while requesting multi target data. "
           "Exception: "
        << ex.what() << endl;
  }
}

TEST_F(RequestsManagerInterfaceTests,
       throwsRequestCanceledOnRequestMultiTargetData) {
  auto endpoint = make_shared<Endpoint>("1.1.1.1.", 10);
  vector<ElmentIdVariant> targets{ElmentIdVariant(ObjectID(1)),
                                  ElmentIdVariant(ObjectID(2))};
  ServerRequestPtr request =
      make_shared<ReadCompositeRequest>(endpoint, targets);
  uint64_t request_identifier = 1234;

  EXPECT_CALL(*requests_manager_, dispatch(request))
      .Times(1)
      .WillOnce(Return(request_identifier));

  try {
    auto result_future = requester_->requestData(request);
    this_thread::sleep_for(1ms);

    auto cleanup_test_result =
        async(launch::async,
              [](MockRequestsManagerPtr requests_manager) {
                requests_manager->doCleanup();
              },
              requests_manager_);
    if (cleanup_test_result.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Response cleanup handling test timedout." << endl;
    }

    if (result_future.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Could not get the result in a timely manner" << endl;
    } else {
      EXPECT_THROW({ result_future.get(); }, RequestCanceled);
    }
  } catch (exception &ex) {
    FAIL() << "Caught an unhandled exception while requesting data. Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RequestsManagerInterfaceTests,
       throwsRequestAlreadyDispatchedOnRequestMultiTargetData) {
  auto endpoint = make_shared<Endpoint>("1.1.1.1.", 10);
  vector<ElmentIdVariant> targets{ElmentIdVariant(ObjectID(1)),
                                  ElmentIdVariant(ObjectID(2))};
  ServerRequestPtr request =
      make_shared<ReadCompositeRequest>(endpoint, targets);
  uint64_t request_identifier = 1234;

  EXPECT_CALL(*requests_manager_, dispatch(request))
      .Times(2)
      .WillRepeatedly(Return(request_identifier));

  try {
    auto result_future = requester_->requestMultiTargetData(request);
    this_thread::sleep_for(1ms);

    auto duplicate_test_result =
        async(launch::async,
              [](ServerRequestPtr msg, RequesterPtr requester) {
                EXPECT_THROW(
                    {
                      auto duplicate_future = requester->requestAction(msg);
                      duplicate_future.get();
                    },
                    RequestAlreadyDispatched);
              },
              request, requester_);
    if (duplicate_test_result.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Duplicate message handling test timedout." << endl;
    }

    auto response_test_result =
        async(launch::async, RespondWithCode(), response_handler_, endpoint,
              request_identifier, ResponseCode::UNAUTHORIZED);
    if (response_test_result.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Response handling test timedout." << endl;
    }

    if (result_future.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Could not get the result in a timely manner" << endl;
    } else {
      EXPECT_THROW({ result_future.get(); }, ResponseReturnedAnErrorCode);
    }
  } catch (exception &ex) {
    FAIL() << "Caught an unhandled exception while requesting multi target "
              "data. "
              "Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RequestsManagerInterfaceTests, returnsTrueOnRequestAction) {
  auto endpoint = make_shared<Endpoint>("1.1.1.1.", 10);
  ServerRequestPtr request = make_shared<WriteRequest>(endpoint);
  uint64_t request_identifier = 1234;

  EXPECT_CALL(*requests_manager_, dispatch(request))
      .Times(1)
      .WillOnce(Return(request_identifier));

  try {
    auto result_future = requester_->requestAction(request);
    this_thread::sleep_for(1ms);

    auto response_test_result =
        async(launch::async, RespondWithCode(), response_handler_, endpoint,
              request_identifier, ResponseCode::CHANGED);
    if (response_test_result.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Response handling test timedout." << endl;
    }

    if (result_future.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Could not get the result in a timely manner" << endl;
    } else {
      auto result = result_future.get();
      EXPECT_TRUE(result);
    }
  } catch (exception &ex) {
    FAIL() << "Caught an exception while requesting an action. Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RequestsManagerInterfaceTests, returnsFalseOnRequestAction) {
  auto endpoint = make_shared<Endpoint>("1.1.1.1.", 10);
  ServerRequestPtr request = make_shared<WriteRequest>(endpoint);
  uint64_t request_identifier = 1234;

  EXPECT_CALL(*requests_manager_, dispatch(request))
      .Times(1)
      .WillOnce(Return(request_identifier));

  try {
    auto result_future = requester_->requestAction(request);
    this_thread::sleep_for(1ms);

    auto response_test_result =
        async(launch::async, RespondWithCode(), response_handler_, endpoint,
              request_identifier, ResponseCode::BAD_REQUEST);
    if (response_test_result.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Response handling test timedout." << endl;
    }

    if (result_future.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Could not get the result in a timely manner" << endl;
    } else {
      auto result = result_future.get();
      EXPECT_FALSE(result);
    }
  } catch (exception &ex) {
    FAIL() << "Caught an exception while requesting an action. Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RequestsManagerInterfaceTests, throwsRequestCanceledOnRequestAction) {
  auto endpoint = make_shared<Endpoint>("1.1.1.1.", 10);
  ServerRequestPtr request = make_shared<WriteRequest>(endpoint);
  uint64_t request_identifier = 1234;

  EXPECT_CALL(*requests_manager_, dispatch(request))
      .Times(1)
      .WillOnce(Return(request_identifier));

  try {
    auto result_future = requester_->requestAction(request);
    this_thread::sleep_for(1ms);

    auto cleanup_test_result =
        async(launch::async,
              [](MockRequestsManagerPtr requests_manager) {
                requests_manager->doCleanup();
              },
              requests_manager_);
    if (cleanup_test_result.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Response cleanup handling test timedout." << endl;
    }

    if (result_future.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Could not get the result in a timely manner" << endl;
    } else {
      EXPECT_THROW({ result_future.get(); }, RequestCanceled);
    }
  } catch (exception &ex) {
    FAIL() << "Caught an exception while requesting an action. Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RequestsManagerInterfaceTests,
       throwsRequestAlreadyDispatchedOnRequestAction) {
  auto endpoint = make_shared<Endpoint>("1.1.1.1.", 10);
  ServerRequestPtr request = make_shared<WriteRequest>(endpoint);
  uint64_t request_identifier = 1234;

  EXPECT_CALL(*requests_manager_, dispatch(request))
      .Times(2)
      .WillRepeatedly(Return(request_identifier));

  try {
    auto result_future = requester_->requestAction(request);
    this_thread::sleep_for(1ms);

    auto duplicate_test_result =
        async(launch::async,
              [](ServerRequestPtr msg, RequesterPtr requester) {
                EXPECT_THROW(
                    {
                      auto duplicate_future = requester->requestAction(msg);
                      duplicate_future.get();
                    },
                    RequestAlreadyDispatched);
              },
              request, requester_);
    if (duplicate_test_result.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Duplicate message handling test timedout." << endl;
    }

    auto response_test_result =
        async(launch::async, RespondWithCode(), response_handler_, endpoint,
              request_identifier, ResponseCode::BAD_REQUEST);
    if (response_test_result.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Response handling test timedout." << endl;
    }

    if (result_future.wait_for(10ms) == future_status::timeout) {
      FAIL() << "Could not get the result in a timely manner" << endl;
    } else {
      auto result = result_future.get();
      EXPECT_FALSE(result);
    }
  } catch (exception &ex) {
    FAIL() << "Caught an exception while requesting an action. Exception: "
           << ex.what() << endl;
  }
}