//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include "print.hpp"
#include <multi/array.hpp>

using namespace multi;

using test_array_t = array<5, bool, int, double>;

TEST(array, properties) {
    static_assert(std::input_or_output_iterator<test_array_t::iterator>);
    static_assert(std::input_iterator<test_array_t::const_iterator>);
    static_assert(
        std::sentinel_for<test_array_t::iterator, test_array_t::iterator>);
    static_assert(std::ranges::range<test_array_t>);
}

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

    EXPECT_EQ(array[0], std::make_tuple(false, 0, 0.0));
    EXPECT_EQ(array[1], std::make_tuple(false, 1, 1.0));
    EXPECT_EQ(array[2], std::make_tuple(false, 2, 2.0));
    EXPECT_EQ(array[3], std::make_tuple(false, 3, 3.0));
    EXPECT_EQ(array[4], std::make_tuple(false, 4, 4.0));
}

TEST(array, copy_initialization) {
    array<5, bool, int, double> array = {{
        {false, 0, 0.0},
        {false, 1, 1.0},
        {false, 2, 2.0},
        {false, 3, 3.0},
        {false, 4, 4.0},
    }};

    auto copy = array;

    *copy.begin() = *array.begin();

    EXPECT_EQ(array, copy);
}

TEST(array, range_initialization) {
    std::array<std::tuple<bool, int, double>, 5> std_array = {{
        {false, 0, 0.0},
        {false, 1, 1.0},
        {false, 2, 2.0},
        {false, 3, 3.0},
        {false, 4, 4.0},
    }};

    auto copy = array<5, bool, int, double>(std_array);

    EXPECT_TRUE(std::ranges::equal(std_array, copy));
}

TEST(array, iterator_initialization) {
    std::array<std::tuple<bool, int, double>, 5> std_array = {{
        {false, 0, 0.0},
        {false, 1, 1.0},
        {false, 2, 2.0},
        {false, 3, 3.0},
        {false, 4, 4.0},
    }};

    auto copy = array<5, bool, int, double>(std_array.begin(), std_array.end());

    EXPECT_TRUE(std::ranges::equal(std_array, copy));
}

TEST(array, front_back) {
    array<5, bool, int, double> array = {{
        {false, 0, 0.0},
        {false, 1, 1.0},
        {false, 2, 2.0},
        {false, 3, 3.0},
        {false, 4, 4.0},
    }};

    EXPECT_EQ(array.front(), std::make_tuple(false, 0, 0.0));
    EXPECT_EQ(array.back(), std::make_tuple(false, 4, 4.0));
}

TEST(array, array_access) {
    std::array<std::tuple<bool, int, double>, 5> std_array = {{
        {false, 0, 0.0},
        {false, 1, 1.0},
        {false, 2, 2.0},
        {false, 3, 3.0},
        {false, 4, 4.0},
    }};

    auto copy = array<5, bool, int, double>(std_array);

    for (size_t i = 0; i < copy.size(); i++) {
        EXPECT_EQ(std_array[i], copy[i]);
        EXPECT_EQ(std_array.at(i), copy.at(i));
    }
}

TEST(array, get) {
    std::array<std::tuple<bool, int, double>, 5> std_array = {{
        {false, 0, 0.0},
        {false, 1, 1.0},
        {false, 2, 2.0},
        {false, 3, 3.0},
        {false, 4, 4.0},
    }};

    auto copy = array<5, bool, int, double>(std_array);

    for (size_t i = 0; i < copy.size(); i++) {
        EXPECT_EQ(std::get<0>(std_array[i]), copy.get<0>(i));
        EXPECT_EQ(std::get<1>(std_array[i]), copy.get<1>(i));
        EXPECT_EQ(std::get<2>(std_array[i]), copy.get<2>(i));
    }
}

TEST(array, assign) {
    array<5, bool, int, double> original = {{
        {false, 0, 0.0},
        {false, 1, 1.0},
        {false, 2, 2.0},
        {false, 3, 3.0},
        {false, 4, 4.0},
    }};

    auto copy = array<5, bool, int, double>();

    copy = original;

    EXPECT_TRUE(std::ranges::equal(original, copy));

    std::array<std::tuple<bool, int, double>, 5> std_array = {{
        {true, 0, 00.0},
        {true, 10, 10.0},
        {true, 20, 20.0},
        {true, 30, 30.0},
        {true, 40, 40.0},
    }};

    copy = std_array;

    EXPECT_TRUE(std::ranges::equal(std_array, copy));
}