#include "msd/zip.hpp"

#include <gtest/gtest.h>

#include <array>
#include <iterator>
#include <vector>

#include "data.hpp"

class ZipTest : public testing::Test {  // NOLINT(readability-identifier-naming)
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

TEST_F(ZipTest, Begin)
{
    auto [a0, b0, c0] = *zip_.begin();
    EXPECT_EQ(a0, 1);
    EXPECT_EQ(b0, 4);
    EXPECT_EQ(c0, 6);

    auto [ca0, cb0, cc0] = *const_zip_.begin();
    EXPECT_EQ(ca0, 1);
    EXPECT_EQ(cb0, 4);
    EXPECT_EQ(cc0, 6);

    EXPECT_EQ(zip_.begin() + 2, zip_.end());
}

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

TEST_F(ZipTest, Size)
{
    EXPECT_EQ(zip_.size(), 2);
    EXPECT_EQ(const_zip_.size(), 2);
}

TEST_F(ZipTest, SizeWhenAContainerIsEmpty)
{
    const std::array<int, 5> non_empty{1, 2, 3};
    std::vector<int> empty{};

    EXPECT_EQ(msd::zip(non_empty, empty).size(), 0);
}

TEST_F(ZipTest, SizeWhenAllContainersAreEmpty)
{
    const std::vector<int> empty_1{};
    std::vector<int> empty_2{};

    EXPECT_EQ(msd::zip(empty_1, empty_2).size(), 0);
}

TEST_F(ZipTest, SizeWhenAllContainersHaveTheSameSize)
{
    const std::array<int, 5> array{1, 2, 3};
    std::vector<int> vector{1, 2, 3};

    EXPECT_EQ(msd::zip(array, vector).size(), 3);
}

TEST_F(ZipTest, Empty)
{
    EXPECT_FALSE(zip_.empty());
    EXPECT_FALSE(const_zip_.empty());
}

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

TEST_F(ZipTest, EmptyWhenAllContainersAreEmpty)
{
    const std::vector<int> empty_1{};
    std::vector<int> empty_2{};

    EXPECT_TRUE(msd::zip(empty_1, empty_2).empty());
}

TEST_F(ZipTest, OperatorBool)
{
    if (!zip_) {
        FAIL();
    }

    EXPECT_TRUE(zip_);
    EXPECT_TRUE(const_zip_);
}

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

TEST_F(ZipTest, FrontWhenZipIsEmpty)
{
    const std::array<int, 5> non_empty{1, 2, 3};
    std::vector<int> empty{};
    msd::zip zip(non_empty, empty);

    EXPECT_DEBUG_DEATH(zip.front(), "");
}

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

TEST_F(ZipTest, BackWhenZipIsEmpty)
{
    const std::array<int, 5> non_empty{1, 2, 3};
    std::vector<int> empty{};
    msd::zip zip(non_empty, empty);
    const msd::zip const_zip(non_empty, empty);

    EXPECT_DEBUG_DEATH(zip.back(), "");
    EXPECT_DEBUG_DEATH(const_zip.back(), "");
}

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

TEST_F(ZipTest, OperatorSubscriptWithIndexOutOfRange) { EXPECT_DEBUG_DEATH(zip_[99], ""); }

TEST_F(ZipTest, NoCopiesAndMoves)
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
        static_assert(std::is_same_v<decltype(a), const int&>);
        static_assert(std::is_same_v<decltype(b), data&>);
        ++iterations;
    }

    EXPECT_EQ(iterations, 4);
    EXPECT_EQ(data::constructs_, 5);
    EXPECT_EQ(data::copies_, 0);
    EXPECT_EQ(data::moves_, 0);
}
