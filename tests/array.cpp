//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <multi/array.hpp>

using namespace multi;

TEST(array, default_initialization) {
    array<5, bool, int, double> array;

    EXPECT_EQ(array.size(), 5);
}

TEST(array, array_initialization) {
    array<5, bool, int, double> array = {{
        {false, 0, 0.0},
        {false, 1, 1.0},
        {false, 2, 2.0},
        {false, 3, 3.0},
        {false, 4, 4.0},
    }};

    EXPECT_EQ(array.size(), 5);
}

TEST(array, copy_initialization) {
    array<5, bool, int, double> array = {{
        {false, 0, 0.0},
        {false, 1, 1.0},
        {false, 2, 2.0},
        {false, 3, 3.0},
        {false, 4, 4.0},
    }};

    auto b = array.cbegin();
    auto e = array.cend();

    // auto copy = array;

    // EXPECT_EQ(array, copy);
}