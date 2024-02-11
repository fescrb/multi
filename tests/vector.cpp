//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <multi/select.hpp>
#include <multi/vector.hpp>
#include <rand.hpp>

TEST(vector, empty) {
  multi::vector<bool, int, double> empty_v;

  EXPECT_EQ(empty_v.size(), 0);
  EXPECT_EQ(empty_v.capacity(), 0);
  EXPECT_TRUE(empty_v.empty());
  EXPECT_FALSE(empty_v);
}

TEST(vector, reserve_empty) {
  constexpr std::size_t CAPACITY = 100;
  multi::vector<bool, int, double> empty_v;

  empty_v.reserve(CAPACITY);

  EXPECT_EQ(empty_v.size(), 0);
  EXPECT_GE(empty_v.capacity(), CAPACITY);
  EXPECT_TRUE(empty_v.empty());
  EXPECT_FALSE(empty_v);
}

TEST(vector, push_back_and_element_access) {
  constexpr std::size_t CAPACITY = 100;
  multi::vector<bool, int, double> v;
  const multi::vector<bool, int, double>& const_v = v;
  std::vector<std::tuple<bool, int, double>> std_v;

  for (int _ = 0; _ < CAPACITY; ++_) {
    auto val = rand_tuple<bool, int, double>();
    auto collected_val = std::make_tuple(std::get<0>(val), std::get<1>(val));
    v.push_back(val);
    std_v.push_back(val);
    EXPECT_EQ(v[v.size() - 1], val);
    EXPECT_EQ(v.back(), val);
    EXPECT_EQ(v.front(), std_v[0]);
    auto col_read = v.template collect<0, 1>(v.size() - 1);
    EXPECT_EQ(col_read, collected_val);
    EXPECT_EQ(const_v[v.size() - 1], val);
    EXPECT_EQ(const_v.back(), val);
    EXPECT_EQ(const_v.front(), std_v[0]);
    auto const_col_read = const_v.template collect<0, 1>(v.size() - 1);
    EXPECT_EQ(const_col_read, collected_val);
  }

  EXPECT_EQ(v.size(), CAPACITY);
  EXPECT_GE(v.capacity(), CAPACITY);
  EXPECT_FALSE(v.empty());
  EXPECT_TRUE(v);

  for (int i = 0; i < CAPACITY; i++) {
    EXPECT_EQ(v[i], std_v[i]);
    EXPECT_EQ(const_v[i], std_v[i]);
  }
}

TEST(vector, copy_construct) {
  constexpr std::size_t CAPACITY = 100;
  multi::vector<bool, int, double> v;

  for (int _ = 0; _ < CAPACITY; ++_) {
    auto val = rand_tuple<bool, int, double>();
    v.push_back(val);
  }

  ASSERT_EQ(v.size(), CAPACITY);
  ASSERT_GE(v.capacity(), CAPACITY);
  ASSERT_FALSE(v.empty());
  ASSERT_TRUE(v);

  multi::vector<bool, int, double> v_cp = v;

  for (int i = 0; i < CAPACITY; i++) {
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

  for (int _ = 0; _ < CAPACITY; ++_) {
    auto val = rand_tuple<bool, int, double>();
    v.push_back(val);
    std_v.push_back(val);
  }

  ASSERT_EQ(v.size(), CAPACITY);
  ASSERT_GE(v.capacity(), CAPACITY);
  ASSERT_FALSE(v.empty());
  ASSERT_TRUE(v);

  for (int i = 0; i < CAPACITY; i++) {
    auto val = rand_tuple<bool, int, double>();
    v[i] = val;
    std_v[i] = val;
  }

  for (int i = 0; i < CAPACITY; i++) {
    EXPECT_EQ(v[i], std_v[i]);
  }
}

TEST(vector, at) {
  constexpr std::size_t CAPACITY = 100;
  multi::vector<bool, int, double> v;
  std::vector<std::tuple<bool, int, double>> std_v;

  for (int _ = 0; _ < CAPACITY; ++_) {
    auto val = rand_tuple<bool, int, double>();
    v.push_back(val);
    std_v.push_back(val);
  }

  ASSERT_EQ(v.size(), CAPACITY);
  ASSERT_GE(v.capacity(), CAPACITY);
  ASSERT_FALSE(v.empty());
  ASSERT_TRUE(v);

  for (int i = 0; i < CAPACITY; i++) {
    auto val = rand_tuple<bool, int, double>();
    v.at(i) = val;
    std_v.at(i) = val;
  }

  for (int i = 0; i < CAPACITY; i++) {
    EXPECT_EQ(v[i], std_v[i]);
  }
}

TEST(vector, range_constraints) {
  constexpr std::size_t CAPACITY = 100;
  static_assert(std::ranges::range<multi::vector<int>>);
  static_assert(std::ranges::range<const multi::vector<int>>);
  static_assert(
      std::sentinel_for<decltype(std::declval<multi::vector<int>>().end()),
                        decltype(std::declval<multi::vector<int>>().begin())>);
  multi::vector<bool, int, double> v;
  const multi::vector<bool, int, double>& const_v = v;

  for (int _ = 0; _ < CAPACITY; ++_) {
    auto val = rand_tuple<bool, int, double>();
    v.push_back(val);
  }

  ASSERT_GE(v.size(), CAPACITY);
  ASSERT_GE(v.capacity(), CAPACITY);
  ASSERT_FALSE(v.empty());
  ASSERT_TRUE(v);

  EXPECT_EQ(std::ranges::begin(v), v.begin());
  EXPECT_EQ(std::ranges::end(v), v.end());
  EXPECT_EQ(std::ranges::begin(const_v), const_v.begin());
  EXPECT_EQ(std::ranges::end(const_v), const_v.end());
}

TEST(vector, iterator_equivalence) {
  constexpr std::size_t CAPACITY = 100;
  multi::vector<bool, int, double> v;
  const multi::vector<bool, int, double>& const_v = v;

  for (int _ = 0; _ < CAPACITY; ++_) {
    auto val = rand_tuple<bool, int, double>();
    v.push_back(val);
  }

  ASSERT_GE(v.size(), CAPACITY);
  ASSERT_GE(v.capacity(), CAPACITY);
  ASSERT_FALSE(v.empty());
  ASSERT_TRUE(v);

  auto it = v.begin();
  auto it_const = const_v.begin();
  EXPECT_EQ(it, it_const);
  EXPECT_GE(it, it_const);
  EXPECT_LE(it, it_const);
  EXPECT_GT(it + 1, it_const);
  EXPECT_GE(it + 1, it_const);
  EXPECT_NE(it + 1, it_const);
  EXPECT_LT(it - 1, it_const);
  EXPECT_LE(it - 1, it_const);
  EXPECT_NE(it - 1, it_const);

  static_assert(std::three_way_comparable_with<
                multi::vector<bool, int>::iterator<0, 1>,
                multi::vector<bool, int>::const_iterator<0, 1>>);
}