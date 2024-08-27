#include "Registrator.hpp"
#include "RequesterInterfaceFacade.hpp"

#include "../MockExceptionHandler.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <memory>

using namespace LwM2M;
using namespace std;

// NOLINTBEGIN

struct MockRegistrator : public Registrator, public ExceptionHandlerInterface {

  MockRegistrator(const DeviceRegistryPtr& registry) : Registrator(registry) {}

  MOCK_METHOD(std::future<DataFormatPtr>, requestData,
      (const DeviceManagementRequestPtr&), (override));
  MOCK_METHOD(void, cancelRequest, (const ServerRequestPtr&), (override));
  MOCK_METHOD(std::future<TargetContentVector>, requestMultiTargetData,
      (const DeviceManagementRequestPtr&), (override));
  MOCK_METHOD(std::future<bool>, requestAction,
      (const DeviceManagementRequestPtr&), (override));
  MOCK_METHOD(size_t, requestObservation,
      (const std::function<void(PayloadDataPtr)>&,
          const InformationReportingRequestPtr&),
      (override));
  MOCK_METHOD(void, cancelObservation,
      (size_t, const InformationReportingRequestPtr&), (override));
  MOCK_METHOD(std::future<ClientResponsePtr>, request,
      (const ServerRequestPtr&), (override));
  MOCK_METHOD(
      void, handleDeviceException, (const std::exception_ptr&), (override));
};

using MockRequesterInterfaceFacadePtr = std::shared_ptr<MockRegistrator>;

// NOLINTNEXTLINE
TEST(RegistratorInstantiationTests,
    throwsInvalidArgumentForNullptrDeviceRegistry) {
  EXPECT_THROW(
      { make_shared<MockRegistrator>(DeviceRegistryPtr()); }, invalid_argument);
}

class RegistratorTests : public ::testing::Test {
protected:
  void SetUp() override {
    registry_ = make_shared<DeviceRegistry>("model/passingModel1.xml");
    registrator_ = make_shared<MockRegistrator>(registry_);
    function<void(exception_ptr)> callback =
        bind(&ExceptionHandlerInterface::handleDeviceException,
            dynamic_pointer_cast<ExceptionHandlerInterface>(registrator_),
            std::placeholders::_1);
    initial_device_ = Nonempty::make_shared<Device>(callback, registrator_,
        make_shared<Endpoint>("0.0.0.0", 10), ObjectDescriptorsMap(), "123456",
        10, "initial_device");
    registry_->registerDevice(initial_device_);
  }
  DeviceRegistryPtr registry_;
  RegistratorPtr registrator_;
  DevicePtr initial_device_ = Nonempty::make_shared<Device>();
};

// NOLINTNEXTLINE
TEST_F(RegistratorTests, returnsCreatedOnRegisterRequest) {
  auto endpoint = make_shared<Endpoint>("0.0.0.0", 10);
  unordered_map<unsigned int, vector<unsigned int>> object_instances;
  object_instances.emplace(3303, vector<unsigned int>{0, 1}); // NOLINT
  auto request = make_shared<RegisterRequest>(
      endpoint, 20, object_instances, "TestDevice"); // NOLINT

  try {
    auto response = registrator_->handleRequest(request);
    EXPECT_TRUE(response->payload_->hasData());
    const auto& data_format =
        std::get<DataFormatPtr>(response->payload_->data_);
    auto location = data_format->get<string>();
    EXPECT_EQ(response->response_code_, ResponseCode::CREATED);
  } catch (exception& ex) {
    FAIL() << "Caught an exception while handling registration request. "
              "Exception: "
           << ex.what() << endl;
  }
}

// NOLINTNEXTLINE
TEST_F(RegistratorTests, throwsNullptrOnEmptyRegisterRequest) {
  EXPECT_THROW(
      registrator_->handleRequest(RegisterRequestPtr()), invalid_argument);
}

// NOLINTNEXTLINE
TEST_F(RegistratorTests, returnsChangedOnUpdateRequest) {
  auto endpoint = make_shared<Endpoint>("0.0.0.0", 10);
  unordered_map<unsigned int, vector<unsigned int>> object_instances;
  object_instances.emplace(3303, vector<unsigned int>{0});
  auto request = make_shared<UpdateRequest>(
      endpoint, initial_device_->getDeviceId(), object_instances);

  try {
    auto response = registrator_->handleRequest(request);
    EXPECT_EQ(response->response_code_, ResponseCode::CHANGED);
  } catch (exception& ex) {
    FAIL() << "Caught an exception while handling registration request. "
              "Exception: "
           << ex.what() << endl;
  }
}

// NOLINTNEXTLINE
TEST_F(RegistratorTests, returnsNotFoundOnUpdateRequest) {
  auto endpoint = make_shared<Endpoint>("0.0.0.0", 10);
  auto request = make_shared<UpdateRequest>(endpoint,
      "NonExistantDeviceLocation", DeviceMetaInfo::ObjectInstancesMap());

  try {
    auto response = registrator_->handleRequest(request);
    EXPECT_EQ(response->response_code_, ResponseCode::NOT_FOUND);
  } catch (exception& ex) {
    FAIL() << "Caught an exception while handling registration request. "
              "Exception: "
           << ex.what() << endl;
  }
}

// NOLINTNEXTLINE
TEST_F(RegistratorTests, throwsNullptrOnEmptyUpdateRequest) {
  EXPECT_THROW(
      registrator_->handleRequest(UpdateRequestPtr()), invalid_argument);
}

// NOLINTNEXTLINE
TEST_F(RegistratorTests, returnsDeletedOnDeregisterRequest) {
  auto endpoint = make_shared<Endpoint>("0.0.0.0", 10);
  auto request =
      make_shared<DeregisterRequest>(endpoint, initial_device_->getDeviceId());

  try {
    auto response = registrator_->handleRequest(request);
    EXPECT_EQ(response->response_code_, ResponseCode::DELETED)
        << "Expected ResponseCode Deleted, but received ResponseCode "
        << toString(response->response_code_);
  } catch (exception& ex) {
    FAIL() << "Caught an exception while handling registration request. "
              "Exception: "
           << ex.what() << endl;
  }
}

// NOLINTNEXTLINE
TEST_F(RegistratorTests, returnsNotFoundOnDeregisterRequest) {
  auto endpoint = make_shared<Endpoint>("0.0.0.0", 10);
  auto request =
      make_shared<DeregisterRequest>(endpoint, "NonExistantDeviceLocation");

  try {
    auto response = registrator_->handleRequest(request);
    EXPECT_EQ(response->response_code_, ResponseCode::NOT_FOUND);
  } catch (exception& ex) {
    FAIL() << "Caught an exception while handling registration request. "
              "Exception: "
           << ex.what() << endl;
  }
}

// NOLINTNEXTLINE
TEST_F(RegistratorTests, throwsNullptrOnEmptyDeregisterRequest) {
  EXPECT_THROW(
      registrator_->handleRequest(DeregisterRequestPtr()), invalid_argument);
}

// NOLINTEND
