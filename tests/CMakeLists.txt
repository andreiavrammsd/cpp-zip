if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

# Testing framework
if (MSVC)
    option(
      gtest_force_shared_crt
      "Use shared (DLL) run-time lib even when Google Test is built as static lib."
      ON)
endif()

include(FetchContent)
if (NOT googletest_POPULATED)
    FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip)
    FetchContent_MakeAvailable(googletest)
endif ()

# Test macro
function(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})

    set_target_warnings(${TESTNAME} PRIVATE)
    target_link_libraries(${TESTNAME} gtest gtest_main gmock)

    if (ENABLE_COVERAGE)
        target_link_libraries(${TESTNAME} -lgcov -lubsan)
        target_compile_options(${TESTNAME} PRIVATE --coverage -fsanitize=undefined)

        add_custom_target(
            run_tests
            COMMAND ctest -C Debug --verbose
            DEPENDS tests
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )

        add_custom_target(
            coverage
            COMMAND lcov --directory . --capture --output-file coverage.info
                    && lcov --remove coverage.info '/usr/*' '*/tests/*' '*/external/*' '*gtest*' '*gmock*' --output-file coverage_filtered.info
                    && genhtml coverage_filtered.info --output-directory coverage_html
            DEPENDS run_tests
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    endif ()

    if (ENABLE_ASAN)
        target_link_libraries(${TESTNAME} -fsanitize=address)
        target_compile_options(${TESTNAME} PRIVATE -fsanitize=address)
    endif ()

    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})

    add_dependencies(tests ${TESTNAME})
endfunction()

add_custom_target(tests)

# Tests
package_add_test(zip_test zip_test.cpp zip_iterator_test.cpp zip_integration_test.cpp)

# Benchmark
if (ENABLE_BENCHMARKS)
    if (NOT benchmark_POPULATED)
        FetchContent_Declare(benchmark URL https://github.com/google/benchmark/archive/refs/tags/v1.8.5.zip)
        FetchContent_MakeAvailable(benchmark)
    endif ()

    add_executable(zip_benchmark zip_benchmark.cpp)
    target_link_libraries(zip_benchmark benchmark)
    set_target_warnings(zip_benchmark PRIVATE)
endif ()
