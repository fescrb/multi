//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tuple>

namespace multi::details {

template <class T>
struct tail;

template <class T, class... Ts>
struct tail<std::tuple<T, Ts...>> {
    using type = std::tuple<Ts...>;
};

template <class T>
using tail_t = tail<T>::type;

}  // namespace multi::details