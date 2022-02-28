//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <multi/details/max_alignof.hpp>

#include <gtest/gtest.h>

TEST(max_alignof, static_assert) {
    static_assert(multi::max_alignof<char, double, int> == std::max({alignof(char), alignof(double), alignof(int)}));
}