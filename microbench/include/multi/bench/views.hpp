//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <ranges>

namespace multi::bench {

template <std::size_t I>
struct elements_fn final {
    template <std::ranges::range R>
    auto operator()(R&& range) const {
        return std::ranges::elements_view<std::views::all_t<R>, I>(
            std::forward<R>(range));
    }

    template <std::ranges::range R>
    friend decltype(auto) operator|(R&& range, const elements_fn& fun) {
        return fun(std::forward<R>(range));
    }
};

template <std::size_t I>
constexpr elements_fn<I> elements;

}  // namespace multi::bench