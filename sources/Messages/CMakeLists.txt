cmake_minimum_required(VERSION 3.6) 

set(THIS Messages)

list(APPEND THIS_INCLUDES_DIRS
            ${INCLUDES_DIR}/Messages
            ${INCLUDES_DIR}/Messages/Device_Managment_Messages
            ${INCLUDES_DIR}/Messages/Information_Reporting_Messages
            ${INCLUDES_DIR}/Messages/Registration_Messages
)

file(GLOB message_sources "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
file(GLOB device_managment_message_sources "${CMAKE_CURRENT_LIST_DIR}/Device_Managment_Messages/*.cpp")
file(GLOB information_reporting_message_sources "${CMAKE_CURRENT_LIST_DIR}/Information_Reporting_Messages/*.cpp")
file(GLOB registration_message_sources "${CMAKE_CURRENT_LIST_DIR}/Registration_Messages/*.cpp")

INCLUDE_DIRS_AND_FILES("${THIS_INCLUDES_DIRS}" THIS_INCLUDES)

add_library(${THIS})                

target_link_libraries(${THIS} PRIVATE 
                              Variant_Visitor::Variant_Visitor
)

target_sources(${THIS} PRIVATE 
                       ${THIS_INCLUDES}
                       ${message_sources}
                       ${device_managment_message_sources}
                       ${information_reporting_message_sources}
                       ${registration_message_sources}
)

target_include_directories(${THIS} INTERFACE
                           $<BUILD_INTERFACE:${INCLUDES_DIR}/Messages>
                           $<BUILD_INTERFACE:${INCLUDES_DIR}/Messages/Device_Managment_Messages>
                           $<BUILD_INTERFACE:${INCLUDES_DIR}/Messages/Information_Reporting_Messages>
                           $<BUILD_INTERFACE:${INCLUDES_DIR}/Messages/Registration_Messages>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/LwM2M>
)

target_compile_features(${THIS} PUBLIC cxx_std_17)

if(BUILD_SHARED_LIBS)
    set_property(TARGET ${THIS} PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

PRINT_TARGET_PROPERTIES(${THIS})

install(
    TARGETS ${THIS}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    FILES ${THIS_INCLUDES}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LwM2M
)
