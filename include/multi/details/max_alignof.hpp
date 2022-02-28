//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstddef>
#include <algorithm>

namespace multi {

template<class... Ts>
constexpr std::size_t max_alignof;

template<class T, class... Ts>
constexpr std::size_t max_alignof<T, Ts...> = std::max(alignof(T), max_alignof<Ts...>);

template<class T>
constexpr std::size_t max_alignof<T> = alignof(T);

} // namespace multi