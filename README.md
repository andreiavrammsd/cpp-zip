# ZIP

[![build](https://github.com/andreiavrammsd/cpp-zip/workflows/build/badge.svg)](https://github.com/andreiavrammsd/cpp-zip/actions)
&nbsp;&nbsp;[![codecov](https://codecov.io/github/andreiavrammsd/cpp-zip/graph/badge.svg?token=TBV8ID8QK0)](https://codecov.io/github/andreiavrammsd/cpp-zip)
&nbsp;&nbsp;[![documentation](https://img.shields.io/badge/msd::zip-Documentation-4EC820?labelColor=5E5E5E)](https://andreiavrammsd.github.io/cpp-zip/)

## What

An implementation of [zip](https://en.cppreference.com/w/cpp/ranges/zip_view) for C++17. Single header file.

## Why

Learning purposes.

## Status

In development. Needs proper testing.

## Install

Integrate with CMake or just copy the [header file](https://github.com/andreiavrammsd/cpp-zip/tree/master/include/msd/zip.hpp).

## Usage and Documentation

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

For more, see [tests](https://github.com/andreiavrammsd/cpp-zip/blob/master/tests) and [documentation](https://andreiavrammsd.github.io/cpp-zip/).

## Known issues

### Calling std::prev on an msd::zip object compiles, but fails at runtime on some std containers.

* list
* forward_list
* unordered_set
* unordered_multiset
* unordered_map
* unordered_multimap

The `msd::zip` iterator is bidirectional to better control when you want to stop iterating.

To get only some of the elements from a C++23 zip, you can use `std::views::take`:
```c++
std::forward_list<int> first_list{1, 2, 3, 4, 5};
std::forward_list<int> second_list{1, 2, 3, 4};
auto forward_zip = std::views::zip(first_list, second_list);

for (auto [a, b] : forward_zip | std::views::take(3)) {
    std::cout << a << ", " << b << "\n";
}
```

With `msd::zip`, you can write:
```c++
for (auto it = forward_zip.begin(); it != std::prev(forward_zip.end()); ++it) {
    auto [a, b] = *it;
    std::cout << a << ", " << b << "\n";
}
```
But it does not work with the mentioned containers.

## Development

### Tools
* [SSH keys for GitHub](https://help.ubuntu.com/community/SSH/OpenSSH/Keys) (~/.ssh)
* [VS Code](https://code.visualstudio.com/) (see [.vscode/extensions.json](https://github.com/andreiavrammsd/cpp-zip/blob/master/.vscode/extensions.json))
* [Docker](https://docs.docker.com/engine/install/ubuntu/)

### Actions
* Debug: F5
* Coverage: Ctrl + Shift + P -> Run Task -> Generate Coverage Report
  * Show coverage inline: Ctrl + Shift + 7 OR Ctrl + Shift + P -> Coverage Gutters: Display Coverage
  * Coverage as HTML: See build/coverage_html/index.html
* Clang tidy: Ctrl + Shift + P -> Run Task -> Run clang-tidy on current file
* Documentation: Ctrl + Shift + P -> Run Task -> Generate Documentation

## TODO

* Set up cache for release workflow (apt cache)
* Exception guarantees
* constexpr
* const correctness
* Write benchmarks
* Ignore google headers from clang-tidy
* Consider checked access that returns an optional reference
* Run clang-tidy on file save
* Cancel running jobs on new commit
* Test
    * Split integration test into smaller ones
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
