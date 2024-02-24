//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <utility>

namespace multi {

template <std::size_t I, class T>
    requires requires(T t) { std::get<I>(t); }
decltype(auto) get(T&& t) noexcept {
    return std::get<I>(std::forward<T>(t));
}

}  // namespace multi