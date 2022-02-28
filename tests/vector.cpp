//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <multi/vector.hpp>

#include <gtest/gtest.h>

#include <limits>

template<class... Ts>
auto rand_tuple() {
    return std::make_tuple(static_cast<Ts>(rand() % static_cast<int>(std::numeric_limits<Ts>::max()))...);
}

TEST(vector, empty) {
    multi::vector<bool, int, double> empty_v;

    EXPECT_EQ(empty_v.size(), 0);
    EXPECT_EQ(empty_v.capacity(), 0);
    EXPECT_TRUE(empty_v.empty());
}

TEST(vector, reserve_empty) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> empty_v;

    empty_v.reserve(CAPACITY);

    EXPECT_EQ(empty_v.size(), 0);
    EXPECT_GE(empty_v.capacity(), CAPACITY);
    EXPECT_TRUE(empty_v.empty());
}

TEST(vector, push_back) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
        std_v.push_back(val);
        EXPECT_EQ(v[v.size()-1], val);
    }

    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(v[i], std_v[i]);
    }


    EXPECT_EQ(v.size(), CAPACITY);
    EXPECT_GE(v.capacity(), CAPACITY);
    EXPECT_TRUE(!v.empty());
}