//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <tuple>

namespace multi {

template <class... Ts>
    requires(sizeof...(Ts) > 0)
class reference_tuple {
    using value_type = std::tuple<Ts...>;
    using reference = std::tuple<Ts&...>;
    using pointer = std::tuple<Ts*...>;
    static constexpr std::size_t num_elements = sizeof...(Ts);
    using index_sequence = std::make_index_sequence<num_elements>;
    pointer _refs;

    template <class T>
    static T& identity(T& t) {
        return t;
    }

    template <class T>
    static T& identity(T&& t) = delete;

public:
    template <std::size_t I>
    std::tuple_element_t<I, reference> get() const {
        return *std::get<I>(this->_refs);
    }

    template <class... Us>
        requires requires(Us... us) {
            pointer{std::addressof(identity(std::forward<Us>(us)))...};
        }
    explicit reference_tuple(Us&&... refs)
        : _refs{std::addressof(identity(std::forward<Us>(refs)))...} {}

    template <class... Us>
    friend auto tail(const reference_tuple& rhs) {
        return [&rhs]<std::size_t I, std::size_t... Is>(
                   std::index_sequence<I, Is...>) {
            return reference_tuple<std::tuple_element_t<Is, value_type>...>{
                rhs.get<Is>()...};
        }(index_sequence{});
    }

    reference tie() const {
        return [this]<std::size_t... Is>(std::index_sequence<Is...>) {
            return reference{get<Is>()...};
        }(index_sequence{});
    }

    operator reference() const { return tie(); }

    value_type tuple() const {
        return [this]<std::size_t... Is>(std::index_sequence<Is...>) {
            return value_type{get<Is>()...};
        }(index_sequence{});
    }

    operator value_type() const { return tuple(); }

    template <class... Us>
        requires std::assignable_from<reference&, std::tuple<Us...>>
    reference_tuple& operator=(const std::tuple<Us...>& val) {
        tie() = val;
        return *this;
    }

    template <class... Us>
        requires std::assignable_from<reference&, std::tuple<Us&...>>
    reference_tuple& operator=(const reference_tuple<Us...>& val) {
        tie() = val.tie();
        return *this;
    }

    template <class... Us>
        requires std::assignable_from<reference&, std::tuple<Us...>>
    const reference_tuple& operator=(const std::tuple<Us...>& val) const {
        tie() = val;
        return *this;
    }

    template <class... Us>
        requires std::assignable_from<reference&, std::tuple<Us&...>>
    const reference_tuple& operator=(const reference_tuple<Us...>& val) const {
        tie() = val.tie();
        return *this;
    }

    reference_tuple& operator=(const reference_tuple& val) {
        tie() = val.tie();
        return *this;
    }

    const reference_tuple& operator=(const reference_tuple& val) const {
        tie() = val.tie();
        return *this;
    }

    template <class... Us>
    bool operator==(const reference_tuple<Us...>& rhs) const {
        return tie() == rhs.tie();
    }

    template <class... Us>
    auto operator<=>(const reference_tuple<Us...>& rhs) const {
        return tie() <=> rhs.tie();
    }
    template <class... Us>
    bool operator==(const std::tuple<Us...>& rhs) const {
        return tie() == rhs;
    }

    template <class... Us>
    auto operator<=>(const std::tuple<Us...>& rhs) const {
        return tie() <=> rhs;
    }
};

template <class... Ts>
reference_tuple(Ts&...) -> reference_tuple<Ts...>;

template <std::size_t I, class... Ts>
decltype(auto) get(const reference_tuple<Ts...>& t) noexcept {
    return t.template get<I>();
}

}  // namespace multi