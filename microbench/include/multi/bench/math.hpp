//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <concepts>

namespace multi::bench {

template <std::integral T>
constexpr T pow(T base, T exponent) noexcept {
    if (exponent == 0) return 1;
    T r = base;
    for (T i = 1; i < exponent; ++i) {
        r *= base;
    }
    return r;
}

}  // namespace multi::bench
