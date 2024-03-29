//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tuple>

namespace multi::details {

template <class T>
struct size;

template <class... Ts>
struct size<std::tuple<Ts...>> {
    static constexpr size_t value = sizeof...(Ts);
};

template <class T>
constexpr size_t size_v = size<T>::value;

}  // namespace multi::details