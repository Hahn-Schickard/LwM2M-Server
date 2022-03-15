cmake_minimum_required(VERSION 3.6) 

set(THIS Messages)
set(MODULE_NAME ${PROJECT_NAME}_${THIS})

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

add_library(${MODULE_NAME})                

target_link_libraries(${MODULE_NAME} PRIVATE 
                                     Variant_Visitor::Variant_Visitor
                                     HSCUL::HSCUL
)

target_sources(${MODULE_NAME} PRIVATE 
                              ${THIS_INCLUDES}
                              ${message_sources}
                              ${device_managment_message_sources}
                              ${information_reporting_message_sources}
                              ${registration_message_sources}
)

target_include_directories(${MODULE_NAME} INTERFACE
                           $<BUILD_INTERFACE:${INCLUDES_DIR}/Messages>
                           $<BUILD_INTERFACE:${INCLUDES_DIR}/Messages/Device_Managment_Messages>
                           $<BUILD_INTERFACE:${INCLUDES_DIR}/Messages/Information_Reporting_Messages>
                           $<BUILD_INTERFACE:${INCLUDES_DIR}/Messages/Registration_Messages>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
)

target_compile_features(${MODULE_NAME} PUBLIC cxx_std_17)

# if(BUILD_SHARED_LIBS)
#     set_property(TARGET ${MODULE_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
# endif()

PRINT_TARGET_PROPERTIES(${MODULE_NAME})

install(
    TARGETS ${MODULE_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    FILES ${THIS_INCLUDES}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)
