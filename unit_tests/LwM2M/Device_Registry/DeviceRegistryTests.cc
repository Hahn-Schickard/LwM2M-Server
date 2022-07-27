#include "DeviceRegistry.hpp"
#include "Event_Model/EventListenerInterface.hpp"

#include "../MockExceptionHandler.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <exception>
#include <functional>
#include <memory>

using namespace LwM2M;
using namespace std;

TEST(throwsExceptionOnNonExistantConfigFile, canBuildWithGoodModel) {
  try {
    auto registry = make_shared<DeviceRegistry>("model/passingModel2.xml");
    EXPECT_NE(registry, nullptr);
    EXPECT_GT(registry->getSupportedDescriptors()->size(), 0);
  } catch (exception& ex) {
    FAIL() << "Caught an exception while building the Device Registry. "
              "Exception: "
           << ex.what() << endl;
  }
}

TEST(throwsExceptionOnNonExistantConfigFile, canBuildWithNonExistantModel) {
  try {
    auto registry = make_shared<DeviceRegistry>("model/nonExistantModel.xml");
    EXPECT_NE(registry, nullptr);
    EXPECT_EQ(registry->getSupportedDescriptors()->size(), 0);
  } catch (exception& ex) {
    FAIL() << "Caught an exception while building the Device Registry. "
              "Exception: "
           << ex.what() << endl;
  }
}

struct MockRegistryListener
    : public Event_Model::EventListenerInterface<RegistryEvent> {
  MockRegistryListener(DeviceRegistryPtr registry)
      : EventListenerInterface(registry) {}

  MOCK_METHOD(void, handleEvent, (shared_ptr<RegistryEvent>), (override));
};

class DeviceRegistryTests : public ::testing::Test {
protected:
  void SetUp() override {
    registry_ = make_shared<DeviceRegistry>("model/passingModel1.xml");
    listener_ = make_shared<MockRegistryListener>(registry_);
    exception_handler_ = make_shared<MockExceptionHandler>();
    auto callback = bind(&ExceptionHandlerInterface::handleDeviceException,
        exception_handler_.get(), std::placeholders::_1);
    initial_device_ = NonemptyPointer::make_shared<Device>(callback,
        RequesterInterfaceFacadePtr(), make_shared<Endpoint>("0.0.0.0", 10),
        ObjectDescriptorsMap(), "1234", 10, "initial_device");
    registry_->registerDevice(initial_device_);
  }

  DeviceRegistryPtr registry_;
  shared_ptr<MockRegistryListener> listener_;
  shared_ptr<MockExceptionHandler> exception_handler_;
  DevicePtr initial_device_ = NonemptyPointer::make_shared<Device>();
};

MATCHER_P(EventPtrIsEqual, value, "") {
  if (arg->type_ == value->type_ && arg->identifier_ == value->identifier_)
    return true;
  else
    return false;
}

TEST_F(DeviceRegistryTests, canRegisterDevice) {
  auto callback = bind(&ExceptionHandlerInterface::handleDeviceException,
      exception_handler_.get(), std::placeholders::_1);
  auto device = NonemptyPointer::make_shared<Device>(callback,
      RequesterInterfaceFacadePtr(), make_shared<Endpoint>("1.1.1.1", 10),
      ObjectDescriptorsMap(), "1243135", 10, "test_device");

  auto event = std::make_shared<RegistryEvent>(
      RegistryEventType::REGISTERED, device->getDeviceId(), device);
  EXPECT_CALL(*listener_, handleEvent(EventPtrIsEqual(event))).Times(1);

  try {
    registry_->registerDevice(device);
    EXPECT_TRUE(registry_->isRegistered(device->getDeviceId()));
  } catch (exception& ex) {
    FAIL() << "Caught an exception while registering test device. Exception: "
           << ex.what();
  }
}

TEST_F(DeviceRegistryTests, canDeregisterDevice) {
  EXPECT_CALL(*listener_,
      handleEvent(EventPtrIsEqual(make_shared<RegistryEvent>(
          RegistryEventType::DEREGISTERED, initial_device_->getDeviceId()))))
      .Times(1);

  try {
    registry_->deregisterDevice(initial_device_->getDeviceId());
  } catch (exception& ex) {
    FAIL() << "Caught an exception while registering test device. Exception: "
           << ex.what();
  }
}

TEST_F(DeviceRegistryTests, deregisterDeviceThrowsDeviceNotFound) {
  EXPECT_THROW({ registry_->deregisterDevice("nonExistantDeviceName"); },
      DeviceNotFound);
}

TEST_F(DeviceRegistryTests, canReregisterDevice) {
  auto callback = bind(&ExceptionHandlerInterface::handleDeviceException,
      exception_handler_.get(), std::placeholders::_1);
  auto device = NonemptyPointer::make_shared<Device>(callback,
      RequesterInterfaceFacadePtr(), make_shared<Endpoint>("1.1.1.1", 10),
      ObjectDescriptorsMap(), "12412325", 10, "test_device");

  auto event = std::make_shared<RegistryEvent>(
      RegistryEventType::REGISTERED, device->getDeviceId(), device);
  EXPECT_CALL(*listener_, handleEvent(EventPtrIsEqual(event))).Times(2);
  EXPECT_CALL(*listener_,
      handleEvent(EventPtrIsEqual(make_shared<RegistryEvent>(
          RegistryEventType::DEREGISTERED, device->getDeviceId()))))
      .Times(1);

  try {
    registry_->registerDevice(device);
    registry_->registerDevice(device);
    EXPECT_TRUE(registry_->isRegistered(device->getDeviceId()));
  } catch (exception& ex) {
    FAIL() << "Caught an exception while registering test device. Exception: "
           << ex.what();
  }
}

TEST_F(DeviceRegistryTests, canUpdateDevice) {
  initial_device_->updateLifetime(2222);

  auto event = std::make_shared<RegistryEvent>(RegistryEventType::UPDATED,
      initial_device_->getDeviceId(), initial_device_);
  EXPECT_CALL(*listener_, handleEvent(EventPtrIsEqual(event))).Times(1);

  try {
    registry_->updateDevice(initial_device_);
  } catch (exception& ex) {
    FAIL() << "Caught an exception while registering test device. Exception: "
           << ex.what();
  }
}

TEST_F(DeviceRegistryTests, updateDeviceThrowsDeviceNotFound) {
  auto callback = bind(&ExceptionHandlerInterface::handleDeviceException,
      exception_handler_.get(), std::placeholders::_1);
  auto device = NonemptyPointer::make_shared<Device>(callback,
      RequesterInterfaceFacadePtr(), make_shared<Endpoint>("1.1.1.1", 10),
      ObjectDescriptorsMap(), "1251212214", 10, "test_device");
  EXPECT_THROW({ registry_->updateDevice(device); }, DeviceNotFound);
}
