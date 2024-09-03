#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <deque>
#include <forward_list>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "msd/zip.hpp"

class ZipIntegrationTest : public testing::Test {
   protected:
    std::vector<int> vector_{1, 0};
    std::deque<int> deque_{1, 2};
    std::list<int> list_{1, 2, 3};
    std::forward_list<int> forward_list_{1, 2, 3, 4};
    const std::array<int, 5> array_{1, 2, 3, 4, 5};
    std::string string_{"123456"};
    std::set<int> set_{1, 2, 3, 4, 5, 6};
    std::multiset<int> multiset_{1, 2, 3, 4, 5, 6};
    std::map<int, int> map_{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
    std::multimap<int, int> multimap_{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
    std::unordered_set<int> unordered_set_{1, 9, -3};
    std::unordered_multiset<int> unordered_multiset_{1, 2, 3, 4, 5, 6};
    std::unordered_map<int, int> unordered_map_{{1, 1}, {2, 2}, {3, 3}, {45, 49}, {5, 5}, {6, 6}};
    std::unordered_multimap<int, int> unordered_multimap_{{1, 1}, {2, 2}, {3, 3}, {4, 4}};
};

// GIVEN: Various container types
// WHEN: They are zipped together and std algorithms are used
// THEN: The zip must integrate well with the algorithms
TEST_F(ZipIntegrationTest, ContainersAndAlgorithms)
{
    const auto zip = msd::zip(vector_, deque_, list_, forward_list_, array_, string_, set_, multiset_, map_, multimap_);

    const int sum = std::accumulate(zip.begin(), zip.end(), 0, [](int acc, auto&& tuple) {
        auto [vec, deq, li, fwd, arr, str, s, mset, mp, mm] = tuple;
        return acc + vec + deq + li + fwd + arr + str + s + mset + mp.second + mm.second;
    });

    EXPECT_EQ(sum, 124);

    const auto iterator = std::find_if(zip.cbegin(), zip.cend(), [](auto tuple) {
        auto [vec, deq, li, fwd, arr, str, s, mset, mp, mm] = tuple;
        return li == 4;
    });

    EXPECT_EQ(iterator, zip.cend());
}

// GIVEN: Unordered containers
// WHEN: They are zipped together and std algorithms are used
// THEN: The zip must integrate well with the algorithms
TEST_F(ZipIntegrationTest, UnorderedContainersAndAlgorithms)
{
    const msd::zip zip(unordered_set_, unordered_multiset_, unordered_map_, unordered_multimap_);

    const bool any_is_negative = std::any_of(zip.cbegin(), zip.cend(), [](auto tuple) {
        auto [uset, umset, umap, umm] = tuple;
        return uset < 0 || umset < 0 || umap.second < 0 || umm.second < 0;
    });

    EXPECT_TRUE(any_is_negative);
}

// GIVEN: Lists and forward_lists
// WHEN: They are zipped together and std algorithms are used
// THEN: The zip must integrate well with the algorithms
TEST_F(ZipIntegrationTest, ListContainersAndAlgorithms)
{
    msd::zip zip(list_, forward_list_);

    const auto adjacent_iterator = std::adjacent_find(zip.begin(), zip.end(), [](auto&& current, const auto&& next) {
        auto&& [current_l, current_f] = current;
        auto&& [next_l, next_f] = next;

        return current_l + current_f + next_l + next_f == 6;
    });

    EXPECT_EQ(adjacent_iterator, zip.begin());
}

// GIVEN: Any containers
// WHEN: They are zipped together and classical iterations are used
// THEN: The zip must be iterated properly
TEST_F(ZipIntegrationTest, ClassicalIterations)
{
    msd::zip zip(deque_, array_);
    std::size_t iterations = 0;

    auto iter = zip.begin();
    while (iter != zip.end()) {
        auto [a, b] = *iter;
        EXPECT_EQ(a, b);
        ++iterations;
        ++iter;
    }
    EXPECT_EQ(iterations, 2);

    iterations = 0;
    for (auto it = zip.cbegin(); it != zip.cend(); ++it) {
        ++iterations;
    }
    EXPECT_EQ(iterations, 2);

    iterations = 0;
    for (auto it = std::next(zip.cbegin()); it != std::prev(zip.cend()); ++it) {
        ++iterations;
    }
    EXPECT_EQ(iterations, 0);
}
