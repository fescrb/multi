//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tuple>

namespace multi::details {

template <class T, class U>
struct prepend;

template <class T, class... Us>
struct prepend<T, std::tuple<Us...>> {
    using type = std::tuple<T, Us...>;
};

template <class T, class U>
using prepend_t = prepend<T, U>::type;

}  // namespace multi::details