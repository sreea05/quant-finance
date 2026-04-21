#pragma once

#include <stdexcept>

/**
 * @brief Core arithmetic library.
 *
 * Provides basic double-precision arithmetic operations intended as a
 * starting point for a C++ library exposed to Python via pybind11.
 */
namespace core_lib
{

    /**
     * @brief Return the sum of @p a and @p b.
     * @param a First operand.
     * @param b Second operand.
     * @return a + b
     */
    double add(double a, double b);

    /**
     * @brief Return the difference of @p a and @p b.
     * @param a First operand.
     * @param b Second operand.
     * @return a - b
     */
    double subtract(double a, double b);

    /**
     * @brief Return the product of @p a and @p b.
     * @param a First operand.
     * @param b Second operand.
     * @return a * b
     */
    double multiply(double a, double b);

    /**
     * @brief Return the quotient of @p a divided by @p b.
     * @param a Dividend.
     * @param b Divisor. Must not be exactly zero.
     * @return a / b
     * @throws std::invalid_argument if @p b is zero.
     */
    double divide(double a, double b);

} // namespace core_lib
