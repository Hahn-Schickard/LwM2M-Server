# Device Model

**Lightweight Machine to Machine Device Model** is an object orientated implementation of **OMA Lightweight Machine to Machine Resource Model** <sup>[1](@ref oma_lwm2m_core_spec)</sup>. Each **%LwM2M Client**  is modeled as a Device and contains various Objects, described by XML descriptor files. Each object contains a number if object instances, which in turn contain a number of resources, which MAY further contain a number of resource instances. This can be visualized as follows:

@image html docs/images/lwm2M_device_model.png "LwM2M Device Model Overview"  width=20%

## Class Diagram

@image html server_architechture/DeviceModel_Simplified.png "LwM2M Device Model Class Diagram" width=1200px

## Device class

Device class is the top most layer of  **%LwM2M Client**  abstraction. This class organizes all of the %LwM2M object and resource instances and separates them from other **%LwM2M Clients**. 

@attention Device class only organizes **%LwM2M Client** object, object instance, resource and resource instance elements that the said **%LwM2M Client** registered during the registration process or the user of the library created within this **%LwM2M Client**. Any interaction with **%LwM2M Clients** does not change the stored %LwM2M Resource Model descriptors. To change how the descriptors work, you must modify the corresponding XML files, however said modifications will be applied to ALL **%LwM2M Clients** using the modified XML data.

Device class is created when a **%LwM2M Client** starts the registration process with the Server. During this process **%LwM2M Client** informs the server of what %LwM2M version it uses, what binding protocol it uses, what is it's name, what object, object instance, resource and resource instance elements are already created and available for use, and finally the lifetime of the current registration.

## Object class

## Resource Class

### Static information

### Dynamic information


## References

#### [1] “Lightweight Machine to Machine Technical Specification: Core,” 2020. Accessed: Aug. 01, s2022. [Online]. Available: https://www.openmobilealliance.org/release/LightweightM2M/V1_2-20201110-A/OMA-TS-LightweightM2M_Core-V1_2-20201110-A.pdf {#oma_lwm2m_core_spec}
