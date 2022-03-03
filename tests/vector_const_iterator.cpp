//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <multi/vector.hpp>

#include <gtest/gtest.h>

#include <rand.hpp>
#include <multi/select.hpp>


TEST(vector_const_iterator, increment) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v_mut;
    const multi::vector<bool, int, double>& v = v_mut;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v_mut.push_back(val);
        std_v.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_FALSE(v.empty());
    ASSERT_TRUE(v);

    auto it = v.begin();
    auto std_it = std_v.begin();

    while(it != v.end()) {
        EXPECT_EQ(*it, *std_it);
        ++it;
        ++std_it;
    }

    it = v.begin();
    std_it = std_v.begin();

    while(it != v.end()) {
        EXPECT_EQ(*it, *std_it);
        it ++;
        std_it++;
    }
    
    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(v[i], std_v[i]);
    }
}

TEST(vector_const_iterator, add) {
    constexpr std::size_t CAPACITY = 100;
    constexpr std::ptrdiff_t step = 2;
    multi::vector<bool, int, double> v_mut;
    const multi::vector<bool, int, double>& v = v_mut;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v_mut.push_back(val);
        std_v.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_FALSE(v.empty());
    ASSERT_TRUE(v);

    auto it = v.begin();
    auto std_it = std_v.begin();

    while(it < v.end()) {
        EXPECT_EQ(*it, *std_it);
        it+=step;
        std_it+=step;
    }
    
    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(v[i], std_v[i]);
    }

    it = v.begin();
    std_it = std_v.begin();

    while(it < v.end()) {
        EXPECT_EQ(*it, *std_it);
        it = it + step;
        std_it= std_it + step;
    }

    it = v.begin();
    std_it = std_v.begin();

    while(it < v.end()) {
        EXPECT_EQ(*it, *std_it);
        it = step + it;
        std_it = step + std_it;
    }
    
    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(v[i], std_v[i]);
    }
}

TEST(vector_const_iterator, index_operator) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v_mut;
    const multi::vector<bool, int, double>& v = v_mut;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v_mut.push_back(val);
        std_v.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_FALSE(v.empty());
    ASSERT_TRUE(v);

    std::ptrdiff_t start = rand() % (CAPACITY / 2);

    auto it = v.begin() + start;
    auto std_it = std_v.begin() + start;

    for(int i = 0; i < (CAPACITY / 2); i++) {
        EXPECT_EQ(it[i], std_it[i]);
    }
}

TEST(vector_const_iterator, comparisons) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v_mut;
    const multi::vector<bool, int, double>& v = v_mut;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v_mut.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_FALSE(v.empty());
    ASSERT_TRUE(v);

    EXPECT_LT(v.begin(), v.end());
    EXPECT_NE(v.begin(), v.end());

    EXPECT_LT(v.begin(), v.begin()+1);
    EXPECT_NE(v.begin(), v.begin()+1);

    EXPECT_GT(v.begin(), v.begin()-1);
    EXPECT_NE(v.begin(), v.begin()-1);

    EXPECT_EQ(v.begin(), v.begin());
    EXPECT_EQ(v.begin(), v.begin()+0);
    EXPECT_EQ(v.begin(), v.begin()+1-1);
}

TEST(vector_const_iterator, difference) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v_mut;
    const multi::vector<bool, int, double>& v = v_mut;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v_mut.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_FALSE(v.empty());
    ASSERT_TRUE(v);

    std::ptrdiff_t start = rand() % (CAPACITY / 2);

    auto it = v.begin() + start;

    for(int i = 0; i < (CAPACITY / 2); i++) {
        std::ptrdiff_t offset = rand() % (CAPACITY / 2);
        EXPECT_EQ((it + offset) - it, offset);
    }
}

TEST(vector_const_iterator, decrement) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v_mut;
    const multi::vector<bool, int, double>& v = v_mut;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v_mut.push_back(val);
        std_v.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_FALSE(v.empty());
    ASSERT_TRUE(v);

    auto it = v.end() - 1;
    auto std_it = std_v.end() -1;

    while(it >= v.begin()) {
        EXPECT_EQ(*it, *std_it);
        --it;
        --std_it;
    }

    it = v.end() - 1;
    std_it = std_v.end() - 1;

    while(it >= v.begin()) {
        EXPECT_EQ(*it, *std_it);
        it--;
        std_it--;
    }
    
    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(v[i], std_v[i]);
    }
}

TEST(vector_const_iterator, subtract) {
    constexpr std::size_t CAPACITY = 100;
    constexpr std::ptrdiff_t step = 2;
    multi::vector<bool, int, double> v_mut;
    const multi::vector<bool, int, double>& v = v_mut;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v_mut.push_back(val);
        std_v.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_FALSE(v.empty());
    ASSERT_TRUE(v);

    auto it = v.end() - 1;
    auto std_it = std_v.end() -1;

    while(it >= v.begin()) {
        EXPECT_EQ(*it, *std_it);
        it-=step;
        std_it-=step;
    }
    
    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(v[i], std_v[i]);
    }

    it = v.end() - 1;
    std_it = std_v.end() - 1;

    while(it >= v.begin()) {
        EXPECT_EQ(*it, *std_it);
        it = it - step;
        std_it= std_it - step;
    }
    
    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(v[i], std_v[i]);
    }
}

TEST(vector_const_iterator, select) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v_mut;
    const multi::vector<bool, int, double>& v = v_mut;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v_mut.push_back(val);
        std_v.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_FALSE(v.empty());
    ASSERT_TRUE(v);

    auto it = v.begin().template select<0, 1>();
    auto std_it = std_v.begin();

    while(it != v.end().template select<0, 1>()) {
        auto std_tup = std::make_tuple(std::get<0>(*std_it), std::get<1>(*std_it));
        EXPECT_EQ(*it, std_tup);
        ++it;
        ++std_it;
    }
}


TEST(vector_const_iterator, constraint_passing) {
    static_assert(std::random_access_iterator<multi::vector<int>::const_iterator<0>>);
    static_assert(multi::selectable<multi::vector<int>::const_iterator<0>, 0>);
    static_assert(multi::selectable<multi::vector<bool, int>::const_iterator<0, 1>, 0>);
    static_assert(std::three_way_comparable<multi::vector<bool, int>::const_iterator<0, 1>>);
    static_assert(std::three_way_comparable_with<
        multi::vector<bool, int>::const_iterator<0, 1>, 
        multi::vector<bool, int>::const_iterator<0>>);
}

TEST(vector_const_iterator, select_comparison) {
    multi::vector<bool, int, double> v_mut;
    const multi::vector<bool, int, double>& v = v_mut;
    auto it = v.begin();
    auto it_select = it.template select<0, 1>();
    EXPECT_EQ(it, it_select);
    EXPECT_GE(it, it_select);
    EXPECT_LE(it, it_select);
    EXPECT_GT(it+1, it_select);
    EXPECT_GE(it+1, it_select);
    EXPECT_NE(it+1, it_select);
    EXPECT_LT(it-1, it_select);
    EXPECT_LE(it-1, it_select);
    EXPECT_NE(it-1, it_select);
}