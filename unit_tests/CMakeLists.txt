cmake_minimum_required(VERSION 3.6)

set(THIS Unit_Tests_Runner)

file(GLOB XML_Parser_Test_Suite "${CMAKE_CURRENT_LIST_DIR}/LwM2M/XML_Parser/*")
file(GLOB ThreadsafeQueue_TestsSuite "${CMAKE_CURRENT_LIST_DIR}/ThreadsafeQueue_TestsSuite/*")
file(GLOB Observer_Pattern_TestSuite "${CMAKE_CURRENT_LIST_DIR}/Observer_Pattern_TestSuite/*")

find_package(GTest REQUIRED)

add_executable(${THIS})
target_sources(${THIS} PRIVATE 
                       "testRunner.cpp"
                       ${XML_Parser_Test_Suite}
                       ${ThreadsafeQueue_TestsSuite}
                       ${Observer_Pattern_TestSuite}
)

target_link_libraries(${THIS} PUBLIC
                              GTest::GTest
                              XML_Parser
                              Threadsafe_Queue
                              Observer_Pattern
                              CoAP_Message
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