cmake_minimum_required(VERSION 3.6) 

set(THIS Runner)

file(GLOB sources_list "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

add_executable(${THIS})

target_sources(${THIS} PRIVATE 
                       ${sources_list}
)

target_link_libraries(${THIS} PRIVATE
                              Variant_Visitor::Variant_Visitor
                              Server
                              HaSLL::HaSLL
                              Threads::Threads
)

target_compile_features(${THIS} PUBLIC cxx_std_17)

add_custom_command(TARGET ${THIS} POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${PROJECT_SOURCE_DIR}/config
                        $<TARGET_FILE_DIR:${THIS}>/config 
                    COMMENT "Copying LwM2M Model descriptors."
                        VERBATIM
)