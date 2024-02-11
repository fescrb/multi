//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include "rand.hpp"
#include <multi/select.hpp>
#include <multi/vector.hpp>

TEST(select, multi_vector) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v;

    for (int _ = 0; _ < CAPACITY; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
    }

    ASSERT_EQ(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

    auto sel_view = v | multi::select<0, 1>;

    EXPECT_EQ(sel_view.size(), v.size());
    EXPECT_EQ(sel_view.empty(), v.empty());

    for (int i = 0; i < CAPACITY; i++) {
        auto val = multi::select<0, 1>(v[i]);
        EXPECT_EQ(sel_view[i], val);
    }
}

TEST(select, tuple_vector) {
    constexpr std::size_t CAPACITY = 100;
    std::vector<std::tuple<bool, int, double>> v;

    for (int _ = 0; _ < CAPACITY; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
    }

    auto sel_view = v | multi::select<0, 1>;

    EXPECT_EQ(sel_view.size(), v.size());
    EXPECT_EQ(sel_view.empty(), v.empty());

    for (int i = 0; i < CAPACITY; i++) {
        auto val = multi::select<0, 1>(v[i]);
        EXPECT_EQ(sel_view[i], val);
    }
}

TEST(select, multi_tuple_vector_equivalence) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> mv;
    std::vector<std::tuple<bool, int, double>> tv;

    for (int _ = 0; _ < CAPACITY; ++_) {
        auto val = rand_tuple<bool, int, double>();
        mv.push_back(val);
        tv.push_back(val);
    }

    auto mv_sel_view = mv | multi::select<0, 1>;
    auto tv_sel_view = tv | multi::select<0, 1>;

    EXPECT_EQ(mv_sel_view.size(), tv_sel_view.size());
    EXPECT_EQ(mv_sel_view.empty(), tv_sel_view.empty());

    for (int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(mv_sel_view[i], tv_sel_view[i]);
    }
}
