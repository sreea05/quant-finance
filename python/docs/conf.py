# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import os
import sys

# Add the Python source directory to sys.path so autodoc can find modules
sys.path.insert(0, os.path.abspath("../src"))

# -- Project information -----------------------------------------------------
project = "quant_finance"
author = "sreea05"

# -- General configuration ---------------------------------------------------
extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.napoleon",
    "sphinx.ext.viewcode",
    "sphinx_autodoc_typehints",
]

# Napoleon settings (Google-style docstrings)
napoleon_google_docstring = True
napoleon_numpy_docstring = False

# Autodoc settings
autodoc_member_order = "bysource"
autodoc_mock_imports = ["quant_finance._core"]

# -- Options for HTML output -------------------------------------------------
html_theme = "alabaster"
