#include <gtest/gtest.h>

#include <array>
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
        iterator_{arr_three_.begin(), vector_two_.begin(), vector_four_.begin()};
};

TEST_F(ZipIteratorTest, IteratorTraits)
{
    using iterator_type = decltype(iterator_);

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
    std::tuple<const int&, int&, const int&> value = *iterator_;
    EXPECT_EQ(std::get<0>(value), 1);
    EXPECT_EQ(std::get<1>(value), 4);
    EXPECT_EQ(std::get<2>(value), 6);

    std::get<1>(value) += 1;

    std::tuple<const int&, int&, const int&> updated_value = *iterator_;
    EXPECT_EQ(std::get<0>(updated_value), 1);
    EXPECT_EQ(std::get<1>(updated_value), 5);
    EXPECT_EQ(std::get<2>(updated_value), 6);

    const std::tuple<const int&, int&, const int&> const_update_value = *iterator_;
    EXPECT_EQ(std::get<0>(const_update_value), 1);
    EXPECT_EQ(std::get<1>(const_update_value), 5);
    EXPECT_EQ(std::get<2>(const_update_value), 6);
}

TEST_F(ZipIteratorTest, OperatorEqual)
{
    const auto copy = iterator_;
    EXPECT_EQ(copy, iterator_);
}

TEST_F(ZipIteratorTest, OperatorNotEqual)
{
    msd::zip_iterator end_iterator{arr_three_.end(), vector_two_.end(), vector_four_.end()};

    EXPECT_NE(end_iterator, iterator_);
}

TEST_F(ZipIteratorTest, OperatorPreIncrement)
{
    msd::zip_iterator iterator{arr_three_.end(), vector_two_.end(), vector_four_.end()};

    ++iterator_;
    ++iterator_;

    EXPECT_EQ(iterator_, iterator);
}

TEST_F(ZipIteratorTest, OperatorPreDecrement)
{
    msd::zip_iterator iterator{arr_three_.end(), vector_two_.end(), vector_four_.end()};

    std::advance(iterator, -2);

    EXPECT_EQ(iterator, iterator_);
}

TEST_F(ZipIteratorTest, OperatorPlusOffset)
{
    msd::zip_iterator iterator{arr_three_.end(), vector_two_.end(), vector_four_.end()};

    const auto end = std::next(iterator_, 2);

    EXPECT_EQ(end, iterator);
}
