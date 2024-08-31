cmake_minimum_required(VERSION 3.12)
project(cpp_zip)
set(PROJECT_VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CXX_EXTENSIONS NO)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(warnings)

include_directories(include)

add_library(msd_zip INTERFACE)
target_include_directories(msd_zip INTERFACE include)

option(ENABLE_TESTS "Build tests" OFF)
option(ENABLE_BENCHMARKS "Enable benchmark tests (needs ENABLE_TESTS=ON)" OFF)
option(ENABLE_COVERAGE "Generate test coverage" OFF)
option(ENABLE_ASAN "Include address sanitizer" OFF)

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
