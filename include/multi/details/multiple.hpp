//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>

namespace multi::details {

template <std::size_t Base, std::size_t Number>
struct is_multiple_of {
    static constexpr bool value = Number % Base == 0;
};

template <std::size_t Base, std::size_t Number>
constexpr bool is_multiple_of_v = is_multiple_of<Base, Number>::value;

template <std::size_t Base, std::size_t Number>
struct next_multiple_of {
    static constexpr std::size_t value = Number + Base - Number % Base;
};

template <std::size_t Base, std::size_t Number>
constexpr std::size_t next_multiple_of_v =
    next_multiple_of<Base, Number>::value;

}  // namespace multi::details