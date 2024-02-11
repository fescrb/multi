//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <multi/details/multiple.hpp>

using namespace multi::details;

TEST(is_multiple, static_assert) {
    static_assert(is_multiple_of_v<4, 8>);
    static_assert(is_multiple_of_v<4, 16>);
    static_assert(!is_multiple_of_v<4, 13>);
    static_assert(is_multiple_of_v<8, 16>);
    static_assert(!is_multiple_of_v<8, 15>);
}

TEST(next_multiple, static_assert) {
    static_assert(next_multiple_of_v<8, 8> == 16);
    static_assert(next_multiple_of_v<8, 9> == 16);
    static_assert(next_multiple_of_v<8, 15> == 16);
}