//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <array>
#include <tuple>
#include <type_traits>
#include <utility>

#include <multi/reference_tuple.hpp>

namespace multi::details {

template <typename T>
struct is_tuple_like : std::false_type {};

template <typename... Ts>
struct is_tuple_like<std::tuple<Ts...>> : std::true_type {};

template <typename T, typename U>
struct is_tuple_like<std::pair<T, U>> : std::true_type {};

template <typename... Ts>
struct is_tuple_like<multi::reference_tuple<Ts...>> : std::true_type {};

template <typename T, size_t S>
struct is_tuple_like<std::array<T, S>> : std::true_type {};

template <typename T>
constexpr bool is_tuple_like_v = is_tuple_like<T>::value;

template <typename T>
concept tuple_like = is_tuple_like_v<T>;

template <typename T>
concept tuple_like_element = tuple_like<
    typename std::iterator_traits<std::ranges::iterator_t<T>>::value_type>;

}  // namespace multi::details