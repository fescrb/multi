//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <functional>

#include <gtest/gtest.h>

#include <multi/reference_tuple.hpp>

using namespace multi;

TEST(reference_tuple, construct) {
    bool b = true;
    const int i = 1;
    double d = 1.0;

    auto rt = reference_tuple(b, i, d);

    bool same_rt =
        std::is_same_v<decltype(rt), reference_tuple<bool, const int, double>>;
    EXPECT_TRUE(same_rt);

    EXPECT_EQ(get<0>(rt), true);
    EXPECT_EQ(get<1>(rt), 1);
    EXPECT_EQ(get<2>(rt), 1.0);

    static_assert(!std::is_constructible_v<reference_tuple<bool, int, double>,
                                           bool, int, double>);
    static_assert(std::is_constructible_v<reference_tuple<bool, int, double>,
                                          bool&, int&, double&>);
    static_assert(!std::is_constructible_v<reference_tuple<bool, int, double>,
                                           bool&&, int&&, double&&>);
}

TEST(reference_tuple, assign) {
    bool b;
    int i;
    double d;

    reference_tuple<bool, int, double> rt(b, i, d);

    rt = std::make_tuple(false, 0, 0.0);

    EXPECT_EQ(b, false);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(d, 0.0);

    // Test that we can assign from tuple
    rt = std::make_tuple(1, 10u, 10.0f);

    EXPECT_EQ(b, true);
    EXPECT_EQ(i, 10);
    EXPECT_FLOAT_EQ(d, 10.0);

    // Test that we can assign from other reference_tuple
    bool b2 = false;
    int i2 = 30;
    double d2 = 30.0;

    reference_tuple<bool, int, double> rt2(b2, i2, d2);

    rt = rt2;

    EXPECT_EQ(b, false);
    EXPECT_EQ(i, 30);
    EXPECT_EQ(d, 30.0);

    // Test we can assign to const reference_tuple if refs are non-const
    const reference_tuple<bool, int, double> crt(b, i, d);

    crt = std::make_tuple(true, 1, 1.0);

    EXPECT_EQ(b, true);
    EXPECT_EQ(i, 1);
    EXPECT_EQ(d, 1.0);

    // Test we can assign to std::tuple

    b2 = true;
    i2 = 1;
    d2 = 1.0;

    std::tie(b, i, d) = rt2;

    EXPECT_EQ(b, true);
    EXPECT_EQ(i, 1);
    EXPECT_EQ(d, 1.0);
}

TEST(reference_tuple, tuple_comparison) {
    bool b = true;
    int i = 1;
    double d = 1.0;

    reference_tuple<bool, int, double> rt(b, i, d);

    EXPECT_EQ(rt, std::make_tuple(true, 1, 1.0));
    EXPECT_EQ(std::make_tuple(true, 1, 1.0), rt);

    EXPECT_NE(rt, std::make_tuple(false, 1, 1.0));
    EXPECT_NE(std::make_tuple(true, 1, 3.0), rt);

    EXPECT_GE(rt, std::make_tuple(false, 0, 1.0));
    EXPECT_GE(std::make_tuple(true, 1, 1.0), rt);

    EXPECT_LE(rt, std::make_tuple(true, 1, 2.0));
    EXPECT_LE(std::make_tuple(true, 1, 1.0), rt);

    EXPECT_GT(rt, std::make_tuple(false, 0, 1.0));
    EXPECT_GT(std::make_tuple(true, 1, 3.0), rt);

    EXPECT_LT(rt, std::make_tuple(true, 1, 2.0));
    EXPECT_LT(std::make_tuple(false, 1, 3.0), rt);
}

TEST(reference_tuple, reference_tuple_comparison) {
    bool b = true;
    int i = 1;
    double d = 1.0;

    reference_tuple<bool, int, double> rt(b, i, d);

    const bool cb = true;
    const int ci = 1;
    const double cd = 1.0;

    reference_tuple<const bool, const int, const double> crt(cb, ci, cd);

    EXPECT_EQ(rt, crt);
    EXPECT_EQ(crt, rt);

    b = false;

    EXPECT_NE(rt, crt);
    EXPECT_NE(crt, rt);

    EXPECT_LE(rt, crt);
    EXPECT_GE(crt, rt);

    EXPECT_LT(rt, crt);
    EXPECT_GT(crt, rt);
}