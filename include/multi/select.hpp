//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <iterator>
#include <ranges>
#include <tuple>

namespace multi {

template<class T, std::size_t I, std::size_t... Is>
concept selectable = requires (const T& t) {
    t.template select<I, Is...>();
};

template<class R, std::size_t I, std::size_t... Is>
    requires std::ranges::view<R> && selectable<std::ranges::iterator_t<R>, I, Is...>
class select_view final : public std::ranges::view_interface<select_view<R, I, Is...>> {
public:
    auto begin() const { return std::ranges::begin(_range).template select<I, Is...>(); }
    auto end() const { return std::ranges::end(_range).template select<I, Is...>(); }

    
    constexpr select_view() = default;
    constexpr select_view(const R range) : _range(range) {}
    constexpr select_view(R&& range) : _range(std::move(range)) {}

private:
    mutable R _range;
};

template<std::size_t I, std::size_t... Is>
struct select_fn final {
    template<class R>
    auto operator()(const R& range) const {
        return select_view<R, I, Is...>(range);
    }

    template<class T>
        requires selectable<T, I, Is...>
    auto operator()(const T& val) const {
        return val.template select<I, Is...>();
    }

    template<class... Ts>
    auto operator()(const std::tuple<Ts...>& val) const {
        return std::tie(std::get<I>(val), std::get<Is>(val)...);
    }

    template<class R>
        requires std::ranges::range<R>
    friend decltype(auto) operator|(const R& range, const select_fn& fun) {
        return fun(range);
    }
};

template<std::size_t I, std::size_t... Is>
constexpr select_fn<I, Is...> select;

} // namespace multi