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

See [tests](tests/).

## Development

### Tools
* [SSH keys for GitHub](https://help.ubuntu.com/community/SSH/OpenSSH/Keys) (~/.ssh)
* [VS Code](https://code.visualstudio.com/) (see [.vscode/extensions.json](.vscode/extensions.json))
* [Docker](https://docs.docker.com/engine/install/ubuntu/)

### Actions
* Debug: F5
* Coverage: Ctrl + Shift + P -> Run Task -> Generate Coverage Report
* Show coverage inline: Ctrl + Shift + 7 OR Ctrl + Shift + P -> Coverage Gutters: Display Coverage
* Coverage as HTML: See build/coverage_html/index.html

## TODO

* Exception guarantees
* constexpr
* const correctness
* Write benchmarks
* Ignore google headers from clang-tidy
* ContainersAndAlgorithms test fails at `EXPECT_EQ(it, std::prev(const_zip.end()));` for std::list
    * Can the zip iterator really be bidirectional?
    * Document or conditionaly set the iterator tag by the containers types
* Consider checked access that returns an optional reference
* Do not allow to mix begin/end with cbegin/cend
* Run clang-tidy, clang-format in CI/file save
* Test
    * Analyze if LCOV_EXCL_LINE is needed
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
