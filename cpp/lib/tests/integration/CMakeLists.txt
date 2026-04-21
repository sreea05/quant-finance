add_executable(integration_tests test_core_lib_integration.cpp)

target_link_libraries(integration_tests
    PRIVATE
        core_lib
        GTest::gtest
        GTest::gtest_main
)
