#include <stdexcept>

#include <gtest/gtest.h>

#include "core_lib/core_lib.hpp"

TEST(CoreLibIntegration, ChainedArithmetic)
{
    // (10 + 5) * 3 / 5 - 2 = 7.0
    double result = core_lib::add(10.0, 5.0);
    result = core_lib::multiply(result, 3.0);
    result = core_lib::divide(result, 5.0);
    result = core_lib::subtract(result, 2.0);
    EXPECT_DOUBLE_EQ(result, 7.0);
}

TEST(CoreLibIntegration, DivideByZeroInChain)
{
    double result = core_lib::subtract(5.0, 5.0); // produces 0.0
    EXPECT_THROW(core_lib::divide(10.0, result), std::invalid_argument);
}

TEST(CoreLibIntegration, NegativeChain)
{
    // -3 * 4 + 12 = 0.0
    double result = core_lib::multiply(-3.0, 4.0);
    result = core_lib::add(result, 12.0);
    EXPECT_DOUBLE_EQ(result, 0.0);
}

TEST(CoreLibIntegration, LargeValueChain)
{
    double result = core_lib::multiply(1e8, 1e8);
    result = core_lib::divide(result, 1e8);
    EXPECT_DOUBLE_EQ(result, 1e8);
}
