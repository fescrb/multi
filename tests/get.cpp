//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <multi/get.hpp>

#include <gtest/gtest.h>

#include <rand.hpp>

#include <ranges>

TEST(get, std_getable) {
    static_assert(multi::std_getable<std::tuple<int,int,int>,0>);
    static_assert(multi::std_getable<std::tuple<int,int,int>,2>);
    static_assert(multi::std_getable<std::pair<int,int>,0>);
    static_assert(multi::std_getable<std::pair<int,int>,1>);
    static_assert(multi::std_getable<std::array<int,4>,0>);
    static_assert(multi::std_getable<std::array<int,4>,3>);

    constexpr std::size_t NUM_TESTS = 100;

    for(auto _ : std::views::iota(0u, NUM_TESTS)) {
        auto tup = rand_tuple<int, double>();
        EXPECT_EQ(multi::get<0>(tup), std::get<0>(tup));
        EXPECT_EQ(multi::get<1>(tup), std::get<1>(tup));
    }
}