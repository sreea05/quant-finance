#include <pybind11/eigen.h>
#include <quant_finance/example.hpp>

#include "example.hpp"

void add_example(py::module_& parent)
{
    py::module_ example = parent.def_submodule("example", "Example module");
    example.def("scale_matrix", &example::scale_matrix, pybind11::return_value_policy::reference_internal);
}
