#include "msd/zip.hpp"

#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

#include "data.hpp"

class ZipTest : public testing::Test {
   protected:
    const std::array<int, 5> arr_three_{1, 2, 3};
    std::vector<int> vector_two_{4, 5};
    const std::vector<int> vector_four_{6, 7, 8, 9};

    using arr_three_type = decltype(arr_three_);
    using vector_two_type = decltype(vector_two_);
    using vector_four_type = decltype(vector_four_);

    msd::zip<arr_three_type, vector_two_type, vector_four_type> zip_{arr_three_, vector_two_, vector_four_};
    const msd::zip<arr_three_type, vector_two_type, vector_four_type> const_zip_{arr_three_, vector_two_, vector_four_};
};

// GIVEN: A zip object is created with three containers of different sizes
// WHEN: The begin() method is called and elements are accessed
// THEN: The first elements of each container should be returned correctly, and modifying a non-const zip reflects in
// subsequent const zip accesses
TEST_F(ZipTest, Begin)
{
    auto [a0, b0, c0] = *zip_.begin();
    EXPECT_EQ(a0, 1);
    EXPECT_EQ(b0, 4);
    EXPECT_EQ(c0, 6);

    b0 = 2;

    auto [ca0, cb0, cc0] = *const_zip_.begin();
    EXPECT_EQ(ca0, 1);
    EXPECT_EQ(cb0, 2);
    EXPECT_EQ(cc0, 6);

    EXPECT_EQ(zip_.begin() + 2, zip_.end());
}

// GIVEN: A zip object is created with three containers of different sizes
// WHEN: The end() method is called and the previous element is accessed
// THEN: The last elements of each container should be returned correctly, and the iterator arithmetic should correctly
// reflect the distance between begin() and end()
TEST_F(ZipTest, End)
{
    auto [a0, b0, c0] = *std::prev(zip_.end());
    EXPECT_EQ(a0, 2);
    EXPECT_EQ(b0, 5);
    EXPECT_EQ(c0, 7);

    auto [ca0, cb0, cc0] = *std::prev(const_zip_.end());
    EXPECT_EQ(ca0, 2);
    EXPECT_EQ(cb0, 5);
    EXPECT_EQ(cc0, 7);

    EXPECT_EQ(zip_.end() - 2, zip_.begin());
}

// GIVEN: A zip object is created with three containers of different sizes
// WHEN: The cbegin() method is called and elements are accessed
// THEN: The first elements of each container should be returned correctly in a const context, and the iterator
// arithmetic should match that of begin()
TEST_F(ZipTest, CBegin)
{
    auto [a0, b0, c0] = *zip_.cbegin();
    EXPECT_EQ(a0, 1);
    EXPECT_EQ(b0, 4);
    EXPECT_EQ(c0, 6);

    auto [ca0, cb0, cc0] = *const_zip_.cbegin();
    EXPECT_EQ(ca0, 1);
    EXPECT_EQ(cb0, 4);
    EXPECT_EQ(cc0, 6);

    EXPECT_EQ(zip_.cbegin() + 2, zip_.cend());
}

// GIVEN: A zip object is created with three containers of different sizes
// WHEN: The cend() method is called and the previous element is accessed
// THEN: The last elements of each container should be returned correctly in a const context, and the iterator
// arithmetic should match that of end()
TEST_F(ZipTest, CEnd)
{
    auto [a0, b0, c0] = *std::prev(zip_.cend());
    EXPECT_EQ(a0, 2);
    EXPECT_EQ(b0, 5);
    EXPECT_EQ(c0, 7);

    auto [ca0, cb0, cc0] = *std::prev(const_zip_.cend());
    EXPECT_EQ(ca0, 2);
    EXPECT_EQ(cb0, 5);
    EXPECT_EQ(cc0, 7);

    EXPECT_EQ(zip_.cend() - 2, zip_.cbegin());
}

// GIVEN: A zip object is created with containers of varying sizes
// WHEN: The size() method is called
// THEN: The size should match the smallest container's size
TEST_F(ZipTest, Size)
{
    EXPECT_EQ(zip_.size(), 2);
    EXPECT_EQ(const_zip_.size(), 2);
}

// GIVEN: A zip object is created with one non-empty container and one empty container
// WHEN: The size() method is called
// THEN: The size should be zero as one of the containers is empty
TEST_F(ZipTest, SizeWhenAContainerIsEmpty)
{
    const std::array<int, 5> non_empty{1, 2, 3};
    std::vector<int> empty{};

    EXPECT_EQ(msd::zip(non_empty, empty).size(), 0);
}

// GIVEN: A zip object is created with all containers being empty
// WHEN: The size() method is called
// THEN: The size should be zero
TEST_F(ZipTest, SizeWhenAllContainersAreEmpty)
{
    const std::vector<int> empty_1{};
    std::vector<int> empty_2{};

    EXPECT_EQ(msd::zip(empty_1, empty_2).size(), 0);
}

// GIVEN: A zip object is created with all containers having the same size
// WHEN: The size() method is called
// THEN: The size should match the common size of all containers
TEST_F(ZipTest, SizeWhenAllContainersHaveTheSameSize)
{
    const std::array<int, 5> array{1, 2, 3};
    std::vector<int> vector{1, 2, 3};

    EXPECT_EQ(msd::zip(array, vector).size(), 3);
}

// GIVEN: A zip object is created with non-empty containers
// WHEN: The empty() method is called
// THEN: The result should be false, indicating the zip is not empty
TEST_F(ZipTest, Empty)
{
    EXPECT_FALSE(zip_.empty());
    EXPECT_FALSE(const_zip_.empty());
}

// GIVEN: A zip object is created with one non-empty container and one empty container
// WHEN: The empty() method is called and an attempt to iterate over the zip is made
// THEN: The result should be true, and no iteration should occur
TEST_F(ZipTest, EmptyWhenAContainerIsEmpty)
{
    const std::array<int, 5> non_empty{1, 2, 3};
    std::vector<int> empty{};
    msd::zip zip(non_empty, empty);

    EXPECT_TRUE(zip.empty());

    for (auto ignore : zip) {
        std::ignore = ignore;
        FAIL();
    }
}

// GIVEN: A zip object is created with all containers being empty
// WHEN: The empty() method is called
// THEN: The result should be true, indicating the zip is empty
TEST_F(ZipTest, EmptyWhenAllContainersAreEmpty)
{
    const std::vector<int> empty_1{};
    std::vector<int> empty_2{};

    EXPECT_TRUE(msd::zip(empty_1, empty_2).empty());
}

// GIVEN: A zip object is created with non-empty containers
// WHEN: The operator bool() is used to check the zip object
// THEN: The result should be true, indicating the zip is valid
TEST_F(ZipTest, OperatorBool)
{
    if (!zip_) {
        FAIL();
    }

    EXPECT_TRUE(zip_);
    EXPECT_TRUE(const_zip_);
}

// GIVEN: A zip object is created with non-empty containers
// WHEN: The front() method is called and elements are accessed
// THEN: The first elements of each container should be returned correctly, and modifying one element should reflect in
// subsequent const accesses
TEST_F(ZipTest, Front)
{
    auto [a, b, c] = zip_.front();
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 4);
    EXPECT_EQ(c, 6);

    b = 10;

    auto [ca, cb, cc] = const_zip_.front();
    EXPECT_EQ(ca, 1);
    EXPECT_EQ(cb, 10);
    EXPECT_EQ(cc, 6);
}

// GIVEN: A zip object is created with one non-empty container and one empty container
// WHEN: The front() method is called
// THEN: The method should trigger a debug assertion failure due to the zip being empty
TEST_F(ZipTest, FrontWhenZipIsEmpty)
{
    const std::array<int, 5> non_empty{1, 2, 3};
    std::vector<int> empty{};

    msd::zip zip(non_empty, empty);
    EXPECT_DEBUG_DEATH(zip.front(), "");

    const msd::zip const_zip = zip;
    EXPECT_DEBUG_DEATH(const_zip.front(), "");
}

// GIVEN: A zip object is created with non-empty containers
// WHEN: The back() method is called and elements are accessed
// THEN: The last elements of each container should be returned correctly, and modifying one element should reflect in
// subsequent const accesses
TEST_F(ZipTest, Back)
{
    auto [a, b, c] = zip_.back();
    EXPECT_EQ(a, 2);
    EXPECT_EQ(b, 5);
    EXPECT_EQ(c, 7);

    b = 12;

    auto [ca, cb, cc] = const_zip_.back();
    EXPECT_EQ(ca, 2);
    EXPECT_EQ(cb, 12);
    EXPECT_EQ(cc, 7);
}

// GIVEN: A zip object is created with one non-empty container and one empty container
// WHEN: The back() method is called
// THEN: The method should trigger a debug assertion failure due to the zip being empty
TEST_F(ZipTest, BackWhenZipIsEmpty)
{
    const std::array<int, 5> non_empty{1, 2, 3};
    std::vector<int> empty{};
    msd::zip zip(non_empty, empty);
    const msd::zip const_zip(non_empty, empty);

    EXPECT_DEBUG_DEATH(zip.back(), "");
    EXPECT_DEBUG_DEATH(const_zip.back(), "");
}

// GIVEN: A zip object is created with non-empty containers
// WHEN: The operator[] is used with an index greater than the zip's size
// THEN: The elements at the specified index should be returned correctly, and modifying one element should reflect in
// subsequent const accesses
TEST_F(ZipTest, OperatorSubscript)
{
    auto [a, b, c] = zip_[0];
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 4);
    EXPECT_EQ(c, 6);

    b = 15;

    const auto value = const_zip_[0];
    EXPECT_EQ(std::get<0>(value), 1);
    EXPECT_EQ(std::get<1>(value), 15);
    EXPECT_EQ(std::get<2>(value), 6);
}

// GIVEN: A zip object is created with non-empty containers
// WHEN: The operator[] is used with an index greater than the zip's size
// THEN: The method should trigger a debug assertion failure due to the out-of-range access
TEST_F(ZipTest, OperatorSubscriptWithIndexOutOfRange)
{
    EXPECT_DEBUG_DEATH(zip_[99], "");
    EXPECT_DEBUG_DEATH(const_zip_[99], "");
}

// GIVEN: A zip object is created with containers holding custom data types
// WHEN: Iterating over the zip object
// THEN: No copies or moves of the contained objects should occur
TEST_F(ZipTest, NoCopiesAndMovesOfContainersWhileIterating)
{
    std::vector<data> items;
    items.reserve(5);
    items.emplace_back(1);
    items.emplace_back(2);
    items.emplace_back(3);
    items.emplace_back(4);
    items.emplace_back(5);

    std::size_t iterations = 0;
    for (auto [a, b] : msd::zip(vector_four_, items)) {
        std::ignore = a;
        std::ignore = b;
        static_assert(std::is_same_v<decltype(a), const int&>);
        static_assert(std::is_same_v<decltype(b), data&>);
        ++iterations;
    }

    EXPECT_EQ(iterations, 4);
    EXPECT_EQ(data::constructs_, 5);
    EXPECT_EQ(data::copies_, 0);
    EXPECT_EQ(data::moves_, 0);
}

// GIVEN: A zip object is created and then copied and moved
// WHEN: The size is checked after copying and moving
// THEN: The elements should remain consistent, reflecting the original zip's state
TEST_F(ZipTest, CopyAndMoveZip)
{
    auto copy = zip_;
    EXPECT_EQ(copy.size(), 2);
    for (std::size_t i = 0; i < zip_.size(); ++i) {
        auto [expected_a, expected_b, expected_c] = zip_[i];
        auto [actual_a, actual_b, actual_c] = copy[i];

        EXPECT_EQ(actual_a, expected_a);
        EXPECT_EQ(actual_b, expected_b);
        EXPECT_EQ(actual_c, expected_c);
    }

    const auto move = std::move(copy);
    EXPECT_EQ(move.size(), 2);
    for (std::size_t i = 0; i < zip_.size(); ++i) {
        auto [expected_a, expected_b, expected_c] = zip_[i];
        auto [actual_a, actual_b, actual_c] = move[i];

        EXPECT_EQ(actual_a, expected_a);
        EXPECT_EQ(actual_b, expected_b);
        EXPECT_EQ(actual_c, expected_c);
    }
}
