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

template<class R, std::size_t I, std::size_t... Is>
    requires std::ranges::range<R> && selectable<std::ranges::iterator_t<R>, I, Is...>
class select_view final : public std::ranges::view_interface<select_view<R, I, Is...>> {
public:
    using iterator_type = select_t<std::ranges::iterator_t<R>, I, Is...>;
    auto begin() const { return _begin; }
    auto end() const { return _end; }

    
    constexpr select_view() = default;
    constexpr select_view(R& range) 
    :   _begin(std::ranges::begin(range).template select<I, Is...>()),
        _end(std::ranges::end(range).template select<I, Is...>()) {}

    constexpr select_view(const select_view&) = default;
    constexpr select_view(select_view&&) = default;

    constexpr auto operator=(const select_view&) -> select_view& = default;
    constexpr auto operator=(select_view&&) -> select_view& = default;

private:
    iterator_type _begin, _end;
};

template<std::size_t I, std::size_t... Is>
struct select_closure final {
    template<class R>
        requires std::ranges::range<R>
    auto operator()(R&& range) const {
        return select_view<R, I, Is...>(range);
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
    friend decltype(auto) operator|(R&& range, const select_closure& fun) {
        return fun(range);
    }
};

template<std::size_t I, std::size_t... Is>
constexpr select_closure<I, Is...> select;

} // namespace multi