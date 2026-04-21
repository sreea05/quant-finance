#include <stdexcept>

#include <gtest/gtest.h>

#include "quant_finance/quant_finance.hpp"

TEST(CoreLibTest, Add)
{
    EXPECT_DOUBLE_EQ(quant_finance::add(1.0, 2.0), 3.0);
    EXPECT_DOUBLE_EQ(quant_finance::add(-1.0, 1.0), 0.0);
    EXPECT_DOUBLE_EQ(quant_finance::add(0.0, 0.0), 0.0);
}

TEST(CoreLibTest, Subtract)
{
    EXPECT_DOUBLE_EQ(quant_finance::subtract(5.0, 3.0), 2.0);
    EXPECT_DOUBLE_EQ(quant_finance::subtract(0.0, 5.0), -5.0);
    EXPECT_DOUBLE_EQ(quant_finance::subtract(3.0, 3.0), 0.0);
}

TEST(CoreLibTest, Multiply)
{
    EXPECT_DOUBLE_EQ(quant_finance::multiply(3.0, 4.0), 12.0);
    EXPECT_DOUBLE_EQ(quant_finance::multiply(-2.0, 3.0), -6.0);
    EXPECT_DOUBLE_EQ(quant_finance::multiply(0.0, 100.0), 0.0);
}

TEST(CoreLibTest, Divide)
{
    EXPECT_DOUBLE_EQ(quant_finance::divide(10.0, 2.0), 5.0);
    EXPECT_DOUBLE_EQ(quant_finance::divide(7.0, 2.0), 3.5);
    EXPECT_DOUBLE_EQ(quant_finance::divide(-6.0, 3.0), -2.0);
}

TEST(CoreLibTest, DivideByZero)
{
    EXPECT_THROW(quant_finance::divide(1.0, 0.0), std::invalid_argument);
    EXPECT_THROW(quant_finance::divide(-5.0, 0.0), std::invalid_argument);
}
