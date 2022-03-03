//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <utility>
#include <tuple>

#include <multi/buffer.hpp>

namespace multi {

template<class Key, class T, class... Ts>
class map {
    // Useful alias to refer to the mapped types
    using vector_type = vector<T, Ts...>;
public:
    using value_type = std::pair<const Key, vector_type::value_type>;
    using reference = std::pair<const Key, vector_type::reference>;
    using const_reference = std::tuple<const T&, const Ts&...>;

};

} // namespace multi