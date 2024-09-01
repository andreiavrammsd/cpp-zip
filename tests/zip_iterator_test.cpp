#include <gtest/gtest.h>

#include <array>
#include <iterator>
#include <type_traits>
#include <vector>

#include "msd/zip.hpp"

class ZipIteratorTest : public testing::Test {  // NOLINT(readability-identifier-naming)
   protected:
    const std::array<int, 3> arr_three_{1, 2, 3};
    std::vector<int> vector_two_{4, 5};
    const std::vector<int> vector_four_{6, 7, 8, 9};

    using arr_three_type = decltype(arr_three_);
    using vector_two_type = decltype(vector_two_);
    using vector_four_type = decltype(vector_four_);

    msd::zip_iterator<arr_three_type::const_iterator, vector_two_type::iterator, vector_four_type::const_iterator>
        begin_iterator_{arr_three_.begin(), vector_two_.begin(), vector_four_.begin()};

    msd::zip_iterator<arr_three_type::const_iterator, vector_two_type::iterator, vector_four_type::const_iterator>
        end_iterator_{arr_three_.end(), vector_two_.end(), vector_four_.end()};
};

TEST_F(ZipIteratorTest, IteratorTraits)
{
    using iterator_type = decltype(begin_iterator_);

    static_assert(std::is_same_v<iterator_type::iterator_category, std::bidirectional_iterator_tag>);
    static_assert(std::is_same_v<iterator_type::difference_type, std::ptrdiff_t>);
    static_assert(std::is_same_v<iterator_type::value_type,
                                 std::tuple<arr_three_type::const_reference, vector_two_type::reference,
                                            vector_four_type::const_reference>>);
    static_assert(
        std::is_same_v<iterator_type::pointer, std::tuple<arr_three_type::const_pointer, vector_two_type::pointer,
                                                          vector_four_type::const_pointer>>);
    static_assert(
        std::is_same_v<iterator_type::reference, std::tuple<arr_three_type::const_reference, vector_two_type::reference,
                                                            vector_four_type::const_reference>>);
}

TEST_F(ZipIteratorTest, OperatorDereference)
{
    std::tuple<const int&, int&, const int&> value = *begin_iterator_;
    EXPECT_EQ(std::get<0>(value), 1);
    EXPECT_EQ(std::get<1>(value), 4);
    EXPECT_EQ(std::get<2>(value), 6);

    std::get<1>(value) += 1;

    std::tuple<const int&, int&, const int&> updated_value = *begin_iterator_;
    EXPECT_EQ(std::get<0>(updated_value), 1);
    EXPECT_EQ(std::get<1>(updated_value), 5);
    EXPECT_EQ(std::get<2>(updated_value), 6);

    const std::tuple<const int&, int&, const int&> const_update_value = *begin_iterator_;
    EXPECT_EQ(std::get<0>(const_update_value), 1);
    EXPECT_EQ(std::get<1>(const_update_value), 5);
    EXPECT_EQ(std::get<2>(const_update_value), 6);
}

TEST_F(ZipIteratorTest, OperatorEqual)
{
    const auto copy = begin_iterator_;
    EXPECT_EQ(copy, begin_iterator_);
}

TEST_F(ZipIteratorTest, OperatorNotEqual) { EXPECT_NE(end_iterator_, begin_iterator_); }

TEST_F(ZipIteratorTest, OperatorPreIncrement)
{
    ++begin_iterator_;
    ++begin_iterator_;

    EXPECT_EQ(begin_iterator_, end_iterator_);
}

TEST_F(ZipIteratorTest, OperatorPreDecrement)
{
    auto iterator = std::prev(end_iterator_, 2);

    EXPECT_EQ(iterator, begin_iterator_);

    auto [a, b, c] = *iterator;
    EXPECT_EQ(a, 2);
    EXPECT_EQ(b, 4);
    EXPECT_EQ(c, 8);
}

TEST_F(ZipIteratorTest, OperatorPlusOffset)
{
    const auto end = std::next(begin_iterator_, 2);

    EXPECT_EQ(end, end_iterator_);
}

TEST_F(ZipIteratorTest, OperatorPlus)
{
    const auto iterator = begin_iterator_ + 1;

    auto [a, b, c] = *iterator;
    EXPECT_EQ(a, 2);
    EXPECT_EQ(b, 5);
    EXPECT_EQ(c, 7);

    const auto end = begin_iterator_ + 3;
    EXPECT_EQ(end, end_iterator_);
}

TEST_F(ZipIteratorTest, OperatorMinus)
{
    const auto iterator = end_iterator_ - 1;

    auto [a, b, c] = *iterator;
    EXPECT_EQ(a, 3);
    EXPECT_EQ(b, 5);
    EXPECT_EQ(c, 9);

    const auto begin = end_iterator_ - 2;
    EXPECT_EQ(begin, begin_iterator_);
}
