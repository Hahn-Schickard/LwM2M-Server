cmake_minimum_required(VERSION 3.6) 

set(THIS LwM2M_Server)

list(APPEND THIS_INCLUDES_DIRS
            ${INCLUDES_DIR}/LwM2M/Server
)

file(GLOB sources_list "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

INCLUDE_DIRS_AND_FILES("${THIS_INCLUDES_DIRS}" THIS_INCLUDES)

add_library(${THIS})                

target_sources(${THIS} PRIVATE 
                       ${THIS_INCLUDES}
                       ${sources_list}
)

target_link_libraries(${THIS} PUBLIC 
                              Stoppable_Task
                              Threads::Threads
                              PRIVATE
                              CoAP_Server
                              LwM2M_Model
                              LwM2M_Interfaces
                              Message_Converter
                              Threadsafe_Queue
                              HaSLL::HaSLL
)

target_include_directories(${THIS} INTERFACE
                           $<BUILD_INTERFACE:${INCLUDES_DIR}/LwM2M/Server>
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
