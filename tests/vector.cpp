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

TEST(vector, push_back_and_element_access) {
    constexpr std::size_t CAPACITY = 100;
    multi::vector<bool, int, double> v;
    const multi::vector<bool, int, double>& const_v = v;
    std::vector<std::tuple<bool, int, double>> std_v;

    for(int _ = 0; _ < CAPACITY ; ++_) {
        auto val = rand_tuple<bool, int, double>();
        v.push_back(val);
        std_v.push_back(val);
        EXPECT_EQ(v[v.size()-1], val);
        EXPECT_EQ(v.back(), val);
        EXPECT_EQ(v.front(), std_v[0]);
        EXPECT_EQ(const_v[v.size()-1], val);
        EXPECT_EQ(const_v.back(), val);
        EXPECT_EQ(const_v.front(), std_v[0]);
    }

    EXPECT_EQ(v.size(), CAPACITY);
    EXPECT_GE(v.capacity(), CAPACITY);
    EXPECT_TRUE(!v.empty());

    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(v[i], std_v[i]);
        EXPECT_EQ(const_v[i], std_v[i]);
    }
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
        it+=2;
        std_it+=2;
    }
    
    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(v[i], std_v[i]);
    }

    it = v.begin();
    std_it = std_v.begin();

    while(it < v.end()) {
        EXPECT_EQ(*it, *std_it);
        it = it + 2;
        std_it= std_it + 2;
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
        it-=2;
        std_it-=2;
    }
    
    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(v[i], std_v[i]);
    }

    it = v.end() - 1;
    std_it = std_v.end() - 1;

    while(it >= v.begin()) {
        EXPECT_EQ(*it, *std_it);
        it = it - 2;
        std_it= std_it - 2;
    }
    
    for(int i = 0; i < CAPACITY; i++) {
        EXPECT_EQ(v[i], std_v[i]);
    }
}