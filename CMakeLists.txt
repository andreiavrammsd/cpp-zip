cmake_minimum_required(VERSION 3.12)
project(cpp_zip)
set(PROJECT_VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(warnings)

add_library(msd_zip INTERFACE)
target_compile_features(msd_zip INTERFACE cxx_std_17)
target_include_directories(msd_zip INTERFACE include)

option(ENABLE_TESTS "Build tests" OFF)
option(ENABLE_BENCHMARKS "Enable benchmark tests (needs ENABLE_TESTS=ON)" OFF)
option(ENABLE_COVERAGE "Generate test coverage" OFF)
option(ENABLE_ASAN "Include address sanitizer" OFF)

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

install(DIRECTORY include/ DESTINATION include)
