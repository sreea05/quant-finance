#pragma once

#include <Eigen/Dense>

/// @file types.hpp
/// @brief Common linear algebra type aliases used throughout the quant_finance library.

namespace qf
{
    /// @brief A dynamic-size column vector of double-precision floating-point values.
    using vector = Eigen::VectorXd;

    /// @brief A dynamic-size matrix of double-precision floating-point values.
    using matrix = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
} // namespace qf
