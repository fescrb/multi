//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <multi/vector.hpp>

#include <gtest/gtest.h>

#include <rand.hpp>
#include <multi/select.hpp>

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

TEST(vector, push_back_and_element_access) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v;
    const multi::vector<bool, int, double>& const_v = v;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        auto collected_val = std::make_tuple(std::get<0>(val), std::get<1>(val));
        v.push_back(val);
        std_v.push_back(val);
        EXPECT_EQ(v[v.size()-1], val);
        EXPECT_EQ(v.back(), val);
        EXPECT_EQ(v.front(), std_v[0]);
        auto col_read = v.template collect<0, 1>(v.size()-1);
        EXPECT_EQ(col_read, collected_val);
        EXPECT_EQ(const_v[v.size()-1], val);
        EXPECT_EQ(const_v.back(), val);
        EXPECT_EQ(const_v.front(), std_v[0]);
        auto const_col_read = const_v.template collect<0, 1>(v.size()-1);
        EXPECT_EQ(const_col_read, collected_val);
    }

    EXPECT_EQ(v.size(), CAPACITY);
    EXPECT_GE(v.capacity(), CAPACITY);
    EXPECT_TRUE(!v.empty());

    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(v[i], std_v[i]);
        EXPECT_EQ(const_v[i], std_v[i]);
    }
}

TEST(vector, copy_construct) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
    }

    ASSERT_EQ(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

    multi::vector<bool, int, double> v_cp = v;

    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(v[i], v_cp[i]);
    }

    EXPECT_NE(v.template data<0>(), v_cp.template data<0>());
    EXPECT_NE(v.template data<1>(), v_cp.template data<1>());
    EXPECT_NE(v.template data<2>(), v_cp.template data<2>());
}

TEST(vector, index_operator) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
        std_v.push_back(val);
    }

    ASSERT_EQ(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

    for(int i = 0; i < CAPACITY; i++) {
        auto val = rand_tuple<bool, int, double>();
        v[i] = val;
        std_v[i] = val;
    }

    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(v[i], std_v[i]);
    }
}

TEST(vector, at) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
        std_v.push_back(val);
    }

    ASSERT_EQ(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

    for(int i = 0; i < CAPACITY; i++) {
        auto val = rand_tuple<bool, int, double>();
        v.at(i) = val;
        std_v.at(i) = val;
    }
    
    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(v[i], std_v[i]);
    }
}

TEST(vector, range_constraints) {
    constexpr std::size_t CAPACITY = 100;
    static_assert(std::ranges::range<multi::vector<int>>);
    multi::vector<bool, int, double> v;
    const multi::vector<bool, int, double>& const_v = v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

    EXPECT_EQ(std::ranges::begin(v), v.begin());
    EXPECT_EQ(std::ranges::end(v), v.end());
    EXPECT_EQ(std::ranges::begin(const_v), const_v.begin());
    EXPECT_EQ(std::ranges::end(const_v), const_v.end());
}

TEST(vector_iterator, increment) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
        std_v.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

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

TEST(vector_iterator, add) {
    constexpr std::size_t CAPACITY = 100;
    constexpr std::ptrdiff_t step = 2;
    multi::vector<bool, int, double> v;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
        std_v.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

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

TEST(vector_iterator, index_operator) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
        std_v.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

    std::ptrdiff_t start = rand() % (CAPACITY / 2);

    auto it = v.begin() + start;
    auto std_it = std_v.begin() + start;

    for(int i = 0; i < (CAPACITY / 2); i++) {
        EXPECT_EQ(it[i], std_it[i]);
    }
}

TEST(vector_iterator, comparisons) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

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

TEST(vector_iterator, difference) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

    std::ptrdiff_t start = rand() % (CAPACITY / 2);

    auto it = v.begin() + start;

    for(int i = 0; i < (CAPACITY / 2); i++) {
        std::ptrdiff_t offset = rand() % (CAPACITY / 2);
        EXPECT_EQ((it + offset) - it, offset);
    }
}

TEST(vector_iterator, decrement) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
        std_v.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

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

TEST(vector_iterator, subtract) {
    constexpr std::size_t CAPACITY = 100;
    constexpr std::ptrdiff_t step = 2;
    multi::vector<bool, int, double> v;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
        std_v.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

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

TEST(vector_iterator, select) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
        std_v.push_back(val);
    }

    ASSERT_GE(v.size(), CAPACITY);
    ASSERT_GE(v.capacity(), CAPACITY);
    ASSERT_TRUE(!v.empty());

    auto it = v.begin().template select<0, 1>();
    auto std_it = std_v.begin();

    while(it != v.end().template select<0, 1>()) {
        auto std_tup = std::make_tuple(std::get<0>(*std_it), std::get<1>(*std_it));
        EXPECT_EQ(*it, std_tup);
        ++it;
        ++std_it;
    }
}

TEST(vector_iterator, constraint_passing) {
    static_assert(std::random_access_iterator<multi::vector<int>::iterator<0>>);
    static_assert(multi::selectable<multi::vector<int>::iterator<0>, 0>);
    static_assert(multi::selectable<multi::vector<bool, int>::iterator<0, 1>, 0>);
}
