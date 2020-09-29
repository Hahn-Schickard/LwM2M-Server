cmake_minimum_required(VERSION 3.6) 

set(THIS Message_Encoder)

list(APPEND THIS_INCLUDES_DIRS
            ${INCLUDES_DIR}/LwM2M/Message_Encoder
)

file(GLOB sources_list "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

INCLUDE_DIRS_AND_FILES("${THIS_INCLUDES_DIRS}" THIS_INCLUDES)

add_library(${THIS})                

target_sources(${THIS} PRIVATE 
                       ${THIS_INCLUDES}
                       ${sources_list}
)

target_link_libraries(${THIS} PUBLIC
                              CoAP_Message
                              CoAP_Payload_Formats
                              LwM2M_Core_Messages
                              LwM2M_Registration_Messages
                              LwM2M_Information_Reporting_Messages
                              LwM2M_Device_Managment_Messages
                              Response_Handler
                              TDC4CPP17::TDC4CPP17
                              HaSLL::HaSLL
)

target_include_directories(${THIS} INTERFACE
                           $<BUILD_INTERFACE:${INCLUDES_DIR}/LwM2M/Message_Encoder>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(${THIS} PUBLIC cxx_std_17)

PRINT_TARGET_PROPERTIES(${THIS})

install(
    TARGETS ${THIS}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    FILES ${THIS_INCLUDES}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
