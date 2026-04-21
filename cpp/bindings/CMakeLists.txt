# --- Dependencies (only needed for the bindings target) ---
find_package(Python REQUIRED COMPONENTS Interpreter Development)

# pybind11 is installed via `uv sync --group dev` (pip package).
# Resolve its CMake config dir from the Python package.
execute_process(
    COMMAND "${Python_EXECUTABLE}" -m pybind11 --cmakedir
    OUTPUT_VARIABLE pybind11_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    COMMAND_ERROR_IS_FATAL ANY
)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(_core bindings.cpp)
target_link_libraries(_core PRIVATE core_lib)

# Copy extension to Python package for development (skipped during wheel builds).
# A custom target marked ALL runs on every build even when _core is up-to-date,
# so switching between Debug and Release always updates the extension.
# copy_if_different avoids touching the file when content hasn't changed.
set(_py_pkg_dir "${PROJECT_SOURCE_DIR}/../python/src/core_lib")
if(EXISTS "${_py_pkg_dir}")
    add_custom_target(copy_core_binding ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:_core>
            "${_py_pkg_dir}/$<TARGET_FILE_NAME:_core>"
        COMMENT "Copying _core extension to python/src/core_lib/"
        VERBATIM
    )
    add_dependencies(copy_core_binding _core)
endif()

# Install the extension into the core_lib package directory (used by scikit-build-core)
install(TARGETS _core DESTINATION core_lib)
