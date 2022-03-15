cmake_minimum_required(VERSION 3.6) 

set(THIS Server)
set(MODULE_NAME ${PROJECT_NAME}_${THIS})

list(APPEND THIS_INCLUDES_DIRS
            ${INCLUDES_DIR}/Server
)

file(GLOB sources_list "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

INCLUDE_DIRS_AND_FILES("${THIS_INCLUDES_DIRS}" THIS_INCLUDES)

add_library(${MODULE_NAME})                

target_sources(${MODULE_NAME}  PRIVATE 
                               ${THIS_INCLUDES}
                               ${sources_list}
)

get_property(BindingsList GLOBAL PROPERTY Bindings)

target_link_libraries(${MODULE_NAME} PUBLIC 
                                     ${PROJECT_NAME}_Binding_Interface
                                     HaSLL::HaSLL
                                     ${BindingsList}
                                     PRIVATE
                                     nlohmann_json::nlohmann_json                              
)

target_include_directories(${MODULE_NAME} INTERFACE
                           $<BUILD_INTERFACE:${INCLUDES_DIR}/Server>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
)

target_compile_features(${MODULE_NAME} PUBLIC cxx_std_17)

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
