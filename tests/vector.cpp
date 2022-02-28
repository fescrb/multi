//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <multi/vector.hpp>

#include <gtest/gtest.h>

TEST(vector, empty) {
    multi::vector<bool, int, double> empty_v;

    EXPECT_EQ(empty_v.size(), 0);
    EXPECT_TRUE(empty_v.empty());
}