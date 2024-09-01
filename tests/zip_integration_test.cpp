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

TEST(ZipIntegrationTest, ContainersAndAlgorithms)
{
    std::vector<int> vector{1, 0};
    std::deque<int> deque{1, 2};
    std::list<int> list{1, 2, 3};
    std::forward_list<int> forward_list{1, 2, 3, 4};
    const std::array<int, 5> array{1, 2, 3, 4, 5};
    std::string string{"123456"};
    std::set<int> set{1, 2, 3, 4, 5, 6};
    std::multiset<int> multiset{1, 2, 3, 4, 5, 6};
    std::map<int, int> map{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
    std::multimap<int, int> multimap{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
    const auto zip = msd::zip(vector, deque, list, forward_list, array, string, set, multiset, map, multimap);

    const int sum = std::accumulate(zip.begin(), zip.end(), 0, [](int acc, auto&& tuple) {
        auto [vec, deq, li, fwd, arr, str, s, mset, mp, mm] = tuple;
        return acc + vec + deq + li + fwd + arr + str + s + mset + mp.second + mm.second;
    });
    EXPECT_EQ(sum, 124);

    const auto iterator_find = std::find_if(zip.cbegin(), zip.cend(), [](auto tuple) {
        auto [vec, deq, li, fwd, arr, str, s, mset, mp, mm] = tuple;
        return li == 4;
    });
    EXPECT_EQ(iterator_find, zip.cend());

    std::unordered_set<int> unordered_set{1, 9, -3};
    std::unordered_multiset<int> unordered_multiset{1, 2, 3, 4, 5, 6};
    std::unordered_map<int, int> unordered_map{{1, 1}, {2, 2}, {3, 3}, {45, 49}, {5, 5}, {6, 6}};
    std::unordered_multimap<int, int> unordered_multimap{{1, 1}, {2, 2}, {3, 3}, {4, 4}};

    const msd::zip unordered_zip(unordered_set, unordered_multiset, unordered_map, unordered_multimap);
    const bool any_is_negative = std::any_of(unordered_zip.begin(), unordered_zip.cend(), [](auto tuple) {
        auto [uset, umset, umap, umm] = tuple;
        return uset < 0 || umset < 0 || umap.second < 0 || umm.second < 0;
    });
    EXPECT_TRUE(any_is_negative);

    msd::zip adjacent_zip(list, forward_list);
    const auto adjacent_iterator =
        std::adjacent_find(adjacent_zip.begin(), adjacent_zip.end(), [](auto&& current, auto&& next) {
            auto&& [current_l, current_f] = current;
            auto&& [next_l, next_f] = next;

            return current_l + current_f + next_l + next_f == 6;
        });
    EXPECT_EQ(adjacent_iterator, adjacent_zip.begin());

    msd::zip zip2(list, set);
    auto iter = zip2.begin();

    std::size_t iterations = 0;
    while (iter != zip2.end()) {
        auto [a, b] = *iter;
        EXPECT_EQ(a, b);
        ++iterations;
        ++iter;
    }
    EXPECT_EQ(iterations, 3);

    iterations = 0;
    for (auto it = zip2.cbegin(); it != zip2.cend(); ++it) {
        ++iterations;
    }
    EXPECT_EQ(iterations, 3);

    iterations = 0;
    for (auto it = std::next(zip2.begin()); it != std::prev(zip2.end()); ++it) {
        ++iterations;
    }
    EXPECT_EQ(iterations, 1);
}
