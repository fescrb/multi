//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <utility>
#include <iterator>
#include <ranges>
#include <tuple>

namespace multi {

template<class T, std::size_t I, std::size_t... Is>
concept selectable = requires (const T& t) {
    t.template select<I, Is...>();
};

template<class T, std::size_t I, std::size_t... Is>
using select_t = decltype(std::declval<T>().template select<I, Is...>());

template<class V, std::size_t I, std::size_t... Is>
    requires std::ranges::view<V> && selectable<std::ranges::iterator_t<V>, I, Is...>
class select_view : public std::ranges::view_interface<select_view<V, I, Is...>> {
public:
    using iterator_type = select_t<std::ranges::iterator_t<V>, I, Is...>;
    auto begin() const { return std::ranges::begin(_view).template select<I, Is...>(); }
    auto end() const { return std::ranges::end(_view).template select<I, Is...>();; }

    
    constexpr select_view() = default;
    constexpr select_view(V&& view) : _view(std::forward<V>(view)) {}

    constexpr select_view(const select_view&) = default;
    constexpr select_view(select_view&&) = default;

    constexpr auto operator=(const select_view&) -> select_view& = default;
    constexpr auto operator=(select_view&&) -> select_view& = default;

private:
    V _view;
};

template<class R, std::size_t I, std::size_t... Is>
select_view(R&& range) -> select_view<std::ranges::ref_view<R>, I, Is...>;

template<std::size_t I, std::size_t... Is>
struct select_fn final {
    template<class R>
    auto operator()(R&& range) const {
        return select_view<std::ranges::views::all_t<R>, I, Is...>(std::ranges::ref_view(std::forward<R>(range)));
    }
    template<class R>
        requires std::ranges::view<R>
    auto operator()(R&& range) const {
        return select_view<R, I, Is...>(std::forward<R>(range));
    }

    template<class T>
        requires selectable<T, I, Is...>
    auto operator()(T&& val) const {
        return val.template select<I, Is...>();
    }

    template<class... Ts>
    auto operator()(const std::tuple<Ts...>& val) const {
        return std::tie(std::get<I>(val), std::get<Is>(val)...);
    }

    template<class R>
        requires std::ranges::range<R>
    friend decltype(auto) operator|(R& range, const select_fn& fun) {
        return fun(range);
    }
};

template<std::size_t I, std::size_t... Is>
constexpr select_fn<I, Is...> select;

} // namespace multi