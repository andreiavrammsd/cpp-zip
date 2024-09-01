#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <deque>
#include <forward_list>
#include <iterator>
#include <list>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "msd/zip.hpp"

TEST(ZipIntegrationTest, ContainersAndAlgorithms)
{
    std::vector<int> vector = {1, 0};
    std::deque<int> deque = {1, 2};
    std::vector<int> list = {1, 2, 3};
    std::forward_list<int> forward_list = {1, 2, 3, 4};
    std::array<int, 5> array = {1, 2, 3, 4, 5};
    std::string string = "123456";
    std::vector<int> set = {1, 2, 3, 4, 5, 6};
    std::vector<int> multiset = {1, 2, 3, 4, 5, 6};
    std::map<int, int> map = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
    std::multimap<int, int> multimap = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
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
}
