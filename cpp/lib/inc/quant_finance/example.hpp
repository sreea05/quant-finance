#pragma once

#include <Eigen/Dense>

#include "quant_finance/math/types.hpp"

/**
 * @brief Core arithmetic library.
 *
 * Provides basic double-precision arithmetic operations intended as a
 * starting point for a C++ library exposed to Python via pybind11.
 */
namespace example
{
    /**
     * @brief Scales every element of a matrix by a scalar factor.
     *
     * @param mat   The input matrix to scale. Passed as a read-only Eigen reference
     *              to avoid unnecessary copies.
     * @param scale The scalar value by which each element of @p mat is multiplied.
     * @return      A new matrix whose elements equal those of @p mat multiplied by @p scale.
     */
    qf::math::matrix scale_matrix(Eigen::Ref<const qf::math::matrix> mat, double scale);
} // namespace example
