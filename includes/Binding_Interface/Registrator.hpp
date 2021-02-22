#ifndef __LWM2M_BINDING_REGISTRATOR_INTERFACE_HPP
#define __LWM2M_BINDING_REGISTRATOR_INTERFACE_HPP

#include "Deregister.hpp"
#include "DeviceRegistry.hpp"
#include "Register.hpp"
#include "RequestsManagerInterface.hpp"
#include "Update.hpp"

namespace LwM2M {

class Registrator {
  DeviceRegistryPtr registry_;
  RequestsManagerInterfacePtr requester_;

  /**
   * @brief Assigns correct LwM2M::ObjecDescriptorMap based on given input.
   * Ignores unsupproted object ids.
   *
   * @param requested_object_instances
   * @return ObjectDescriptorsMap
   */
  ObjectDescriptorsMap assignObjectDescriptors(
      const std::unordered_map<unsigned int, std::vector<unsigned int>>
          requested_object_instances);

public:
  Registrator(DeviceRegistryPtr registry,
              RequestsManagerInterfacePtr requester);

  /**
   * @brief Registers a new LwM2M::Device or Reregisters it, if it already
   * exists in the LwM2M::DeviceRegistry. Returns an appropriate
   * LwM2M::RegisterResponse with corresponding LwM2M::ResponseCode and/or
   * unique LwM2M::Device id within the LwM2M::DeviceRegistry
   *
   * @param request
   * @return RegisterResponsePtr
   */
  RegisterResponsePtr handleRquest(RegisterRequestPtr request);

  /**
   * @brief Updates an existing LwM2M::Device within the LwM2M::DeviceRegistry.
   * Returns an appropriate LwM2M::UpdateResponse with with corresponding
   * LwM2M::ResponseCode
   *
   * @param request
   * @return UpdateResponsePtr
   */
  UpdateResponsePtr handleRquest(UpdateRequestPtr request);

  /**
   * @brief Removes a given LwM2M::Device from the LwM2M::DeviceRegistry, if
   * contained Device ID is not found within LwM2M::DeviceRegistry does not
   * modify it. Returns an appropriate LwM2M::DeregisterResponse with with
   * corresponding LwM2M::ResponseCode
   *
   * @param request
   * @return DeregisterResponsePtr
   */
  DeregisterResponsePtr handleRquest(DeregisterRequestPtr request);

  EventSourcePtr getEventSource();
};

using RegistratorPtr = std::shared_ptr<Registrator>;
} // namespace LwM2M

#endif //__LWM2M_BINDING_REGISTRATOR_INTERFACE_HPP