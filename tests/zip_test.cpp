#include "msd/zip.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <array>
#include <deque>
#include <forward_list>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "data.hpp"

TEST(ZipTest, Zip)
{
    const std::array<const int, 3> odd{1, 3, 5};
    const std::array<float, 3> even{2.0F, 4.0F, 6.0F};
    std::vector<int> vec{9, 10, 11, 12};
    std::vector<data> items;
    items.reserve(6);
    items.emplace_back(1);
    items.emplace_back(2);
    items.emplace_back(3);
    items.emplace_back(4);
    items.emplace_back(5);
    items.emplace_back(6);

    const std::array<std::array<int, 4>, 3> expected{
        {
            {1, 2, 9, 1},
            {3, 4, 10, 2},
            {5, 6, 11, 3},
        },
    };

    std::size_t index = 0;
    for (auto [a, b, c, d] : msd::zip(odd, even, vec, items)) {
        static_assert(std::is_same_v<decltype(a), const int&>);
        static_assert(std::is_same_v<decltype(b), const float&>);
        static_assert(std::is_same_v<decltype(c), int&>);
        static_assert(std::is_same_v<decltype(d), data&>);

        EXPECT_THAT(expected[index], ::testing::ElementsAre(a, b, c, d.num));
        ++index;

        c += d.num;
    }

    EXPECT_THAT(vec, ::testing::ElementsAre(10, 12, 14, 12));

    EXPECT_EQ(data::constructs_, 6);
    EXPECT_EQ(data::copies_, 0);
    EXPECT_EQ(data::moves_, 0);
}

TEST(ZipTest, Begin)
{
    const std::array<int, 5> a{1, 2, 3};
    std::vector<int> b{4, 5};
    const std::vector<int> c{6, 7, 8, 9};

    msd::zip zip(a, b, c);

    auto [a0, b0, c0] = *zip.begin();
    EXPECT_EQ(a0, 1);
    EXPECT_EQ(b0, 4);
    EXPECT_EQ(c0, 6);

    EXPECT_EQ(std::prev(zip.end()), std::next(zip.begin()));
    EXPECT_EQ(zip.cbegin(), std::prev(zip.cend(), 2));
}

TEST(ZipTest, End) {}

TEST(ZipTest, Size)
{
    const std::array<int, 5> a{1, 2, 3};
    std::vector<int> b{4, 5};
    const std::vector<int> c{6, 7, 8, 9};

    EXPECT_EQ(msd::zip(a, b, c).size(), 2);
}

TEST(ZipTest, Empty)
{
    const std::array<int, 5> non_empty{1, 2, 3};
    const std::vector<int> empty{};
    const msd::zip empty_zip(non_empty, empty);
    for (auto _ : empty_zip) {
        std::ignore = _;
        FAIL();
    }
    EXPECT_TRUE(empty_zip.empty());

    const msd::zip non_empty_zip(non_empty, non_empty);
    EXPECT_FALSE(non_empty_zip.empty());
}

TEST(ZipTest, CBegin) {}

TEST(ZipTest, CEnd) {}

TEST(ZipTest, OperatorBool)
{
    const std::array<int, 5> non_empty{1, 2, 3};
    const std::vector<int> empty{};
    const msd::zip empty_zip(non_empty, empty);
    for (auto _ : empty_zip) {
        std::ignore = _;
        FAIL();
    }
    EXPECT_FALSE(empty_zip);

    const msd::zip non_empty_zip(non_empty, non_empty);
    EXPECT_TRUE(non_empty_zip);
}

TEST(ZipTest, Front)
{
    std::array<int, 3> one{1, 3, 5};
    const std::array<int, 4> two{2, 4, 6, 8};

    msd::zip zip(one, two);
    auto [a, b] = zip.front();
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 2);
    a = 9;
    EXPECT_EQ(one[0], 9);

    const msd::zip const_zip(one, two);
    auto [const_a, const_b] = const_zip.front();
    EXPECT_EQ(const_a, 9);
    EXPECT_EQ(const_b, 2);

    std::vector<int> empty_vec;
    msd::zip empty_zip(empty_vec, empty_vec);
    EXPECT_DEBUG_DEATH(empty_zip.front(), "");
}

TEST(ZipTest, Back)
{
    std::array<int, 3> one{1, 3, 5};
    const std::array<int, 4> two{2, 4, 6, 8};

    msd::zip zip(one, two);
    auto [a, b] = zip.back();
    EXPECT_EQ(a, 5);
    EXPECT_EQ(b, 6);
    a = 9;
    EXPECT_EQ(one[2], 9);

    const msd::zip const_zip(one, two);
    auto [const_a, const_b] = const_zip.back();
    EXPECT_EQ(const_a, 9);
    EXPECT_EQ(const_b, 6);
}

TEST(ZipTest, OperatorSubscript)
{
    std::array<int, 3> one{1, 3, 5};
    const std::array<int, 4> two{2, 4, 6, 8};

    const std::array<std::array<int, 2>, 3> expected{
        {
            {1, 2},
            {3, 4},
            {5, 6},
        },
    };

    msd::zip zip(one, two);
    for (std::size_t i = 0; i < zip.size(); ++i) {
        auto [a, b] = zip[i];
        EXPECT_EQ(a, expected[i][0]);
        EXPECT_EQ(b, expected[i][1]);

        EXPECT_DEBUG_DEATH(zip[99], "");
    }

    const msd::zip const_zip(one, two);
    for (std::size_t i = 0; i < const_zip.size(); ++i) {
        auto [a, b] = zip[i];
        EXPECT_EQ(a, expected[i][0]);
        EXPECT_EQ(b, expected[i][1]);

        EXPECT_DEBUG_DEATH(zip[99], "");
    }
}

TEST(ZipTest, ContainersAndAlgorithms)
{
    std::vector<int> vector = {1, 0};
    std::deque<int> deque = {1, 2};
    std::list<int> list = {1, 2, 3};
    std::forward_list<int> forward_list = {1, 2, 3, 4};
    std::array<int, 5> array = {1, 2, 3, 4, 5};
    std::string string = "123456";
    std::set<int> set = {1, 2, 3, 4, 5, 6};
    std::multiset<int> multiset = {1, 2, 3, 4, 5, 6};
    std::map<int, int> map = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
    std::multimap<int, int> multimap = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
    std::unordered_set<int> unordered_set = {1, 2, 3, 4, 5, 6};
    std::unordered_multiset<int> unordered_multiset = {1, 2, 3, 4, 5, 6};
    std::unordered_map<int, int> unordered_map = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
    std::unordered_multimap<int, int> unordered_multimap = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};

    auto zip = msd::zip(vector, deque, list, forward_list, array, string, set, multiset, map, multimap, unordered_set,
                        unordered_multiset, unordered_map, unordered_multimap);

    const int sum = std::accumulate(zip.begin(), zip.end(), 0, [](int acc, auto&& tuple) {
        auto [vec, deq, li, fwd, arr, str, s, mset, mp, mm, uset, umset, umap, umm] = tuple;

        return acc + vec + deq + li + fwd + arr + str + s + mset + mp.second + mm.second;
    });
    EXPECT_EQ(sum, 124);

    const msd::zip const_zip = zip;
    const auto it = std::find_if(const_zip.begin(), const_zip.end(), [](auto tuple) {
        auto [vec, deq, li, fwd, arr, str, s, mset, mp, mm, uset, umset, umap, umm] = tuple;
        return mm.second == 2;
    });
    EXPECT_EQ(it, std::next(const_zip.begin()));

    const msd::zip moved_zip = std::move(zip);
    const auto it_moved_zip = std::find_if(moved_zip.cbegin(), moved_zip.cend(), [](auto tuple) {
        auto [vec, deq, li, fwd, arr, str, s, mset, mp, mm, uset, umset, umap, umm] = tuple;
        return umap.second == 22;
    });
    EXPECT_EQ(it_moved_zip, moved_zip.cend());
}

TEST(ZipTest, API)
{
    std::array<int, 3> actual{1, 3, 5};
    std::array<int, 3> expected{1, 3, 5};
    msd::zip zip(actual, expected);

    EXPECT_EQ(zip.size(), 3);
    for (auto [a, e] : zip) {
        EXPECT_EQ(a, e);
    }

    const std::vector<int> input{1, 2, 3};
    std::vector<int> output(3);
    for (auto [in, out] : msd::zip(input, output)) {
        out = in + 1;
    }

    EXPECT_THAT(output, ::testing::ElementsAre(2, 3, 4));

    const std::array<int, 5> input_arr{1, 2, 3};
    std::vector<int> output_vec;
    for (std::tuple<const int&, int>&& tuple : msd::zip(input_arr, output_vec)) {
        output_vec.push_back(std::get<0>(tuple));
    }

    EXPECT_TRUE(output_vec.empty());

    std::array<int, 5> three{1, 2, 3};
    std::vector<int> one{5};
    for (auto [t, o] : msd::zip(three, one)) {
        o = t;
    }
    EXPECT_EQ(one.size(), 1);
    EXPECT_THAT(one, ::testing::ElementsAre(1));

    std::array<int, 5> arr_three{1, 2, 3};
    std::array<int, 2> arr_one{1, 2};
    std::size_t iterations = 0;
    for (auto [t, o] : msd::zip(arr_three, arr_one)) {
        std::ignore = t;
        std::ignore = o;
        ++iterations;
    }
    EXPECT_EQ(iterations, 2);

    EXPECT_FALSE(msd::zip(arr_three, arr_one).empty());
}
