add_executable(integration_tests test_quant_finance_integration.cpp)

target_link_libraries(integration_tests
    PRIVATE
        quant_finance
        GTest::gtest
        GTest::gtest_main
)
