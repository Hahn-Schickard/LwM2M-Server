cmake_minimum_required(VERSION 3.6) 

set(THIS CoAP_Binding)

list(APPEND THIS_INCLUDES_DIRS
            ${INCLUDES_DIR}/CoAP_Binding
)

file(GLOB sources_list "${CMAKE_CURRENT_LIST_DIR}/*.?pp")

INCLUDE_DIRS_AND_FILES("${THIS_INCLUDES_DIRS}" THIS_INCLUDES)

add_library(${THIS})                

target_sources(${THIS} PRIVATE 
                       ${THIS_INCLUDES}
                       ${sources_list}
)

target_link_libraries(${THIS} PUBLIC 
                              Binding_Interface
                              CoAPS4Cpp::CoAPS4Cpp
                              Variant_Visitor::Variant_Visitor
)

target_include_directories(${THIS} INTERFACE
                           $<BUILD_INTERFACE:${INCLUDES_DIR}/CoAP_Binding>
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
