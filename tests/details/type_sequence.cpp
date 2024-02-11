//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <multi/details/type_sequence.hpp>
#include <type_traits>

TEST(type_sequence, packed_sizeof) {
#pragma pack(push, t1, 1)
  struct packed_test {
    bool v0;
    int v1;
    double v2;
  };
#pragma pack(pop, t1)
  static_assert(multi::details::packed_sizeof<
                    multi::details::type_sequence<bool, int, double>> ==
                sizeof(packed_test));
  static_assert(
      multi::details::packed_sizeof<multi::details::type_sequence<>> == 0);
}

TEST(type_sequence, max_alignof) {
  static_assert(multi::details::max_alignof<
                    multi::details::type_sequence<bool, int, double>> ==
                std::max({alignof(char), alignof(int), alignof(double)}));
  static_assert(multi::details::max_alignof<multi::details::type_sequence<>> ==
                1);
}

TEST(type_sequence, size) {
  static_assert(multi::details::type_sequence<bool, int, double>::size() == 3);
  static_assert(multi::details::type_sequence<int, double>::size() == 2);
  static_assert(multi::details::type_sequence<double>::size() == 1);
  static_assert(multi::details::type_sequence<>::size() == 0);
}

TEST(type_sequence, sequence_element) {
  static_assert(
      std::is_same_v<multi::details::sequence_element_t<
                         0, multi::details::type_sequence<bool, int, double>>,
                     bool>);
  static_assert(
      std::is_same_v<multi::details::sequence_element_t<
                         1, multi::details::type_sequence<bool, int, double>>,
                     int>);
  static_assert(
      std::is_same_v<multi::details::sequence_element_t<
                         2, multi::details::type_sequence<bool, int, double>>,
                     double>);
}

TEST(type_sequence, subsequence) {
  static_assert(
      std::is_same_v<multi::details::subsequence_t<
                         std::index_sequence<0, 1>,
                         multi::details::type_sequence<bool, int, double>>,
                     multi::details::type_sequence<bool, int>>);
  static_assert(
      std::is_same_v<multi::details::subsequence_t<
                         std::index_sequence<1, 2>,
                         multi::details::type_sequence<bool, int, double>>,
                     multi::details::type_sequence<int, double>>);
  static_assert(
      std::is_same_v<multi::details::subsequence_t<
                         std::index_sequence<0, 2>,
                         multi::details::type_sequence<bool, int, double>>,
                     multi::details::type_sequence<bool, double>>);
  static_assert(
      std::is_same_v<multi::details::subsequence_t<
                         std::index_sequence<>,
                         multi::details::type_sequence<bool, int, double>>,
                     multi::details::type_sequence<>>);
}

TEST(type_sequence, take_subsequence) {
  static_assert(
      std::is_same_v<multi::details::take_subsequence_t<
                         2, multi::details::type_sequence<bool, int, double>>,
                     multi::details::type_sequence<bool, int>>);
  static_assert(
      std::is_same_v<multi::details::take_subsequence_t<
                         3, multi::details::type_sequence<bool, int, double>>,
                     multi::details::type_sequence<bool, int, double>>);
  static_assert(
      std::is_same_v<multi::details::take_subsequence_t<
                         0, multi::details::type_sequence<bool, int, double>>,
                     multi::details::type_sequence<>>);
}