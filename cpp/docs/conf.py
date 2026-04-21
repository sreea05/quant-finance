# Configuration file for the Sphinx documentation builder (C++ API).
#
# Breathe bridges Doxygen XML output into Sphinx so that C++ docs
# are rendered with the same tooling as the Python docs.

project = "quant_finance (C++)"
author = "sreea05"

extensions = [
    "breathe",
]

# -- Breathe configuration ---------------------------------------------------
breathe_projects = {"quant_finance": "../docs/xml"}
breathe_default_project = "quant_finance"

# -- Options for HTML output -------------------------------------------------
html_theme = "alabaster"
