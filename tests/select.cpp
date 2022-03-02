//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <multi/select.hpp>

#include <gtest/gtest.h>

#include <rand.hpp>
#include <multi/vector.hpp>

#include <iostream>

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

    auto sel_view = multi::select_view<std::ranges::ref_view<multi::vector<bool, int, double>>,0, 1>(std::ranges::ref_view(v));
    auto begin_via_select = multi::select<0, 1>(v.begin());
    auto end_via_select = multi::select<0, 1>(v.end());

    EXPECT_EQ(sel_view.begin(), begin_via_select);
    EXPECT_EQ(sel_view.end(), end_via_select);
    EXPECT_EQ(sel_view.size(), v.size());
    EXPECT_EQ(sel_view.empty(), v.empty());
    EXPECT_EQ(sel_view.front(), *begin_via_select);
    EXPECT_EQ(sel_view.back(), *(end_via_select-1));
    
    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(sel_view[i], *(begin_via_select+i));
    }
}


TEST(select_view, const_vector_functionality) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v_mut;
    const multi::vector<bool, int, double>& v = v_mut;
    
    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v_mut.push_back(val);
    }

    ASSERT_EQ(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

    auto sel_view = multi::select_view<std::ranges::ref_view<const multi::vector<bool, int, double>>,0, 1>(v);
    auto begin_via_select = multi::select<0, 1>(v.begin());
    auto end_via_select = multi::select<0, 1>(v.end());

    EXPECT_EQ(sel_view.begin(), begin_via_select);
    EXPECT_EQ(sel_view.end(), end_via_select);
    EXPECT_EQ(sel_view.size(), v.size());
    EXPECT_EQ(sel_view.empty(), v.empty());
    EXPECT_EQ(sel_view.front(), *begin_via_select);
    EXPECT_EQ(sel_view.back(), *(end_via_select-1));
    
    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(sel_view[i], *(begin_via_select+i));
    }
}

TEST(select, vector_iterator) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
    }

    ASSERT_EQ(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

    auto begin_via_iterator = v.begin().template select<0, 1>();
    auto begin_via_select = multi::select<0, 1>(v.begin());
    
    auto end_via_iterator = v.end().template select<0, 1>();
    auto end_via_select = multi::select<0, 1>(v.end());

    EXPECT_EQ(begin_via_iterator, begin_via_select);
    EXPECT_EQ(end_via_iterator, end_via_select);

    auto it = begin_via_select;
    auto it_std = begin_via_iterator;
    
    while(it != end_via_select) {
        EXPECT_EQ(*it, *it_std);
        ++it;
        ++it_std;
    }
}

TEST(select, vector_range) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v;
    
    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
    }

    ASSERT_EQ(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

    auto v_r = multi::select<0, 1>(v);
    auto begin_via_range = v_r.begin();
    auto begin_via_select = multi::select<0, 1>(v.begin());
    
    auto end_via_range = v_r.end();
    auto end_via_select = multi::select<0, 1>(v.end());

    EXPECT_EQ(begin_via_range, begin_via_select);
    EXPECT_EQ(end_via_range, end_via_select);

    auto it = v_r.begin();
    auto it_std = begin_via_select;
    
    while(it != end_via_select) {
        EXPECT_EQ(*it, *it_std);
        ++it;
        ++it_std;
    }

    auto v_r2 = v | multi::select<0, 1>; 

    it = v_r2.begin();
    it_std = begin_via_select;
    
    while(it != v_r2.end()) {
        EXPECT_EQ(*it, *it_std);
        ++it;
        ++it_std;
    }
}