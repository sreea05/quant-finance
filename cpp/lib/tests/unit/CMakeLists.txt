add_executable(unit_tests test_quant_finance.cpp)

target_link_libraries(unit_tests
    PRIVATE
        quant_finance
        GTest::gtest
        GTest::gtest_main
)
