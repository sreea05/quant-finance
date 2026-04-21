#include <pybind11/pybind11.h>

#include "core_lib/core_lib.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_core, m)
{
    m.doc() = "Test";
    py::module math = m.def_submodule("math", "math module");
    math.def("add", &core_lib::add);
    math.def("subtract", &core_lib::subtract);
    math.def("multiply", &core_lib::multiply);
    math.def("divide", &core_lib::divide);
}
