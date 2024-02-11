//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>
#include <utility>
#include <tuple>
#include <array>



namespace multi::details {

template<typename T>
struct tuple_like : std::false_type {};

template<typename... Ts>
struct tuple_like<std::tuple<Ts...>> : std::true_type {};

template<typename T, typename U>
struct tuple_like<std::pair<T, U>> : std::true_type {};

template<typename T, size_t S>
struct tuple_like<std::array<T, S>> : std::true_type {};


} // namespace multi::details