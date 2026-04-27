cpp_dir   := "cpp"
py_dir    := "python"
venv_bin  := justfile_directory() / py_dir / ".venv" / "bin"
cmake_exe := venv_bin / "cmake"

# List available recipes
default:
    @just --list

# ── Dependencies ──────────────────────────────────────────────────────────────

# Install all dependencies
install:
    cd {{py_dir}} && uv sync
    cd {{cpp_dir}} && vcpkg install

# ── C++ build ─────────────────────────────────────────────────────────────────

# Build the C++ library and pybind11 bindings (debug)
cpp-build-debug:
    cd {{cpp_dir}} && "{{cmake_exe}}" --preset debug
    cd {{cpp_dir}} && "{{cmake_exe}}" --build --preset debug

# Build the C++ library and pybind11 bindings (release)
cpp-build-release:
    cd {{cpp_dir}} && "{{cmake_exe}}" --preset release
    cd {{cpp_dir}} && "{{cmake_exe}}" --build --preset release

# ── Testing ───────────────────────────────────────────────────────────────────

# Run C++ unit tests (debug)
cpp-test-unit-debug: cpp-build-debug
    {{cpp_dir}}/build/debug/lib/tests/unit/unit_tests

# Run C++ unit tests (release)
cpp-test-unit-release: cpp-build-release
    {{cpp_dir}}/build/release/lib/tests/unit/unit_tests

# Run C++ integration tests (debug)
cpp-test-integration-debug: cpp-build-debug
    {{cpp_dir}}/build/debug/lib/tests/integration/integration_tests

# Run C++ integration tests (release)
cpp-test-integration-release: cpp-build-release
    {{cpp_dir}}/build/release/lib/tests/integration/integration_tests

# Run all C++ tests (debug)
cpp-test-debug: cpp-test-unit-debug cpp-test-integration-debug

# Run all C++ tests (release)
cpp-test-release: cpp-test-unit-release cpp-test-integration-release

# Run C++ tests with coverage report (output: cpp/coverage/html/)
cpp-test-coverage: cpp-test-unit-debug cpp-test-integration-debug
    mkdir -p {{cpp_dir}}/coverage/html
    cd {{cpp_dir}} && "{{venv_bin}}/gcovr" --config gcovr/gcovr.cfg --root . --filter 'lib/src/' --filter 'lib/inc/' \
        --exclude-unreachable-branches --exclude-throw-branches --decisions \
        --html-template-dir gcovr/templates \
        --print-summary --html-details coverage/html/index.html --txt \
        --fail-under-line 100 --fail-under-branch 100 --fail-under-decision 100

# Run Python unit tests (debug)
py-test-unit-debug: cpp-build-debug
    cd {{py_dir}} && uv run pytest tests/unit/ -v

# Run Python unit tests (release)
py-test-unit-release: cpp-build-release
    cd {{py_dir}} && uv run pytest tests/unit/ -v

# Run Python integration tests (debug)
py-test-integration-debug: cpp-build-debug
    cd {{py_dir}} && uv run pytest tests/integration/ -v

# Run Python integration tests (release)
py-test-integration-release: cpp-build-release
    cd {{py_dir}} && uv run pytest tests/integration/ -v

# Run all Python tests (debug)
py-test-debug: py-test-unit-debug py-test-integration-debug

# Run all Python tests (release)
py-test-release: py-test-unit-release py-test-integration-release

# Run Python tests with coverage (output: python/coverage/html/)
py-test-coverage: cpp-build-release
    cd {{py_dir}} && uv run pytest tests/unit/ tests/integration/ -v --cov=quant_finance --cov-branch --cov-report=term-missing --cov-report=html:coverage/html --cov-fail-under=100

# ── Python package ───────────────────────────────────────────────────────────

# Build a distributable Python wheel
py-build-pkg:
    cd {{py_dir}} && uv build --wheel

# ── Lint / Format ─────────────────────────────────────────────────────────────

# Format Python code and sort imports (ruff)
py-format:
    cd {{py_dir}} && uv run ruff check --select I --fix src/ tests/
    cd {{py_dir}} && uv run ruff format src/ tests/

# Check Python formatting and import order (ruff)
py-format-check:
    cd {{py_dir}} && uv run ruff format --check src/ tests/
    cd {{py_dir}} && uv run ruff check --select I src/ tests/

# Fix Python lint issues (ruff)
py-lint-fix:
    cd {{py_dir}} && uv run ruff check --fix src/ tests/

# Check Python linting (ruff)
py-lint-check:
    cd {{py_dir}} && uv run ruff check src/ tests/

# Run Python type checker (mypy)
py-typecheck:
    cd {{py_dir}} && uv run mypy src/ tests/

# Format C++ code (clang-format)
cpp-format:
    "{{venv_bin}}/clang-format" -i -style=file:{{cpp_dir}}/.clang-format \
        $(find {{cpp_dir}} -name '*.cpp' -o -name '*.hpp' | grep -v build | grep -v vcpkg_installed)

# Check C++ formatting (clang-format)
cpp-format-check:
    "{{venv_bin}}/clang-format" --dry-run --Werror -style=file:{{cpp_dir}}/.clang-format \
        $(find {{cpp_dir}} -name '*.cpp' -o -name '*.hpp' | grep -v build | grep -v vcpkg_installed)

# ── Documentation ────────────────────────────────────────────────────────────

# Generate C++ API documentation (Doxygen + Sphinx/Breathe)
cpp-docs:
    cd {{cpp_dir}} && doxygen docs/Doxyfile
    cd {{cpp_dir}} && "{{venv_bin}}/sphinx-build" -b html docs docs/sphinx

# Generate Python documentation (Sphinx)
py-docs:
    cd {{py_dir}} && uv run sphinx-build -b html docs docs/_build

# ── Clean ─────────────────────────────────────────────────────────────────────

# Remove all artifacts
clean:
    git clean -Xdf
