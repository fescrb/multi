//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstddef>

namespace multi {

template<class... Ts>
constexpr std::size_t packed_sizeof = 0;

template<class T, class... Ts>
constexpr std::size_t packed_sizeof<T, Ts...> = sizeof(T) + packed_sizeof<Ts...>;

template<class T>
constexpr std::size_t packed_sizeof<T> = sizeof(T);


} // namespace multi