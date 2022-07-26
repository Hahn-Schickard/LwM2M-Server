#include "Registrator.hpp"
#include "RequesterInterfaceFacade.hpp"

#include "../MockExceptionHandler.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <memory>

using namespace LwM2M;
using namespace std;

struct MockRegistrator : public Registrator, public ExceptionHandlerInterface {

  MockRegistrator(DeviceRegistryPtr registry) : Registrator(registry) {}

  MOCK_METHOD(std::future<DataFormatPtr>, requestData,
      (DeviceManagementRequestPtr), (override));
  MOCK_METHOD(void, cancelRequest, (ServerRequestPtr), (override));
  MOCK_METHOD(std::future<TargetContentVector>, requestMultiTargetData,
      (DeviceManagementRequestPtr message), (override));
  MOCK_METHOD(std::future<bool>, requestAction, (DeviceManagementRequestPtr),
      (override));
  MOCK_METHOD(size_t, requestObservation,
      (std::function<void(PayloadDataPtr)>, InformationReportingRequestPtr),
      (override));
  MOCK_METHOD(void, cancelObservation, (size_t, InformationReportingRequestPtr),
      (override));
  MOCK_METHOD(
      std::future<ClientResponsePtr>, request, (ServerRequestPtr), (override));
  MOCK_METHOD(void, handleDeviceException, (std::exception_ptr), (override));
};

using MockRequesterInterfaceFacadePtr = std::shared_ptr<MockRegistrator>;

TEST(RegistratorInstantiationTests,
    throwsInvalidArgumentForNullptrDeviceRegistry) {
  EXPECT_THROW(
      { make_shared<MockRegistrator>(DeviceRegistryPtr()); }, invalid_argument);
}

/*
TODO: Handle RegistratorTests() and TearDown w.r.t. initial_device_.
For example, initial_device_ could be changed to a std::shared_ptr or to a
std::optional<NonemptyPtr>

class RegistratorTests : public ::testing::Test {
protected:
  void SetUp() override {
    registry_ = make_shared<DeviceRegistry>("model/passingModel1.xml");
    registrator_ = make_shared<MockRegistrator>(registry_);
    function<void(exception_ptr)> callback =
        bind(&ExceptionHandlerInterface::handleDeviceException,
            dynamic_pointer_cast<ExceptionHandlerInterface>(registrator_),
            std::placeholders::_1);
    initial_device_ = NonemptyPointer::make_shared<Device>(callback, registrator_,
        make_shared<Endpoint>("0.0.0.0", 10), ObjectDescriptorsMap(), "123456",
        10, "initial_device");
    registry_->registerDevice(initial_device_);
  }
  DeviceRegistryPtr registry_;
  RegistratorPtr registrator_;
  DevicePtr initial_device_;
};

TEST_F(RegistratorTests, returnsCreatedOnRegisterRequest) {
  auto endpoint = make_shared<Endpoint>("0.0.0.0", 10);
  unordered_map<unsigned int, vector<unsigned int>> object_instances;
  object_instances.emplace(3303, vector<unsigned int>{0, 1});
  auto request = make_shared<RegisterRequest>(
      endpoint, 20, object_instances, "TestDevice");

  try {
    auto response = registrator_->handleRquest(request);
    EXPECT_TRUE(response->payload_->hasData());
    auto data_format = std::get<DataFormatPtr>(response->payload_->data_);
    auto location = data_format->get<string>();
    EXPECT_EQ(response->response_code_, ResponseCode::CREATED);
  } catch (exception& ex) {
    FAIL() << "Caught an exception while handling registration request. "
              "Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RegistratorTests, throwsNullptrOnEmptyRegisterRequest) {
  EXPECT_THROW(
      registrator_->handleRquest(RegisterRequestPtr()), invalid_argument);
}

TEST_F(RegistratorTests, returnsChangedOnUpdateRequest) {
  auto endpoint = make_shared<Endpoint>("0.0.0.0", 10);
  unordered_map<unsigned int, vector<unsigned int>> object_instances;
  object_instances.emplace(3303, vector<unsigned int>{0});
  auto request = make_shared<UpdateRequest>(
      endpoint, initial_device_->getDeviceId(), object_instances);

  try {
    auto response = registrator_->handleRquest(request);
    EXPECT_EQ(response->response_code_, ResponseCode::CHANGED);
  } catch (exception& ex) {
    FAIL() << "Caught an exception while handling registration request. "
              "Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RegistratorTests, returnsNotFoundOnUpdateRequest) {
  auto endpoint = make_shared<Endpoint>("0.0.0.0", 10);
  auto request =
      make_shared<UpdateRequest>(endpoint, "NonExistantDeviceLocation");

  try {
    auto response = registrator_->handleRquest(request);
    EXPECT_EQ(response->response_code_, ResponseCode::NOT_FOUND);
  } catch (exception& ex) {
    FAIL() << "Caught an exception while handling registration request. "
              "Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RegistratorTests, throwsNullptrOnEmptyUpdateRequest) {
  EXPECT_THROW(
      registrator_->handleRquest(UpdateRequestPtr()), invalid_argument);
}

TEST_F(RegistratorTests, returnsDeletedOnDeregisterRequest) {
  auto endpoint = make_shared<Endpoint>("0.0.0.0", 10);
  auto request =
      make_shared<DeregisterRequest>(endpoint, initial_device_->getDeviceId());

  try {
    auto response = registrator_->handleRquest(request);
    EXPECT_EQ(response->response_code_, ResponseCode::DELETED);
  } catch (exception& ex) {
    FAIL() << "Caught an exception while handling registration request. "
              "Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RegistratorTests, returnsNotFoundOnDeregisterRequest) {
  auto endpoint = make_shared<Endpoint>("0.0.0.0", 10);
  auto request =
      make_shared<DeregisterRequest>(endpoint, "NonExistantDeviceLocation");

  try {
    auto response = registrator_->handleRquest(request);
    EXPECT_EQ(response->response_code_, ResponseCode::NOT_FOUND);
  } catch (exception& ex) {
    FAIL() << "Caught an exception while handling registration request. "
              "Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RegistratorTests, throwsNullptrOnEmptyDeregisterRequest) {
  EXPECT_THROW(
      registrator_->handleRquest(DeregisterRequestPtr()), invalid_argument);
}
*/
