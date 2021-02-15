#include "MockRequestsManager.hpp"
#include "Registrator.hpp"

#include "gtest/gtest.h"

#include <memory>

using namespace LwM2M;
using namespace std;

TEST(RegistratorInstantiationTests,
     throwsInvalidArgumentForNullptrDeviceRegistry) {
  EXPECT_THROW(
      {
        make_shared<Registrator>(DeviceRegistryPtr(), MockRequestsManagerPtr());
      },
      invalid_argument);
}

TEST(RegistratorInstantiationTests,
     throwsInvalidArgumentForNullptrDispatcherInterface) {
  EXPECT_THROW(
      {
        make_shared<Registrator>(
            make_shared<DeviceRegistry>("thisDoesNotMatter"),
            MockRequestsManagerPtr());
      },
      invalid_argument);
}

class RegistratorTests : public ::testing::Test {
protected:
  void SetUp() override {
    registry_ = make_shared<DeviceRegistry>("model/passingModel1.xml");
    auto DispatcherInterface = make_shared<MockRequestsManager>();
    registrator_ = make_shared<Registrator>(registry_, DispatcherInterface);
    initial_device_ = make_shared<Device>(
        DispatcherInterface, make_shared<Endpoint>("0.0.0.0", 10),
        ObjectDescriptorsMap(), 10, "initial_device");
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
  auto request = make_shared<RegisterRequest>(endpoint, 20, object_instances,
                                              "TestDevice");

  try {
    auto response = registrator_->handleRquest(request);
    EXPECT_TRUE(registry_->isRegistered(response->location_));
    EXPECT_EQ(response->response_code_, ResponseCode::CREATED);
  } catch (exception &ex) {
    FAIL() << "Cought an exception while handling registration request. "
              "Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RegistratorTests, throwsNullptrOnEmptyRegisterRequest) {
  EXPECT_THROW(registrator_->handleRquest(RegisterRequestPtr()),
               invalid_argument);
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
  } catch (exception &ex) {
    FAIL() << "Cought an exception while handling registration request. "
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
  } catch (exception &ex) {
    FAIL() << "Cought an exception while handling registration request. "
              "Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RegistratorTests, throwsNullptrOnEmptyUpdateRequest) {
  EXPECT_THROW(registrator_->handleRquest(UpdateRequestPtr()),
               invalid_argument);
}

TEST_F(RegistratorTests, returnsDeletedOnDeregisterRequest) {
  auto endpoint = make_shared<Endpoint>("0.0.0.0", 10);
  auto request =
      make_shared<DeregisterRequest>(endpoint, initial_device_->getDeviceId());

  try {
    auto response = registrator_->handleRquest(request);
    EXPECT_EQ(response->response_code_, ResponseCode::DELETED);
  } catch (exception &ex) {
    FAIL() << "Cought an exception while handling registration request. "
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
  } catch (exception &ex) {
    FAIL() << "Cought an exception while handling registration request. "
              "Exception: "
           << ex.what() << endl;
  }
}

TEST_F(RegistratorTests, throwsNullptrOnEmptyDeregisterRequest) {
  EXPECT_THROW(registrator_->handleRquest(DeregisterRequestPtr()),
               invalid_argument);
}