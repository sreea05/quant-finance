#pragma once

#include <Eigen/Dense>

#include "quant_finance/math/types.hpp"

/**
 * @brief Example arithmetic operations on @ref qf::math::matrix types.
 *
 * Demonstrates basic double-precision matrix operations using the type aliases
 * defined in @ref qf::math.
 */
namespace example
{
    /**
     * @brief Scales every element of a matrix by a scalar factor.
     *
     * @param mat   The input @ref qf::math::matrix to scale. Passed as a read-only Eigen
     *              reference to avoid unnecessary copies.
     * @param scale The scalar value by which each element of @p mat is multiplied.
     * @return      A new @ref qf::math::matrix whose elements equal those of @p mat
     *              multiplied by @p scale.
     */
    qf::math::matrix scale_matrix(Eigen::Ref<const qf::math::matrix> mat, double scale);
} // namespace example
