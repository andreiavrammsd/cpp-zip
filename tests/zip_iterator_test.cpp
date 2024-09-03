#include <gtest/gtest.h>

#include <array>
#include <iterator>
#include <type_traits>
#include <vector>

#include "msd/zip.hpp"

class ZipIteratorTest : public testing::Test {
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

// GIVEN: A zip_iterator object is created with the beginning of three containers
// WHEN: Iterator traits are verified
// THEN: The traits should match the expected values for a bidirectional iterator with the appropriate types for value,
// pointer, and reference
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

// GIVEN: A zip_iterator object is created with the beginning of three containers
// WHEN: The dereference operator (*) is used to access the elements
// THEN: The values of the elements should match the expected values, and modifications should be reflected correctly
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

// GIVEN: Two zip_iterator objects pointing to the same container positions
// WHEN: The equality operator (==) is used
// THEN: The iterators should be equal, and iterators pointing to the same position should be comparable
TEST_F(ZipIteratorTest, OperatorEqual)
{
    const auto copy = begin_iterator_;
    EXPECT_EQ(copy, begin_iterator_);
    EXPECT_EQ(std::prev(end_iterator_), std::next(begin_iterator_));
}

// GIVEN: Two zip_iterator objects pointing to different container positions
// WHEN: The inequality operator (!=) is used
// THEN: The iterators should not be equal if they are pointing to different positions
TEST_F(ZipIteratorTest, OperatorNotEqual)
{
    EXPECT_NE(end_iterator_, begin_iterator_);
    EXPECT_NE(std::prev(end_iterator_), begin_iterator_);
}

// GIVEN: A zip_iterator object pointing to the beginning of the containers
// WHEN: The pre-increment operator (++) is used
// THEN: The iterator should advance to the end position
TEST_F(ZipIteratorTest, OperatorPreIncrement)
{
    ++begin_iterator_;
    ++begin_iterator_;

    EXPECT_EQ(begin_iterator_, end_iterator_);
}

// GIVEN: A zip_iterator object pointing to the end of the containers
// WHEN: The pre-decrement operator (--) is used to move the iterator backwards
// THEN: The iterator should correctly point to the beginning, and the elements should match the expected values
TEST_F(ZipIteratorTest, OperatorPreDecrement)
{
    auto iterator = std::prev(end_iterator_, 2);

    EXPECT_EQ(iterator, begin_iterator_);

    auto [a, b, c] = *iterator;
    EXPECT_EQ(a, 2);
    EXPECT_EQ(b, 4);
    EXPECT_EQ(c, 8);
}

// GIVEN: A zip_iterator object pointing to the beginning of the containers
// WHEN: The addition operator (+) is used with an offset
// THEN: The iterator should advance by the specified offset, and moving it by the length of the containers should reach
// the end iterator
TEST_F(ZipIteratorTest, OperatorPlusOffset)
{
    const auto end = std::next(begin_iterator_, 2);

    EXPECT_EQ(end, end_iterator_);
}

// GIVEN: A zip_iterator object pointing to the beginning of the containers
// WHEN: The addition operator (+) is used with an integer
// THEN: The iterator should advance by the specified integer offset, and the resulting iterator should match the
// expected value
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

// GIVEN: A zip_iterator object and an offset
// WHEN: The iterator plus another iterator (as offset) is computed
// THEN: The result should match the end iterator if the offset covers the entire length
TEST_F(ZipIteratorTest, OperatorPlusIterator)
{
    EXPECT_EQ(begin_iterator_ + std::next(begin_iterator_, 2), end_iterator_);
}

// GIVEN: A zip_iterator object pointing to the end of the containers
// WHEN: The subtraction operator (-) is used with an integer
// THEN: The iterator should move backwards by the specified integer offset, and moving it by the length of the
// containers should reach the beginning iterator
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

// GIVEN: A zip_iterator object and another iterator (as offset)
// WHEN: The iterator minus another iterator is computed
// THEN: The result should match the beginning iterator if the offset covers the entire length
TEST_F(ZipIteratorTest, OperatorMinusIterator)
{
    EXPECT_EQ(end_iterator_ - std::prev(end_iterator_, 2), begin_iterator_);
}

// GIVEN: A zip_iterator object pointing to the end of the containers
// WHEN: std::prev is used to move the iterator backwards by one position
// THEN: The resulting iterator should match the end iterator minus one position
TEST_F(ZipIteratorTest, Prev) { EXPECT_EQ(std::prev(end_iterator_), end_iterator_ - 1); }

// GIVEN: A zip_iterator object pointing to the beginning of the containers
// WHEN: std::next is used to move the iterator forwards by one position
// THEN: The resulting iterator should match the beginning iterator plus one position
TEST_F(ZipIteratorTest, Next) { EXPECT_EQ(std::next(begin_iterator_), begin_iterator_ + 1); }

// GIVEN: A zip_iterator object pointing to the beginning of the containers and the end iterator
// WHEN: std::advance is used to move the iterator forward and backward by the same amount
// THEN: The iterators should match after the advance operations
TEST_F(ZipIteratorTest, Advance)
{
    std::advance(begin_iterator_, 2);
    std::advance(end_iterator_, -1);
    EXPECT_EQ(begin_iterator_, end_iterator_);
}

// GIVEN: A zip_iterator object pointing to the beginning of the containers and the end iterator
// WHEN: std::distance is used to compute the distance between the two iterators
// THEN: The distance should match the expected number of elements between the iterators
TEST_F(ZipIteratorTest, Distance) { EXPECT_EQ(std::distance(begin_iterator_, end_iterator_), 2); }
