//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <multi/select.hpp>

#include <gtest/gtest.h>

#include <rand.hpp>
#include <multi/vector.hpp>


TEST(select_view, vector_functionality) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v;
    
    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
    }

    ASSERT_EQ(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

    auto sel_view = v | multi::select<0, 1>;

    EXPECT_EQ(sel_view.size(), v.size());
    EXPECT_EQ(sel_view.empty(), v.empty());
    
    for(int i = 0; i < CAPACITY; i++) {
        auto val = multi::select<0,1>(v[i]);
        EXPECT_EQ(sel_view[i], val);
    }
}
