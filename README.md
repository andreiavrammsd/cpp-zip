# ZIP

[![build](https://github.com/andreiavrammsd/cpp-zip/workflows/build/badge.svg)](https://github.com/andreiavrammsd/cpp-zip/actions)

[![codecov](https://codecov.io/github/andreiavrammsd/cpp-zip/graph/badge.svg?token=TBV8ID8QK0)](https://codecov.io/github/andreiavrammsd/cpp-zip)

## What

An implementation of [zip](https://en.cppreference.com/w/cpp/ranges/zip_view) for C++17. Single header file.

## Why

Learning purposes.

## Status

In development. Needs proper testing.

## Install

Integrate with CMake or just copy the [header file](include/msd/zip.hpp).

## Use

```c++
#include <msd/zip.hpp>

#include <algorithm>
#include <cassert>
#include <vector>

int main() {
    std::vector actual_numbers = {1, 2, 3};
    std::vector expected_numbers = {1, 2, 3};

    const std::size_t size = std::min(actual_numbers.size(), expected_numbers.size());
    for (std::size_t i = 0; i < size; ++i) {
        assert(actual_numbers[i] == expected_numbers[i]);
    }

    // vs

    for (auto [actual, expected] : msd::zip(actual_numbers, expected_numbers)) {
        assert(actual == expected);
    }
}

```

See [tests](tests/zip_test.cpp).

## TODO

* Write size() as end() - begin()
* Exception guarantees
* Set container and iterator traits
* constexpr
* Write benchmarks
* ContainersAndAlgorithms test fails at `EXPECT_EQ(it, std::prev(const_zip.end()));` for std::list
* Test
    * 100% coverage
    * Finish tests
    * With std algorithms
    * Entire API in non-const and const context
    * iterator operations: +, -, +=, -=, ++ pre/post, --pre/post, std::prev/next/advance, begin() + size() - begin(), etc.
    * Empty container
    * Two arrays of different sizes
    * Two vectors of different sizes
    * One array and one empty vector
    * copy, move operations
    * Improve tests with unordered containers
