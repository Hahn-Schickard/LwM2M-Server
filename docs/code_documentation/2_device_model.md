# Device Model 

**Lightweight Machine to Machine Device Model** is an object orientated implementation of **OMA Lightweight Machine to Machine Resource Model** <sup>[1](@ref oma_lwm2m_core_spec)</sup>. Each **LwM2M Client** is modeled as a Device and contains various Objects, described by XML descriptor files. Each object contains a number if object instances, which in turn contain a number of resources, which MAY further contain a number of resource instances. This can be visualized as follows: 

@image html docs/images/lwm2M_device_model.png "LwM2M Device Model Overview" width=20%

## Class Diagram

@image html server_architechture/DeviceModel_Simplified.png "LwM2M Device Model Class Diagram"  width=1200px

## Device class

## Object class

## Resource Class

### Static information

### Dynamic information


## References 

#### [1] “Lightweight Machine to Machine Technical Specification: Core,” 2020. Accessed: Aug. 01, 2022. [Online]. Available: https://www.openmobilealliance.org/release/LightweightM2M/V1_2-20201110-A/OMA-TS-LightweightM2M_Core-V1_2-20201110-A.pdf {#oma_lwm2m_core_spec}
