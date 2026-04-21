# Configuration file for the Sphinx documentation builder (C++ API).
#
# Breathe bridges Doxygen XML output into Sphinx so that C++ docs
# are rendered with the same tooling as the Python docs.

project = "core_lib (C++)"
author = "sreea05"

extensions = [
    "breathe",
]

# -- Breathe configuration ---------------------------------------------------
breathe_projects = {"core_lib": "../docs/xml"}
breathe_default_project = "core_lib"

# -- Options for HTML output -------------------------------------------------
html_theme = "alabaster"
