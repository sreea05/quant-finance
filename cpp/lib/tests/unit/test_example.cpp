#include <gtest/gtest.h>

#include "quant_finance/example.hpp"
#include "quant_finance/math/types.hpp"

TEST(example, scale_matrix)
{
    qf::math::matrix mat(3, 3);
    // clang-format off
        mat <<  1, 2, 3,
                4, 5, 6,
                7, 8, 9;
    // clang-format on

    qf::math::matrix expected(3, 3);
    // clang-format off
        expected << 5,  10, 15,
                    20, 25, 30,
                    35, 40, 45;
    // clang-format on

    qf::math::matrix result = example::scale_matrix(mat, 5);
    EXPECT_TRUE(result.isApprox(expected));
}

TEST(example, scale_matrix_by_zero)
{
    qf::math::matrix mat(2, 2);
    // clang-format off
    mat << 1, 2,
           3, 4;
    // clang-format on

    qf::math::matrix result = example::scale_matrix(mat, 0);
    EXPECT_TRUE(result.isZero());
}

TEST(example, scale_matrix_by_one)
{
    qf::math::matrix mat(2, 2);
    // clang-format off
    mat << 1.5, 2.5,
           3.5, 4.5;
    // clang-format on

    qf::math::matrix result = example::scale_matrix(mat, 1);
    EXPECT_TRUE(result.isApprox(mat));
}
