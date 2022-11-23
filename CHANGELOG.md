# Changelog
## [0.5.2] - 2022.11.23
### Added
 - keep alive handler via update request

### Changed
 - `stringifyDataVariant()` to hexify Opaque values in Example
 - some `TRACE` level messages into `INFO` for `Registrator::handleRequest()` method implementations to help track Device state changes
 - `TRACE` level message for device removal into `WARNING` for `DeviceRegistry::registerDevice()`
 - `TRACE` level message for device registration into `INFO` for `DeviceRegistry::registerDevice()`
 - `TRACE` level message for device update into `INFO` for `DeviceRegistry::updateDevice()`
 - `TRACE` level message for device deregistration into `INFO` for `DeviceRegistry::deregisterDevice()`
 - `TimeStamp::toString()` implementation to reduce raw pointer usage 

### Removed
 - redundant cases in CoAP Message encoding
 - logger warning messages for encoding CoAP Message Payload with no value

### Fixed
 - double erase for Observers in Example

## [0.5.1] - 2022.11.16
### Changed
 - conan packaging recipe
 - HaSLL dependency to fuzzy v0.3
 - HSCUL dependency to fuzzy v0.2
 - Nonempty_Pointer dependency to fuzzy v0.1
 - Threadsafe_Containers dependency to fuzzy v0.6
 - CoAPS4Cpp dependency to fuzzy v0.5
 - Variant_Visitor dependency to fuzzy v0.1
 - Event_Model dependency to fuzzy v0.3
 
### Added
 - Keep Alive request handling stub in `Registrator::handleRequest(const RegisterRequestPtr&)` 

## [0.5.0] - 2022.10.17
### Fixed
 - `Registrator::handleRequest` typo

### Added
 - example usage documentation
 - device model documentation stub
 - message model documentation stub
 - server architecture documentation stub
 - `RequestResult` class for CancelableInterface
 - `CancelableInterface` class
 - `CompositeReadableInterface` class
 - doxygen documentation to `Device` class
 - `Device::getObjectInstance()` method
 - `Device::getObjectInstances()` method
 - `Device::getResource()` method
 - `Device::getResources()` method
 - `Device::getResourceInstance()` method
 - `Device::getResourceInstances()` method
 - `DevicePtr` alias
 - `ExecutableInterface` class
 - doxygen documentation to `Object` class
 - `Object::getObjectInstance()` method
 - `Object::getObjectInstances()` method
 - `Object::getResource()` method
 - `Object::getResources()` method
 - `Object::getResourceInstance()` method
 - `Object::getResourceInstances()` method
 - `ObjectPtr` alias
 - doxygen documentation to `ObjectInstance` class
 - `ObjectInstance::getResource()` method
 - `ObjectInstance::getResourceInstance()` method
 - `ObjectInstance::getResourceInstances()` method
 - `ObjectInstancePtr` alias
 - doxygen documentation to `Observable` class
 - `ResourceDescriptorDoesNotExist` exception
 - doxygen documentation to `ObjectDescriptor` class
 - `ObservableInterface` class
 - `Operationless` class
 - doxygen documentation to `RangeEnumeration` class
 - `ReadableInterface` class
 - `ResourceInstanceDoesNotExist` exception
 - `ResourceInstanceCouldNotBeResolved` exception
 - doxygen documentation to `Resource` class
 - `Resource::getResourceInstance()` method
 - `Resource::getResourceInstances()` method
 - `ResourcePtr` alias
 - Message_Model doxygen group
 - doxygen documentation to `ElementID` class
 - `ElementID` expanding ctor

### Changed
 - code formatting rules
 - contributing guide
 - readme
 - nlohmann_json dependency to v3.11.1
 - pugixml dependency to v1.11
 - HaSLL dependency to v0.3.1
 - Nonempty_Pointer dependency to v0.1.2
 - Threadsafe_Container dependency to v0.6.0
 - CoAPS4Cpp dependency to v0.5.0
 - EventModel dependency to v0.3.4
 - Logger usage to work with HaSLL v0.3.1
 - `Device` class to use `RequesterInterfaceFacadePtr` instead of `RequesterPtr`
 - `CallableEntity`into `ElementAddress`
 - `Executable` class to no longer inherit from `Resource<T>`
 - `Object` class to use `RequesterInterfaceFacade` instead of `RequesterPtr`
 - `ObjectInstance` class to use `RequesterInterfaceFacade` instead of `RequesterPtr`
 - `ReadAndWritable ` class to no longer inherit from `Resource<T>`
 - `ReadAndWritable` to be a composition of `Readable` and `Writable` classes
 - `Readable` class to no longer inherit from `Resource<T>`
 - `Writable` class to no longer inherit from `Resource<T>`
 - `RegistryEvent` ctor to accept `DevicePtr` as an option value 

### Removed
 - explicit `Registrator` dtor
 - `Requester` class
 
## [0.4.0] - 2022.04.08
### Added
 - `DeviceMetaInfo` class to store device information from Registration Interface
 - `makeDevice()` method to `Registrator` class
 - manual discovery mechanism, inc case of `DiscoverRequest` failure
 - `ObjectInstance` Read support
 - `TargetContentVector` payload support in `handleReadResponse()` method of `Registrator` class
 - `cancelRequest()` to allow for cancellation of discovery 
 - `ElementID` field to `DiscoverRequest` for easier discovery mechanism
 - `ElementID` field to `ReadRequest` for easier manual discovery mechanism
 - `getIdentifierType()` method to TLV class
 - `Payload` hashers
 - save mechanism to `CoAP_Binding` for encoded coap messages to support `cancelRequest()`
 - multi resource read support
 - TLV identifier and length field converters
 - draft handler for object without instance
 - `hasData()` checker to `Payload`
 - fallback mechanism when building registration request, in case of `ContentFormat` option not being set by the caller
 - more trace logging messages in `CoAP_Binding::makeClientResponse()` method
 - `Registrator::handleDeviceException()` method
 - `CallableEntity` to `Device` model to contain address information and Requester interface
 - `Observable` to model Information Reporting interface implementations
 - `Requester::requestObservation()` method
 - `Requester::cancelObservation()` method
 - `MockExceptionHandler` to unit tests
 - `ExceptionHandlerInterface` to unit tests
 - `CoAP_Binding::requestObservation()` method implementation
 - `CoAP_Binding::cancelObservation()` implementation
 - `CoAP_Binding::observed_elements_` to keep track of observations 
 - `DeviceManagementRequest` to generalize `Device` Management Interface Requests
 - `RegistrationInterfaceRequest` to generalize Registration Interface Requests
 - `RegistrationInterfaceResponse` to generalize Registration Interface Responses
 - `InformationReportingRequest` to generalize Information Reporting Uplink Request
 - `InformationReportingDownlinkResponse` to generalize Information Reporting Downlink Response
 - `InformationReportingDownlinkRequest` to generalize Information Reporting Downlink Request
 - `MockRegistrator::cancelRequest()` mock 
 - `MockRegistrator` request mock 
 - `MockRegistrator` requestObservation mock 
 - `MockRegistrator` cancelObservation mock
 - `TestRequester::requestObservation()` method
 - `TestRequester::cancelObservation()` method 
 - `TestRequester::notify()` method 
 - `Observable` class
 - `ObserverInterface` facade

### Changed
 - CoAPS4Cpp to v0.4.0
 - `handleRequest(RegisterRequest)` method to assign register location before DevicePtr is built
 - `RegisterRequest` to use `DeviceMetaInfo` class
 - `UpdateRequest` to use `DeviceMetaInfo` class
 - `Device` ID generation to be done in `Registrator` instead of `Device` ctor
 - `Device` ctor to accept device id specification
 - `DeviceRegistryTests` to use new registration request handler
 - `Registrator`Tests to use new `Device` ctor
 - Event creation and dispatchment in DeviceRegistry
 - `multiple_instance` resources to be filter down in `ObjectInstance` class and not in `Registrator` 
 - `ElementID::getObjectInstanceID()`, `ElementID::getResourceID()` and `ElementID::getResourceInstanceID()` to correctly return the value or throw `std::bad_optional_access` exception
 - `Object` constructor to silently ignore objects without and assigned instance
 - `Object` class internal helper function assignResourceDescriptors() to build mandatory resources first and then check for optional resources
 - CoRE Link payload decoder in CoAP_Binding to support all available targets
 - `size_of()` functions in `DataFormat` unit, to accept const references instead of values
 - Event_Model to v0.3.2
 - `Readable` resource constructor order
 - `Writable` resource constructor order
 - `ReadAndWritable` resource constructor order
 - `Executable` resource constructor order
 - `Resource` class constructor to only accept `ResourceDescriptorPtr`
 - `Object` class constructor to require ExceptionHandler for Observable
 - `ObjectInstance` class constructor to require ExceptionHandler for `Observable` class
 - `Device` class constructor to require ExceptionHandler for `Observable` class
 - `Executable` resource to implement CallableEntity
 - `Writable` resource to implement CallableEntity 
 - `Readable` resource to implement Observable
 - `ReadAndWritable` resource to implement Observable
 - `MockRegistrator` to implementation `ExceptionHandlerInterface`
 - `ResourceTest` to build LwM2M `Resources` for each test separately
 - `CreateRequest` to implement `DeviceManagementRequest`
 - `DeleteRequest` to implement `DeviceManagementRequest`
 - `DiscoverRequest` to implement `DeviceManagementRequest`
 - `ExecuteRequest` to implement `DeviceManagementRequest`
 - `ReadRequest` to implement `DeviceManagementRequest`
 - `ReadCompositeRequest` to implement `DeviceManagementRequest`
 - `WriteRequest` to implement `DeviceManagementRequest`
 - `WriteAttributesRequest` to implement `DeviceManagementRequest`
 - `WriteCompositeRequest` to implement `DeviceManagementRequest`
 - `DeregisterResponse` to implement RegistrationInterfaceResponse
 - `DeregisterRequest` to implement `RegistrationInterfaceRequest`
 - `RegisterResponse` to implement `RegistrationInterfaceResponse` 
 - `RegisterRequest` to implement `RegistrationInterfaceRequest`
 - `UpdateResponse` to implement RegistrationInterfaceResponse
 - `UpdateRequest` to implement `RegistrationInterfaceRequest`
 - `CancelObservationRequest` to implement InformationReportingRequest
 - `CancelObserveCompositeRequest` to implement InformationReportingRequest
 - `ObserveRequest` to implement InformationReportingRequest
 - `ObserveCompositeRequest` to implement InformationReportingRequest
 - `SendResponse` to implement InformationReportingDownlinkResponse
 - `SendRequest` to implement InformationReportingDownlinkRequest
 - `Requester::requestData` to accept `DeviceManagementRequest`Ptr
 - `Requester::requestMultiTargetData` to accept `DeviceManagementRequest`Ptr
 - `Requester::requestAction` to accept `DeviceManagementRequest`Ptr
 - `Readable` asyncDataRequest to accept `DeviceManagementRequest`Ptr
 - `ReadAndWritable` asyncDataRequest to accept `DeviceManagementRequest`Ptr
 - `CoAP_Binding::requestData` to accept `DeviceManagementRequest`Ptr
 - `CoAP_Binding::requestMultiTargetData` to accept `DeviceManagementRequest`Ptr
 - `CoAP_Binding::requestAction` to accept `DeviceManagementRequest`Ptr
 - `MockRegistrator` requestData mock to accept `DeviceManagementRequest`Ptr
 - `MockRegistrator` requestMultiTargetData mock to accept `DeviceManagementRequest`Ptr
 - `MockRegistrator` requestAction mock to accept `DeviceManagementRequest`Ptr
 - `TestRequester` implementation to use optional<promise<T>> instead of preset promises 
 - pugiXML to v1.11 from conan-center 
 - `RegistrationInterfaceError` to RegistrationInterfaceMessage from `RegistrationInterfaceRequest`sBuilder
 - CoAP Binding to use `CoAP_Decoder` to build LwM2M Messages from CoAP
 - CoAP Binding to use `CoAP_Encoder` to build CoAP Messages from LwM2M

### Removed
 - unused nlohmann_json links
 - `ResourceMetaInfo` class
 - `Resource::getDescriptor()` method and it's implementations

## [0.3.0] - 2021.08.09
### Added
 - explicit `Endpoint` constructors 
 - `toString()` method for `Endpoint` struct
 - a check if message payload is empty for buildOptions function in CoAP_Binding unit
 - explicit `RegisterResponse` ctor to signify failure
 - explicit `RegisterResponse` ctor to signify success
 - explicit `UpdateResponse` ctor to signify failure
 - explicit `UpdateResponse` ctor to signify success
 - explicit `DeregisterResponse` ctor to signify failure
 - explicit `DeregisterResponse` ctor to signify success
 - `RegisterRequest()` method to build successful `RegisterResponse` 
 - `RegisterRequest()` method to build failed `RegisterResponse` 
 - `makeDefaultMessage()` function to MessageTests.cc 
 - support for LwM2M optional resources
 - support for LwM2M multiple instance resources
 - device discovery mechanism
 - `ElementID` to model `Object`, `ObjectInstance`, `Resource` and `Resource` Instance IDs
 - parametrized ctors to CoAP_Binding

### Changed
 - `UpdateRequest::makeResponse()` method to auto resolve the correct `UpdateResponse` ctor
 - `DeregisterRequest::makeResponse()` method to auto resolve the correct `DeregisterResponse` ctor
 - Device ID generation function to create hex id with proper hash calculation
 - `RegistrationInterfaceRequestsBuilder` to explicitly set either `ResponseCode` or `Response` value 
 - `Registrator` to explicitly set failure codes as `ResponseCode`
 - LwM2M Server internal runner mechanism
 - main cpp to better showcase Server and Listener functionality
 - CoAPS4Cpp to v0.3.0
 - Event_Model to v0.3.0
 - `Object`, `Object` Instance, `Resource` and `Resource` Instance ID modelling

### Fixed 
 - `ResourceTests.cc` to set correct ctor parameters
 - Register.cc to set correct ctor parameters
 - CoAP socket blocks

### Removed
 - default `Endpoint` constructor
 - direct `Stoppable` dependency

## [0.2.4] - 2021.05.11
### Added
 - `ObjectDoesNotExist` exception 
 - `ObjectInstanceDoesNotExists` exception
 - `ResourceDoesNotExist` exception 
 - sanity checks for `CoAP_Binding buildOptions()` method when dealing with Registration Interface Messages
 - `Timestamp` data value 
 - `Resources` with `Timestamp` support

### Changed
 - `ResponseReturnedAnErrorCode` inheritance from `std::runtime_error` to public
 - `ResponseReturnedAnEmptyPayload` inheritance from `std::runtime_error` to public
 - `RequestNotFound` inheritance from `std::runtime_error` to public
 - `RequestAlreadyDispatched` inheritance from `std::runtime_error` to public
 - `RequestCanceled` inheritance from `std::runtime_error` to public
 - `UnsupportedOperation` inheritance from `std::logic_error` to public
 - `UnsupportedResponseCode` inheritance from `std::logic_error` to public
 - `RegistrationInterfaceError` inheritance from `std::domain_error` to public
 - `Device::getObject()` method to throw `ObjectDoesNotExist` instead of returning an empty value
 - `Object::getResource()` method to throw `ObjectInstance`DoesNotExists instead of returning an empty value
 - `ObjectInstance::getResource()` method to throw `ResourceDoesNotExist` instead of returning an empty value
 - `RequestsManagerInterface` to share internal logger with it's children
 - GTest runner to throw on failed assertions

### Fixed
 - spelling mistakes throughout the code base
 - spelling mistakes in the CHANGELOG
 - spelling mistakes in the README

### Removed 
 - support for LwM2M optional resources
 - support for LwM2M multiple instance resources

## [0.2.3] - 2021.04.27
### Fixed 
 - CoAP Binding not setting default ContentFormat when none is provided
 - Server not auto rejecting registration request for unsupported LwM2M versions

### Changed
 - CoAPS4Cpp to v0.2.4
 - `ParameterNotFound` to `RegistrationInterfaceError` in CoAP BInding `RegistrationInterfaceRequest`Builder

## [0.2.2] - 2021.04.01
### Changed
 - Event_Model to 0.2.0

## [0.2.1] - 2021.03.05
### Added
 - Forward definitions for `DataFormat` getter template arguments
 - Logging for `ClientResponsePtr makeClientResponse(CoAP::MessagePtr message)` method
 - Logging for `CoAP::MessagePtr encode(CoAP::MessagePtr request, ServerResponsePtr message)` method

## [0.2.0] - 2021.03.03
### Added
 - `BindingInterface` for Binding abstractions
 - `Notifier` placeholder for `Observer` implementation
 - `Registrator` class to handle common registration interface Messages
 - `RequestsManagerInterface` to handle common ServerRequests as well as an abstraction for Binding specific `RequestsManager` implementation
 - `ResponseHandler` to handle `ClientResponses` 
 - CoAP Binding implementation
 - CoAP Configuration 
 - CoAP TLV encoder/decoder 
 - CoAP Requests Manager implementations
 - Server Configuration
 - Device Model test cases
 - Message test cases
 - Device Registry test cases
 - Binding interface test cases
 - CoAPS4Cpp library dependency
 - `ElementID` variant to abstract various element types
 - `DataFormat` and `Payload` classes to abstract data within LwM2M messages
 - string converters for various enumerations
 - added config/ directory to host various configuration files, including the xml model

### Changed
 - Message model to contain various message specializations for `Device` Management, Registration and Information Reporting Messages
 - Message class definition
 - `Readable` resources to be a separate unit 
 - `Writable` resources to be a separate unit 
 - `ReadAndWritable` resources to be a separate unit 
 - `Executable` resources to be a separate unit 
 - `Resource` method of request
 - loggerConfig.json location to config/ 
 - serverConfig.json location to config/
 - serverConfig.json content
 - model/ directory to be under config/
 - conan dependencies
 - TLV implementation
 - source file structure (packaged sources will be found under LwM2M directory, to avoid header name collisions)
 - Server class implementation 

### Removed
 - CoAP implementation
 - CoAP specific messages
 - ASIO dependency
 - utility primitive converters 

## [0.1.2] - 2020.10.07
### Added 
 - DataVariant parameter to `writable` resources
 - Write Request generation
 - Write Request encoding
 - `LwM2M::DataFormat` to `CoAP::PayloadFormat` converter for `CoAP_Encoder`
 - Asynchronous Write method
 - Write implementations for `writable` resources
 - TLV encoding
 - Exception handling for Unsupported resource parameter

### Changed
 - Integer conversion to vector<uint8_t> 
 - Return type for `Writable` and `Executable` methods to void
 - `Payload` type to shared_ptr<DataFormat> for ServerRequest_Write 

## [0.1.1] - 2020.09.30
### Added 
 - ObjectsMap getter to Device
 - name getter to Device
 - instances getter to Object
 - resources getter to ObjectInstance
 - Server Config serialization
 - serverConfig.json

## [0.1.0] - 2020.09.29
### Added 
 - draft CoAP Server implementation
 - LwM2M Core
 - LwM2M Messages
 - LwM2M Device Model
 - XML Parser
 - LwM2M Device Registry
 - LwM2M Message Encoder 
 - LwM2M Message Decoder
 - LwM2M Response handler
 - LwM2M Server
 - Utility helpers for String Splitter, primitive conversions and hashing
 - unit tests
 - conan packaging
 - conan package integration test
 - documentation
 - LICENSE and NOTICE files
 - Contributing guide 
 - Authors
