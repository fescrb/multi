//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tuple>

#include <multi/reference_tuple.hpp>

namespace multi {

template <std::size_t I, class T>
struct tuple_element;

template <std::size_t I, class... Ts>
struct tuple_element<I, std::tuple<Ts...>>
    : std::tuple_element<I, std::tuple<Ts...>> {};

template <std::size_t I, class... Ts>
struct tuple_element<I, reference_tuple<Ts...>>
    : std::tuple_element<I, std::tuple<Ts&...>> {};

template <std::size_t I, class T>
using tuple_element_t = tuple_element<I, T>::type;

}  // namespace multi