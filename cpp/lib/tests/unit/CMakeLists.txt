add_executable(unit_tests test_core_lib.cpp)

target_link_libraries(unit_tests
    PRIVATE
        core_lib
        GTest::gtest
        GTest::gtest_main
)
