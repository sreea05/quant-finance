#include <gtest/gtest.h>

#include "quant_finance/example.hpp"
#include "quant_finance/math/types.hpp"

TEST(example, chain_scale_operations)
{
    qf::math::matrix mat(2, 2);
    // clang-format off
    mat << 1, 2,
           3, 4;
    // clang-format on

    // Scale by 2 then by 3 should equal a single scale by 6
    qf::math::matrix scaled_twice = example::scale_matrix(example::scale_matrix(mat, 2), 3);
    qf::math::matrix scaled_once = example::scale_matrix(mat, 6);

    EXPECT_TRUE(scaled_twice.isApprox(scaled_once));
}

TEST(example, scale_by_zero_produces_zero_matrix)
{
    qf::math::matrix mat(3, 3);
    // clang-format off
    mat << 1, 2, 3,
           4, 5, 6,
           7, 8, 9;
    // clang-format on

    qf::math::matrix result = example::scale_matrix(mat, 0);

    EXPECT_TRUE(result.isZero());
}

TEST(example, scale_by_one_preserves_matrix)
{
    qf::math::matrix mat(2, 3);
    // clang-format off
    mat << 1.5, 2.5, 3.5,
           4.5, 5.5, 6.5;
    // clang-format on

    qf::math::matrix result = example::scale_matrix(mat, 1);

    EXPECT_TRUE(result.isApprox(mat));
}
