add_library(core_lib STATIC src/core_lib.cpp)

target_include_directories(core_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:inc>
)

target_compile_features(core_lib PUBLIC cxx_std_20)
set_target_properties(core_lib PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
