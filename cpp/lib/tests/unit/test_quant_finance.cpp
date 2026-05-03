#include <gtest/gtest.h>

#include "quant_finance/quant_finance.hpp"

TEST(core_lib_test, scale_matrix)
{
    qf::matrix mat(3, 3);
    // clang-format off
        mat <<  1, 2, 3,
                4, 5, 6,
                7, 8, 9;
    // clang-format on

    qf::matrix expected(3, 3);
    // clang-format off
        expected << 5,  10, 15,
                    20, 25, 30,
                    35, 40, 45;
    // clang-format on

    qf::matrix result = qf::scale_matrix(mat, 5);
    EXPECT_TRUE(result.isApprox(expected));
}

TEST(core_lib_test, scale_matrix_by_zero)
{
    qf::matrix mat(2, 2);
    // clang-format off
    mat << 1, 2,
           3, 4;
    // clang-format on

    qf::matrix result = qf::scale_matrix(mat, 0);
    EXPECT_TRUE(result.isZero());
}

TEST(core_lib_test, scale_matrix_by_one)
{
    qf::matrix mat(2, 2);
    // clang-format off
    mat << 1.5, 2.5,
           3.5, 4.5;
    // clang-format on

    qf::matrix result = qf::scale_matrix(mat, 1);
    EXPECT_TRUE(result.isApprox(mat));
}
