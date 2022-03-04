//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <multi/select.hpp>

namespace multi {

template<class T, std::size_t I>
concept getable = requires (const T& t) {t.template get<I>();};

template<class T, std::size_t I>
concept std_getable = requires (const T& t) { std::get<I>(t); };

template<std::size_t I>
struct get_closure final {
    template<class R>
        requires std::ranges::range<R>
    auto operator()(R&& range) const {
        return select_view<R, I>(range);
    }

    template<class T>
        requires getable<T, I>
    auto operator()(T&& val) const {
        return val.template get<I>();
    }

    template<class T>
        requires std_getable<T, I>
    auto operator()(T&& val) const {
        return std::get<I>(val);
    }

    template<class R>
        requires std::ranges::range<R>
    friend decltype(auto) operator|(R&& range, const get_closure& fun) {
        return fun(range);
    }
};

template<std::size_t I>
constexpr get_closure<I> get;

} // namespace multi