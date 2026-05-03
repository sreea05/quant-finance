#include <iostream>

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>

#include "quant_finance/example.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_core, m)
{
    m.doc() = "Test";
    py::module math = m.def_submodule("math", "math module");
    math.def("scale_matrix", &example::scale_matrix, py::return_value_policy::reference_internal);
}
