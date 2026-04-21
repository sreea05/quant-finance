#include <pybind11/pybind11.h>

#include "quant_finance/quant_finance.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_core, m)
{
    m.doc() = "Test";
    py::module math = m.def_submodule("math", "math module");
    math.def("add", &quant_finance::add);
    math.def("subtract", &quant_finance::subtract);
    math.def("multiply", &quant_finance::multiply);
    math.def("divide", &quant_finance::divide);
}
