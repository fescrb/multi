//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <concepts>
#include <iterator>
#include <ranges>
#include <tuple>
#include <utility>

#include "multi/details/tuple_like.hpp"

namespace multi {

template <std::ranges::input_range R, size_t N, size_t... Ns>
    requires std::ranges::view<R> &&
             multi::details::tuple_like<std::ranges::range_value_t<R>>
class select_view
    : public std::ranges::view_interface<select_view<R, N, Ns...>> {
    R mBase;

    template <bool>
    class iterator;

    template <bool>
    class sentinel;

public:
    select_view()
        requires std::default_initializable<R>
    = default;
    constexpr explicit select_view(R b) : mBase(std::move(b)) {}

    constexpr R base() const&
        requires std::copy_constructible<R>
    {
        return mBase;
    }

    constexpr R base() && { return std::move(mBase); }

    constexpr auto begin()
        requires std::ranges::range<R>
    {
        return iterator<false>{std::ranges::begin(mBase)};
    }

    constexpr auto begin() const
        requires std::ranges::range<const R>
    {
        return iterator<true>{std::ranges::begin(mBase)};
    }

    constexpr auto end()
        requires(std::ranges::range<R>)
    {
        return sentinel<false>(std::ranges::end(mBase));
    }

    constexpr auto end() const
        requires(std::ranges::common_range<R>)
    {
        return iterator<false>(std::ranges::end(mBase));
    }

    constexpr auto end() const
        requires(std::ranges::range<const R>)
    {
        return sentinel<true>(std::ranges::end(mBase));
    }

    constexpr auto end() const
        requires(std::ranges::common_range<const R>)
    {
        return iterator<true>(std::ranges::end(mBase));
    }

    constexpr auto size()
        requires std::ranges::sized_range<R>
    {
        return std::ranges::size(mBase);
    }

    constexpr auto size() const
        requires std::ranges::sized_range<const R>
    {
        return std::ranges::size(mBase);
    }
};

template <std::ranges::input_range R, size_t N, size_t... Ns>
    requires std::ranges::view<R> &&
             multi::details::tuple_like<std::ranges::range_value_t<R>>
template <bool Const>
class select_view<R, N, Ns...>::iterator {
    using base_range_t = std::conditional_t<Const, const R, R>;
    using base_iterator_t = std::ranges::iterator_t<base_range_t>;
    base_iterator_t mCurrent = base_iterator_t();

    static constexpr auto get(const base_iterator_t& i) {
        return std::tie(std::get<N>(*i), std::get<Ns>(*i)...);
    }

    template <bool>
    friend class iterator;

    template <bool>
    friend class sentinel;

public:
    using iterator_concept =
        std::iterator_traits<base_iterator_t>::iterator_category;
    using value_type = decltype(get(std::declval<base_iterator_t>()));
    using difference_type = std::ranges::range_difference_t<base_range_t>;

    iterator()
        requires std::default_initializable<base_iterator_t>
    = default;
    explicit iterator(base_iterator_t c) : mCurrent(std::move(c)) {}

    constexpr const base_iterator_t& base() const& noexcept { return mCurrent; }

    constexpr base_iterator_t base() && { return std::move(mCurrent); }

    constexpr decltype(auto) operator*() const { return get(mCurrent); }

    constexpr iterator& operator++() {
        ++mCurrent;
        return *this;
    }

    constexpr void operator++(int) { ++mCurrent; }

    constexpr iterator operator++(int)
        requires std::ranges::forward_range<base_range_t>
    {
        auto temp = *this;
        ++mCurrent;
        return temp;
    }

    constexpr iterator& operator--()
        requires std::ranges::bidirectional_range<base_range_t>
    {
        --mCurrent;
        return *this;
    }

    constexpr iterator operator--(int)
        requires std::ranges::bidirectional_range<base_range_t>
    {
        auto temp = *this;
        --mCurrent;
        return temp;
    }

    constexpr iterator& operator+=(difference_type n)
        requires std::ranges::random_access_range<base_range_t>
    {
        mCurrent += n;
        return *this;
    }

    constexpr iterator& operator-=(difference_type n)
        requires std::ranges::random_access_range<base_range_t>
    {
        mCurrent -= n;
        return *this;
    }

    constexpr decltype(auto) operator[](difference_type n) const
        requires std::ranges::random_access_range<base_range_t>
    {
        return get(mCurrent + n);
    }

    friend constexpr bool operator==(const iterator& lhs, const iterator& rhs)
        requires std::equality_comparable<base_iterator_t>
    {
        return lhs.mCurrent == rhs.mCurrent;
    }

    friend constexpr bool operator<(const iterator& lhs, const iterator& rhs)
        requires std::ranges::random_access_range<base_range_t>
    {
        return lhs.mCurrent < rhs.mCurrent;
    }

    friend constexpr bool operator>(const iterator& lhs, const iterator& rhs)
        requires std::ranges::random_access_range<base_range_t>
    {
        return lhs.mCurrent > rhs.mCurrent;
    }

    friend constexpr bool operator<=(const iterator& lhs, const iterator& rhs)
        requires std::ranges::random_access_range<base_range_t>
    {
        return lhs.mCurrent <= rhs.mCurrent;
    }

    friend constexpr bool operator>=(const iterator& lhs, const iterator& rhs)
        requires std::ranges::random_access_range<base_range_t>
    {
        return lhs.mCurrent >= rhs.mCurrent;
    }

    friend constexpr auto operator<=>(const iterator& lhs, const iterator& rhs)
        requires std::ranges::random_access_range<base_range_t> &&
                 std::three_way_comparable<base_iterator_t>
    {
        return lhs.mCurrent <=> rhs.mCurrent;
    }

    friend constexpr iterator operator+(const iterator& lhs,
                                        difference_type rhs)
        requires std::ranges::random_access_range<base_range_t>
    {
        return lhs + rhs;
    }

    friend constexpr iterator operator+(difference_type lhs,
                                        const iterator& rhs)
        requires std::ranges::random_access_range<base_range_t>
    {
        return lhs + rhs;
    }

    friend constexpr iterator operator-(const iterator& lhs,
                                        difference_type rhs)
        requires std::ranges::random_access_range<base_range_t>
    {
        return lhs - rhs;
    }

    friend constexpr iterator operator-(difference_type lhs,
                                        const iterator& rhs)
        requires std::ranges::random_access_range<base_range_t>
    {
        return lhs - rhs;
    }

    friend constexpr difference_type operator-(const iterator& lhs,
                                               const iterator& rhs)
        requires std::sized_sentinel_for<base_iterator_t, base_iterator_t>
    {
        return lhs.mCurrent - rhs.mCurrent;
    }
};

template <std::ranges::input_range R, size_t N, size_t... Ns>
    requires std::ranges::view<R> &&
             multi::details::tuple_like<std::ranges::range_value_t<R>>
template <bool Const>
class select_view<R, N, Ns...>::sentinel {
    using base_range_t = std::conditional_t<Const, const R, R>;
    template <bool OtherConst>
    using base_iterator_t =
        std::ranges::iterator_t<std::conditional_t<OtherConst, const R, R>>;
    using base_sentinel_t = std::ranges::sentinel_t<base_range_t>;
    using difference_type = std::ranges::range_difference_t<base_range_t>;

    base_sentinel_t mEnd = base_sentinel_t();

    template <bool>
    friend class sentinel;

public:
    sentinel() = default;
    constexpr explicit sentinel(base_sentinel_t e) : mEnd(std::move(e)) {}
    constexpr sentinel(sentinel<!Const> other)
        requires Const && std::convertible_to<R, const R>
        : mEnd(move(other.end)) {}

    constexpr base_sentinel_t base() const { return mEnd; }

    template <bool OtherConst>
        requires std::sentinel_for<base_sentinel_t, base_iterator_t<OtherConst>>
    friend constexpr bool operator==(const iterator<OtherConst>& lhs,
                                     const sentinel& rhs) {
        return lhs.mCurrent == rhs.mEnd;
    }

    template <bool OtherConst>
        requires std::sized_sentinel_for<base_sentinel_t,
                                         base_iterator_t<OtherConst>>
    friend constexpr difference_type operator-(const iterator<OtherConst>& lhs,
                                               const sentinel& rhs) {
        return lhs.mCurrent - rhs.mEnd;
    }

    template <bool OtherConst>
        requires std::sized_sentinel_for<base_sentinel_t,
                                         base_iterator_t<OtherConst>>
    friend constexpr difference_type operator-(
        const sentinel& lhs, const iterator<OtherConst>& rhs) {
        return lhs.mEnd - rhs.mCurrent;
    }
};

// template<typename T>
// concept selectable =

template <std::size_t I, std::size_t... Is>
struct select_fn final {
    template <std::ranges::range R>
    auto operator()(R&& range) const {
        return select_view<std::views::all_t<R&&>, I, Is...>(
            std::forward<R>(range));
    }

    // TODO: this should work for all multi::details::tuple_like(s)
    template <class... Ts>
    auto operator()(const std::tuple<Ts...>& val) const {
        return std::tie(std::get<I>(val), std::get<Is>(val)...);
    }

    template <std::ranges::range R>
    friend decltype(auto) operator|(R&& range, const select_fn& fun) {
        return fun(std::forward<R>(range));
    }
};

template <std::size_t I, std::size_t... Is>
constexpr select_fn<I, Is...> select;

}  // namespace multi