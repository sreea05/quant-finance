#include "quant_finance/quant_finance.hpp"

namespace qf
{
    qf::matrix scale_matrix(Eigen::Ref<const qf::matrix> mat, const double scale)
    {
        qf::matrix result = mat * scale;
        return result;
    }
} // namespace qf
