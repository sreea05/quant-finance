# quant-finance

[![CI](https://github.com/sreea05/quant-finance/actions/workflows/ci.yml/badge.svg)](https://github.com/sreea05/quant-finance/actions/workflows/ci.yml)

A C++ quantitative finance library (`quant_finance`) exposed to Python via [pybind11](https://github.com/pybind/pybind11), built with [scikit-build-core](https://scikit-build-core.readthedocs.io) as the Python build backend and [vcpkg](https://vcpkg.io) for C++ package management.

## Table of contents

- [quant-finance](#quant-finance)
  - [Table of contents](#table-of-contents)
  - [Project structure](#project-structure)
  - [Prerequisites](#prerequisites)
  - [Quick start](#quick-start)
  - [Building](#building)
  - [Running tests](#running-tests)
    - [C++ tests (Google Test)](#c-tests-google-test)
    - [Python tests (pytest)](#python-tests-pytest)
  - [Test coverage](#test-coverage)
    - [C++ coverage (gcovr)](#c-coverage-gcovr)
    - [Python coverage (pytest-cov)](#python-coverage-pytest-cov)
  - [Lint and format](#lint-and-format)
    - [Python (ruff)](#python-ruff)
    - [C++ (clang-format)](#c-clang-format)
    - [Type checking (mypy)](#type-checking-mypy)
    - [Pre-commit hook](#pre-commit-hook)
  - [Documentation](#documentation)
    - [C++ API docs (Doxygen + Sphinx)](#c-api-docs-doxygen--sphinx)
    - [Python API docs (Sphinx)](#python-api-docs-sphinx)
  - [Building the Python package](#building-the-python-package)
  - [CI/CD](#cicd)
  - [Usage](#usage)
  - [Recipe reference](#recipe-reference)

## Project structure

```
quant-finance/
├── justfile                    # Task runner — `just <recipe>` for all commands
├── README.md
├── hooks/
│   └── pre-commit              # Git pre-commit hook (format + lint checks)
│
├── cpp/                        # Standalone C++ project
│   ├── CMakeLists.txt          # Root — project settings, vcpkg toolchain, subdirectories
│   ├── CMakePresets.json       # Presets: release and debug (with coverage)
│   ├── vcpkg.json              # C++ dependencies: gtest
│   ├── docs/               # Docs config: Doxygen + Sphinx/Breathe source
│   │   ├── Doxyfile            # Doxygen configuration for C++ API docs
│   │   ├── conf.py
│   │   ├── index.rst
│   │   └── api.rst
│   ├── gcovr_templates/        # Custom gcovr HTML templates (decision coverage)
│   ├── lib/
│   │   ├── CMakeLists.txt      # quant_finance static library
│   │   ├── inc/quant_finance/
│   │   │   └── quant_finance.hpp
│   │   ├── src/
│   │   │   └── quant_finance.cpp
│   │   └── tests/
│   │       ├── CMakeLists.txt
│   │       ├── unit/
│   │       │   ├── CMakeLists.txt
│   │       │   └── test_quant_finance.cpp
│   │       └── integration/
│   │           ├── CMakeLists.txt
│   │           └── test_quant_finance_integration.cpp
│   └── bindings/
│       ├── CMakeLists.txt      # _core pybind11 module (finds Python + pybind11)
│       └── bindings.cpp
│
└── python/                     # Python package (uv src layout)
    ├── pyproject.toml          # scikit-build-core backend; cmake + ninja + pybind11 dev deps
    ├── ruff.toml               # Ruff linter / formatter configuration
    ├── mypy.ini                # Mypy strict type-checking configuration
    ├── docs/                   # Sphinx documentation source (Python API)
    │   ├── conf.py
    │   ├── index.rst
    │   └── api.rst
    ├── src/
    │   └── quant_finance/
    │       ├── __init__.py     # Re-exports from _core
    │       └── math.py
    └── tests/
        ├── unit/
        │   └── test_quant_finance.py        # Unit tests (individual functions)
        └── integration/
            └── test_quant_finance.py        # Integration tests (module-level)
```

## Prerequisites

| Tool | Purpose | Install |
|------|---------|---------|
| **GCC / Clang** (C++20) | Compile C++ | System package manager |
| **uv** | Python + dev tooling | `curl -LsSf https://astral.sh/uv/install.sh \| sh` |
| **vcpkg** | C++ package manager | See [vcpkg Getting Started](https://vcpkg.io/en/getting-started) |
| **just** | Task runner | `cargo install just` or see [just docs](https://github.com/casey/just#installation) |
| **doxygen** *(optional)* | C++ API docs | System package manager (`apt install doxygen`, `brew install doxygen`) |

> **Note**: cmake, ninja, and pybind11 are **not** required system installs — they are installed into `python/.venv/` by `just install`.

## Quick start

```bash
git clone https://github.com/sreea05/quant-finance
cd quant-finance
```

> **Tip**: Set `VCPKG_ROOT` in your shell profile before running `just install`:
> `export VCPKG_ROOT=/path/to/vcpkg`

Install all dependencies, then build and test:

```bash
just install              # Python venv + C++ vcpkg packages
just cpp-build-release    # compile library + bindings, copy extension to python/
just cpp-test-release     # run C++ unit and integration tests
just py-test-release      # run Python tests (imports the C++ extension)
```

After `just cpp-build-release`, the compiled extension is copied into `python/src/quant_finance/` so `import quant_finance` works immediately.

## Building

A single build command compiles the C++ `quant_finance` static library **and** the `_core` pybind11 extension module. The extension is automatically copied into `python/src/quant_finance/` after build.

```bash
just cpp-build-release    # or just cpp-build-debug
```

**What happens**: CMake builds two targets from the `cpp/` tree:

1. `quant_finance` — static library (`cpp/lib/`)
2. `_core` — pybind11 extension module (`cpp/bindings/`), linked against `quant_finance`

A custom `copy_core_binding` target copies the compiled `_core*.so` into `python/src/quant_finance/` on every build, so Python can import it directly during development.

## Running tests

### C++ tests (Google Test)

```bash
just cpp-test-release     # or just cpp-test-debug
just cpp-test-unit-release       # unit tests only
just cpp-test-integration-release # integration tests only
```

### Python tests (pytest)

```bash
just py-test-release      # or just py-test-debug
just py-test-unit-release          # unit tests only
just py-test-integration-release   # integration tests only
```

The `py-test-*` recipes depend on `cpp-build-*`, so the C++ extension is always built (and copied) before running tests.

## Test coverage

### C++ coverage (gcovr)

```bash
just cpp-test-coverage
```

Builds with `--coverage` flags (debug CMake preset includes gcov instrumentation), runs all C++ tests, then produces an HTML report at `cpp/coverage/html/index.html` and a terminal summary. Fails if line, branch, or decision coverage is below 100%.

### Python coverage (pytest-cov)

```bash
just py-test-coverage
```

Runs pytest with branch coverage on the `quant_finance` package. HTML report is written to `python/coverage/html/`. Fails if line or branch coverage is below 100%.

## Lint and format

### Python (ruff)

```bash
just py-format           # auto-format and sort imports
just py-format-check     # check formatting and import order (CI)
just py-lint-fix         # auto-fix lint issues
just py-lint-check       # check lint (CI)
```

### C++ (clang-format)

```bash
just cpp-format          # auto-format
just cpp-format-check    # check formatting (CI)
```

### Type checking (mypy)

```bash
just py-typecheck        # strict type-checking on src/ and tests/
```

Mypy is configured in `python/mypy.ini` with `strict = True`. The `quant_finance._core` C extension module is excluded from import checks since it is generated at build time.

### Pre-commit hook

A `hooks/pre-commit` script runs `py-format-check`, `py-lint-check`, and `cpp-format-check`. To enable it:

```bash
ln -sf ../../hooks/pre-commit .git/hooks/pre-commit
```

## Documentation

### C++ API docs (Doxygen + Sphinx)

C++ headers use Doxygen-style `/** @brief ... */` comments. Documentation is generated in two steps: Doxygen produces XML, then Sphinx + [Breathe](https://breathe.readthedocs.io/) renders it as HTML:

```bash
just cpp-docs
```

Output is written to `cpp/docs/sphinx/` (git-ignored). Open `cpp/docs/sphinx/index.html` to browse.

### Python API docs (Sphinx)

Python documentation is generated with [Sphinx](https://www.sphinx-doc.org/) using autodoc, Napoleon (Google-style docstrings), and `sphinx-autodoc-typehints`. Source files live in `python/docs/`.

```bash
just py-docs
```

Output is written to `python/docs/_build/` (git-ignored). Open `python/docs/_build/index.html` to browse.

## Building the Python package

To produce a distributable `.whl` file, the package is built via [scikit-build-core](https://scikit-build-core.readthedocs.io). scikit-build-core invokes CMake internally, pointing at the `cpp/` source tree (configured in `pyproject.toml` via `cmake.source-dir = "../cpp"`), compiles the `_core` extension, and packages everything into a wheel.

```bash
just py-build-pkg
```

The resulting wheel is written to `python/dist/` and can be installed anywhere with `pip install python/dist/quant_finance-*.whl`.

> **Note**: The `--wheel` flag (used internally by `py-build-pkg`) is required because the C++ source tree lives outside `python/` (at `../cpp`). Building a wheel directly from the source tree works because the relative path resolves correctly, whereas an sdist-based build would fail since the sdist doesn't include the `cpp/` directory.

> **Build caching**: `pyproject.toml` sets `build-dir = "build/release"`, so scikit-build-core reuses a persistent build directory. The first `py-build-pkg` runs a full CMake build; subsequent runs are incremental and only recompile changed sources.

## CI/CD

A GitHub Actions workflow (`.github/workflows/ci.yml`) runs on every push to `main` and on pull requests. The pipeline:

1. Installs all dependencies (uv, just, vcpkg)
2. Checks C++ formatting (clang-format)
3. Checks Python formatting and import order (ruff)
4. Checks Python linting (ruff)
5. Runs Python type checking (mypy)
6. Builds the C++ library and bindings (debug + release)
7. Runs C++ tests with coverage (debug build, enforces 100%)
8. Runs Python tests with coverage (enforces 100%)

## Usage

```python
from quant_finance import add, subtract, multiply, divide

print(add(1.0, 2.0))       # 3.0
print(subtract(5.0, 3.0))  # 2.0
print(multiply(3.0, 4.0))  # 12.0
print(divide(10.0, 2.0))   # 5.0

# divide raises ValueError on exact zero (mapped from C++ std::invalid_argument)
try:
    divide(1.0, 0.0)
except ValueError as e:
    print(e)               # Division by zero
```

## Recipe reference

| Recipe | Description |
|--------|-------------|
| `just install` | Install Python venv (uv) and C++ packages (vcpkg) |
| `just cpp-build-release` | Build library + bindings, copy extension to python/ (release) |
| `just cpp-build-debug` | Build library + bindings, copy extension to python/ (debug, with coverage) |
| `just cpp-test-release` | Build and run all C++ tests (release) |
| `just cpp-test-debug` | Build and run all C++ tests (debug) |
| `just cpp-test-unit-release` | Run C++ unit tests only (release) |
| `just cpp-test-unit-debug` | Run C++ unit tests only (debug) |
| `just cpp-test-integration-release` | Run C++ integration tests only (release) |
| `just cpp-test-integration-debug` | Run C++ integration tests only (debug) |
| `just cpp-test-coverage` | Run all C++ tests with debug build, produce gcovr report |
| `just py-test-release` | Build extension and run Python tests (release) |
| `just py-test-debug` | Build extension and run Python tests (debug) |
| `just py-test-unit-release` | Run Python unit tests only (release) |
| `just py-test-unit-debug` | Run Python unit tests only (debug) |
| `just py-test-integration-release` | Run Python integration tests only (release) |
| `just py-test-integration-debug` | Run Python integration tests only (debug) |
| `just py-test-coverage` | Run Python tests with coverage report |
| `just py-build-pkg` | Build a distributable Python wheel (`.whl`) into `python/dist/` |
| `just py-format` | Auto-format Python code and sort imports |
| `just py-format-check` | Check Python formatting and import order |
| `just py-lint-fix` | Auto-fix Python lint issues |
| `just py-lint-check` | Check Python linting |
| `just py-typecheck` | Run mypy strict type checking on Python code |
| `just cpp-format` | Auto-format C++ code |
| `just cpp-format-check` | Check C++ formatting |
| `just cpp-docs` | Generate C++ API documentation (Doxygen + Sphinx) |
| `just py-docs` | Generate Python API documentation (Sphinx) |
| `just clean` | Remove all build artifacts |

Run `just` with no arguments to list all available recipes.


