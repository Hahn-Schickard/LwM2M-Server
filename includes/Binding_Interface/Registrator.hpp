#ifndef __LWM2M_BINDING_REGISTRATOR_INTERFACE_HPP
#define __LWM2M_BINDING_REGISTRATOR_INTERFACE_HPP

#include "Deregister.hpp"
#include "DeviceRegistry.hpp"
#include "Logger.hpp"
#include "Register.hpp"
#include "Requester.hpp"
#include "Update.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace LwM2M {

class Registrator : public Requester,
                    public std::enable_shared_from_this<Requester> {

  DeviceRegistryPtr registry_;
  std::shared_ptr<HaSLL::Logger> logger_;

  /**
   * @brief Assigns correct LwM2M::ObjectDescriptorMap based on given input.
   * Ignores unsupported object ids.
   *
   * @param requested_instances
   * @return ObjectDescriptorsMap
   */
  ObjectDescriptorsMap
  assignAvailableDescriptors(ElementIDs requested_instances);

  ElementIDs discoverAvailableDescriptors(
      EndpointPtr endpoint,
      const RegisterRequest::ObjectInstancesMap object_instances);

  ElementIDs discover(ServerRequestPtr request);
  void cancelDiscovery(ServerRequestPtr request);

public:
  Registrator(DeviceRegistryPtr registry);

  ~Registrator();

  /**
   * @brief Registers a new LwM2M::Device or Registers it, if it already
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