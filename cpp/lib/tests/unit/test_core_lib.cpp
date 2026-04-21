#include <stdexcept>

#include <gtest/gtest.h>

#include "core_lib/core_lib.hpp"

TEST(CoreLibTest, Add)
{
    EXPECT_DOUBLE_EQ(core_lib::add(1.0, 2.0), 3.0);
    EXPECT_DOUBLE_EQ(core_lib::add(-1.0, 1.0), 0.0);
    EXPECT_DOUBLE_EQ(core_lib::add(0.0, 0.0), 0.0);
}

TEST(CoreLibTest, Subtract)
{
    EXPECT_DOUBLE_EQ(core_lib::subtract(5.0, 3.0), 2.0);
    EXPECT_DOUBLE_EQ(core_lib::subtract(0.0, 5.0), -5.0);
    EXPECT_DOUBLE_EQ(core_lib::subtract(3.0, 3.0), 0.0);
}

TEST(CoreLibTest, Multiply)
{
    EXPECT_DOUBLE_EQ(core_lib::multiply(3.0, 4.0), 12.0);
    EXPECT_DOUBLE_EQ(core_lib::multiply(-2.0, 3.0), -6.0);
    EXPECT_DOUBLE_EQ(core_lib::multiply(0.0, 100.0), 0.0);
}

TEST(CoreLibTest, Divide)
{
    EXPECT_DOUBLE_EQ(core_lib::divide(10.0, 2.0), 5.0);
    EXPECT_DOUBLE_EQ(core_lib::divide(7.0, 2.0), 3.5);
    EXPECT_DOUBLE_EQ(core_lib::divide(-6.0, 3.0), -2.0);
}

TEST(CoreLibTest, DivideByZero)
{
    EXPECT_THROW(core_lib::divide(1.0, 0.0), std::invalid_argument);
    EXPECT_THROW(core_lib::divide(-5.0, 0.0), std::invalid_argument);
}
