cmake_minimum_required(VERSION 3.6)

set(THIS Unit_Tests_Runner)

file(GLOB Generic_Messages_Test_Suite "${CMAKE_CURRENT_LIST_DIR}/LwM2M/Messages/*")
file(GLOB Registration_Messages_Test_Suite "${CMAKE_CURRENT_LIST_DIR}/LwM2M/Messages/Registration_Messages/*")
file(GLOB Device_Model_Test_Suite "${CMAKE_CURRENT_LIST_DIR}/LwM2M/Device_Model/*")
file(GLOB XML_Parser_Test_Suite "${CMAKE_CURRENT_LIST_DIR}/LwM2M/XML_Parser/*")
file(GLOB Device_Registry_Test_Suite "${CMAKE_CURRENT_LIST_DIR}/LwM2M/Device_Registry/*")
file(GLOB Binding_Interface_Test_Suite "${CMAKE_CURRENT_LIST_DIR}/LwM2M/Binding_Interface/*")

find_package(GTest REQUIRED)

add_executable(${THIS})
target_sources(${THIS} PRIVATE
                       "testRunner.cpp"
                       ${Generic_Messages_Test_Suite}
                       ${Registration_Messages_Test_Suite}
                       ${Device_Model_Test_Suite}
                       ${XML_Parser_Test_Suite}
                       ${Device_Registry_Test_Suite}
                       ${Binding_Interface_Test_Suite}
)

target_link_libraries(${THIS} PUBLIC
                              GTest::gtest
                              GTest::gmock
                              Variant_Visitor::Variant_Visitor
                              ${PROJECT_NAME}_Messages
                              ${PROJECT_NAME}_Device_Model
                              ${PROJECT_NAME}_XML_Parser
                              ${PROJECT_NAME}_Device_Registry
                              ${PROJECT_NAME}_Binding_Interface
)

add_test(
    NAME ${THIS}
    COMMAND ${THIS}
)

set_target_properties(${THIS} PROPERTIES
                              CXX_STANDARD 17
)

add_custom_command(TARGET ${THIS} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_LIST_DIR}/LwM2M/XML_Parser/test_model $<TARGET_FILE_DIR:${THIS}>)
