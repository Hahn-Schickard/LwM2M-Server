#ifndef __LWM2M_BINDING_REGISTRATOR_INTERFACE_HPP
#define __LWM2M_BINDING_REGISTRATOR_INTERFACE_HPP

#include "Deregister.hpp"
#include "DeviceRegistry.hpp"
#include "Register.hpp"
#include "Requester.hpp"
#include "Update.hpp"

namespace LwM2M {

class Registrator {
  DeviceRegistryPtr registry_;
  RequesterPtr requester_;

  ObjectDescriptorsMap assignObjectDescriptors(
      const std::unordered_map<unsigned int, std::vector<unsigned int>>
          requested_object_instances);

public:
  Registrator(DeviceRegistryPtr registry, RequesterPtr requester);

  RegisterResponsePtr handleRquest(RegisterRequestPtr request);
  UpdateResponsePtr handleRquest(UpdateRequestPtr request);
  DeregisterResponsePtr handleRquest(DeregisterRequestPtr request);
};

} // namespace LwM2M

#endif //__LWM2M_BINDING_REGISTRATOR_INTERFACE_HPP