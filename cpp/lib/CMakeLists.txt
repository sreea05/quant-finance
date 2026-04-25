add_library(quant_finance STATIC src/quant_finance.cpp)

find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(quant_finance PRIVATE Eigen3::Eigen)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost REQUIRED COMPONENTS math)
target_link_libraries(quant_finance PRIVATE Boost::math)

target_include_directories(quant_finance PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:inc>
)

target_compile_features(quant_finance PUBLIC cxx_std_20)
set_target_properties(quant_finance PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
