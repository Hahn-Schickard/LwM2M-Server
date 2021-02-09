cmake_minimum_required(VERSION 3.6) 

set(THIS Binding_Interface)

list(APPEND THIS_INCLUDES_DIRS
            ${INCLUDES_DIR}/Binding_Interface
)

INCLUDE_DIRS_AND_FILES("${THIS_INCLUDES_DIRS}" THIS_INCLUDES)

add_library(${THIS} INTERFACE)

target_link_libraries(${THIS} INTERFACE
                              Device_Registry
)

target_include_directories(${THIS} INTERFACE
                           $<BUILD_INTERFACE:${INCLUDES_DIR}/Binding_Interface>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/LwM2M>
)

target_compile_features(${THIS} INTERFACE cxx_std_17)

install(
    TARGETS ${THIS}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    FILES ${THIS_INCLUDES}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LwM2M
)
