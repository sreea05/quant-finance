#include "quant_finance/example.hpp"

namespace example
{
    qf::math::matrix scale_matrix(Eigen::Ref<const qf::math::matrix> mat, const double scale)
    {
        qf::math::matrix result = mat * scale;
        return result;
    }
} // namespace example
