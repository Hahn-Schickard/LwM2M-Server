#ifndef __LWM2M_BINDING_REGISTRATOR_INTERFACE_HPP
#define __LWM2M_BINDING_REGISTRATOR_INTERFACE_HPP

#include "Deregister.hpp"
#include "DeviceRegistry.hpp"
#include "Register.hpp"
#include "RequesterInterfaceFacade.hpp"
#include "Update.hpp"

#include "HaSLL/Logger.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace LwM2M {

class Registrator
    : public RequesterInterfaceFacade,
      public std::enable_shared_from_this<RequesterInterfaceFacade> {

  DeviceRegistryPtr registry_;
  HaSLI::LoggerPtr logger_;

  void makeDevice(std::string device_id, EndpointPtr device_address,
      DeviceMetaInfo device_info);

  ObjectDescriptorsMap assignAvailableDescriptors(
      ElementIDs requested_instances);

  ElementIDs discoverAvailableDescriptors(EndpointPtr endpoint,
      const DeviceMetaInfo::ObjectInstancesMap object_instances);

  ElementIDs discover(ServerRequestPtr request);

  void handleDeviceException(
      std::string device_id, std::exception_ptr exception_ptr);

public:
  Registrator(DeviceRegistryPtr registry);

  /**
   * @brief Registers a new LwM2M::Device or Registers it, if it already
   * exists in the LwM2M::DeviceRegistry. Returns an appropriate
   * LwM2M::RegisterResponse with corresponding LwM2M::ResponseCode and/or
   * unique LwM2M::Device id within the LwM2M::DeviceRegistry
   *
   * @param request
   * @return RegisterResponsePtr
   */
  RegisterResponsePtr handleRequest(RegisterRequestPtr request);

  /**
   * @brief Updates an existing LwM2M::Device within the LwM2M::DeviceRegistry.
   * Returns an appropriate LwM2M::UpdateResponse with with corresponding
   * LwM2M::ResponseCode
   *
   * @param request
   * @return UpdateResponsePtr
   */
  UpdateResponsePtr handleRequest(UpdateRequestPtr request);

  /**
   * @brief Removes a given LwM2M::Device from the LwM2M::DeviceRegistry, if
   * contained Device ID is not found within LwM2M::DeviceRegistry does not
   * modify it. Returns an appropriate LwM2M::DeregisterResponse with with
   * corresponding LwM2M::ResponseCode
   *
   * @param request
   * @return DeregisterResponsePtr
   */
  DeregisterResponsePtr handleRequest(DeregisterRequestPtr request);

  EventSourcePtr getEventSource();
};

using RegistratorPtr = std::shared_ptr<Registrator>;
} // namespace LwM2M

#endif //__LWM2M_BINDING_REGISTRATOR_INTERFACE_HPP