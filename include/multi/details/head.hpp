//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tuple>

namespace multi::details {

template <class T>
struct head;

template <class T, class... Ts>
struct head<std::tuple<T, Ts...>> {
    using type = T;
};

template <class T>
using head_t = head<T>::type;

}  // namespace multi::details