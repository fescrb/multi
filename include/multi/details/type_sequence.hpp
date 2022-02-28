//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstddef>

#include <multi/details/packed_sizeof.hpp>
#include <multi/details/max_alignof.hpp>

namespace multi {

namespace details {

template<std::size_t I, class Ts>
struct sequence_element;

template<std::size_t I, class Ts>
using sequence_element_t = typename sequence_element<I, Ts>::type;

template<class Is, class Ts>
struct subsequence;

template<class Is, class Ts>
using subsequence_t = typename subsequence<Is, Ts>::type;

template<class... Ts>
struct type_sequence {
    static constexpr std::size_t size() noexcept {
        return sizeof...(Ts);
    }
};

template<std::size_t I, class T, class... Ts>
struct sequence_element<I, type_sequence<T, Ts...>> : sequence_element<I-1, type_sequence<Ts...>>{};

template<class T, class... Ts>
struct sequence_element<0, type_sequence<T, Ts...>> {
    using type = T;
};

template<std::size_t... Is, class Ts>
struct subsequence<std::index_sequence<Is...>, Ts> {
    using type = type_sequence<typename sequence_element<Is, Ts>::type...>;
};

template<class... Ts>
constexpr std::size_t packed_sizeof<type_sequence<Ts...>> = packed_sizeof<Ts...>;

template<class... Ts>
constexpr std::size_t max_alignof<type_sequence<Ts...>> = max_alignof<Ts...>;

} // namespace details

}  // namespace multi