cmake_minimum_required(VERSION 3.6)

set(THIS Unit_Tests_Runner)

file(GLOB XML_Parser_Test_Suite "${CMAKE_CURRENT_LIST_DIR}/LwM2M/XML_Parser/*")

find_package(GTest REQUIRED)

add_executable(${THIS})
target_sources(${THIS} PRIVATE 
                       "testRunner.cpp"
                       ${XML_Parser_Test_Suite}
)

target_link_libraries(${THIS} PUBLIC
                              GTest::GTest
                              XML_Parser
)

add_test(
    NAME ${THIS}
    COMMAND ${THIS}
)

set_target_properties(${THIS} PROPERTIES
                              CXX_STANDARD 17
)
