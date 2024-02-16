//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <tuple>

namespace multi {

template <class... Ts>
    requires(sizeof...(Ts) > 1)
class reference_tuple {
    using value_type = std::tuple<Ts...>;
    using reference = std::tuple<Ts&...>;
    static constexpr std::size_t num_elements = sizeof...(Ts);
    using index_sequence = std::make_index_sequence<num_elements>;
    void* _refs[num_elements];

public:
    template <std::size_t I>
    friend std::tuple_element_t<I, value_type>& get(
        const reference_tuple& tuple) {
        return *static_cast<std::tuple_element_t<I, value_type>*>(
            tuple._refs[I]);
    }

    explicit reference_tuple(Ts&... refs) : _refs{std::addressof(refs)...} {}

    operator reference() const {
        return [this]<std::size_t... Is>(std::index_sequence<Is...>) {
            return reference{get<Is>(*this)...};
        }(index_sequence{});
    }

    reference_tuple& operator=(const std::tuple<Ts...>& val) {
        static_cast<reference>(*this) = val;
        return *this;
    }

    const reference_tuple& operator=(const std::tuple<Ts...>& val) const {
        static_cast<reference>(*this) = val;
        return *this;
    }
};

}  // namespace multi