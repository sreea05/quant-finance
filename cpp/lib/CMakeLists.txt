add_library(quant_finance STATIC src/quant_finance.cpp)

target_include_directories(quant_finance PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:inc>
)

target_compile_features(quant_finance PUBLIC cxx_std_20)
set_target_properties(quant_finance PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
