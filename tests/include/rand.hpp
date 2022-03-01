//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


#include <limits>

template<class... Ts>
auto rand_tuple() {
    return std::make_tuple(static_cast<Ts>(rand() % static_cast<int>(std::numeric_limits<Ts>::max()))...);
}