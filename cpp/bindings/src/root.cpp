#include "root.hpp"
#include "example.hpp"
#include "logger.hpp"

void add_root(py::module_& parent)
{
    add_logger(parent);
    add_example(parent);
}
