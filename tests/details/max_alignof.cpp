//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <multi/details/max_alignof.hpp>

TEST(max_alignof, static_assert) {
  static_assert(multi::details::max_alignof<char, int, double> ==
                std::max({alignof(char), alignof(int), alignof(double)}));
  static_assert(multi::details::max_alignof<char, int> ==
                std::max({alignof(char), alignof(int)}));
  static_assert(multi::details::max_alignof<> == 1);
}