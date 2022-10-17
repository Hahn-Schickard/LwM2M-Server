cmake_minimum_required(VERSION 3.6) 

set(THIS CoAP_Binding)
set(MODULE_NAME ${PROJECT_NAME}_${THIS})

ADD_NEW_BINDING(${MODULE_NAME})

list(APPEND THIS_INCLUDES_DIRS
            ${INCLUDES_DIR}/CoAP_Binding
)

file(GLOB sources_list "${CMAKE_CURRENT_LIST_DIR}/*.?pp")

INCLUDE_DIRS_AND_FILES("${THIS_INCLUDES_DIRS}" THIS_INCLUDES)

add_library(${MODULE_NAME})                

target_sources(${MODULE_NAME} PRIVATE 
                              ${THIS_INCLUDES}
                              ${sources_list}
)

target_link_libraries(${MODULE_NAME} PUBLIC 
                                     ${PROJECT_NAME}_Binding_Interface
                                     CoAPS4Cpp::CoAPS4Cpp
                                     Variant_Visitor::Variant_Visitor
                                     HaSLL::HaSLL
                                     HSCUL::HSCUL
)

target_include_directories(${MODULE_NAME} INTERFACE
                           $<BUILD_INTERFACE:${INCLUDES_DIR}/CoAP_Binding>
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
