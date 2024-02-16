//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <multi/reference_tuple.hpp>

using namespace multi;

TEST(reference_tuple, assign) {
    bool b;
    int i;
    double d;

    reference_tuple<bool, int, double> rt(b, i, d);

    rt = std::make_tuple(false, 0, 0.0);

    EXPECT_EQ(get<0>(rt), false);
    EXPECT_EQ(get<1>(rt), 0);
    EXPECT_EQ(get<2>(rt), 0.0);

    const reference_tuple<bool, int, double> crt(b, i, d);

    crt = std::make_tuple(true, 1, 1.0);

    EXPECT_EQ(get<0>(crt), true);
    EXPECT_EQ(get<1>(crt), 1);
    EXPECT_EQ(get<2>(crt), 1.0);
}