#include <iostream>

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>

#include "root.hpp"

PYBIND11_MODULE(_core, m)
{
    m.doc() = "Test";
    add_root(m);
}
