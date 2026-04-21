#include <stdexcept>

#include <gtest/gtest.h>

#include "quant_finance/quant_finance.hpp"

TEST(CoreLibIntegration, ChainedArithmetic)
{
    // (10 + 5) * 3 / 5 - 2 = 7.0
    double result = quant_finance::add(10.0, 5.0);
    result = quant_finance::multiply(result, 3.0);
    result = quant_finance::divide(result, 5.0);
    result = quant_finance::subtract(result, 2.0);
    EXPECT_DOUBLE_EQ(result, 7.0);
}

TEST(CoreLibIntegration, DivideByZeroInChain)
{
    double result = quant_finance::subtract(5.0, 5.0); // produces 0.0
    EXPECT_THROW(quant_finance::divide(10.0, result), std::invalid_argument);
}

TEST(CoreLibIntegration, NegativeChain)
{
    // -3 * 4 + 12 = 0.0
    double result = quant_finance::multiply(-3.0, 4.0);
    result = quant_finance::add(result, 12.0);
    EXPECT_DOUBLE_EQ(result, 0.0);
}

TEST(CoreLibIntegration, LargeValueChain)
{
    double result = quant_finance::multiply(1e8, 1e8);
    result = quant_finance::divide(result, 1e8);
    EXPECT_DOUBLE_EQ(result, 1e8);
}
