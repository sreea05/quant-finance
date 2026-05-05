#include <quant_finance/logger.hpp>

#include "logger.hpp"

void add_logger(py::module_& parent)
{
    py::module_ logger = parent.def_submodule("logger", "Logger");

    py::class_<qf::logger>(logger, "logger")
        .def_static("get", &qf::logger::get, py::return_value_policy::reference)
        .def("set_level", &qf::logger::set_level)
        .def("get_level", &qf::logger::get_level);

    py::enum_<qf::logger::level>(logger, "level")
        .value("TRACE", qf::logger::level::TRACE)
        .value("DEBUG", qf::logger::level::DEBUG)
        .value("INFO", qf::logger::level::INFO)
        .value("WARN", qf::logger::level::WARN)
        .value("ERR", qf::logger::level::ERR)
        .value("CRITICAL", qf::logger::level::CRITICAL)
        .value("OFF", qf::logger::level::OFF)
        .export_values();
}
