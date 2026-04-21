#include "quant_finance/quant_finance.hpp"

namespace quant_finance
{
    double add(double a, double b)
    {
        return a + b;
    }

    double subtract(double a, double b)
    {
        return a - b;
    }

    double multiply(double a, double b)
    {
        return a * b;
    }

    double divide(double a, double b)
    {
        // Check for exact zero to guard against division by zero.
        // Near-zero values are intentionally allowed and produce a very large result.
        if (b == 0.0)
        {
            throw std::invalid_argument("Division by zero");
        }
        return a / b;
    }
} // namespace quant_finance
