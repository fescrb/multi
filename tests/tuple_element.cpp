//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <multi/tuple_element.hpp>

using namespace multi;

TEST(tuple_element, basic) {
    using crt = reference_tuple<const bool, const int, const double>;

    static_assert(std::is_same_v<const bool&, tuple_element_t<0, crt>>);
    static_assert(std::is_same_v<const int&, tuple_element_t<1, crt>>);
    static_assert(std::is_same_v<const double&, tuple_element_t<2, crt>>);

    using rt = reference_tuple<bool, int, double>;

    static_assert(std::is_same_v<bool&, tuple_element_t<0, rt>>);
    static_assert(std::is_same_v<int&, tuple_element_t<1, rt>>);
    static_assert(std::is_same_v<double&, tuple_element_t<2, rt>>);

    using t = std::tuple<bool, int, double>;

    static_assert(std::is_same_v<bool, tuple_element_t<0, t>>);
    static_assert(std::is_same_v<int, tuple_element_t<1, t>>);
    static_assert(std::is_same_v<double, tuple_element_t<2, t>>);
}