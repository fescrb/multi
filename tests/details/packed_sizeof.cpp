//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <multi/details/packed_sizeof.hpp>

TEST(packed_sizeof, static_assert) {
#pragma pack(push, t1, 1)
  struct packed_test {
    bool v0;
    int v1;
    double v2;
  };
#pragma pack(pop, t1)
  static_assert(multi::details::packed_sizeof<bool, int, double> ==
                sizeof(packed_test));
  static_assert(multi::details::packed_sizeof<> == 0);
}