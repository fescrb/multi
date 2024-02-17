//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <multi/reference_tuple.hpp>

namespace multi {
template <class... Ts>
    requires(sizeof...(Ts) > 1)
void PrintTo(const multi::reference_tuple<Ts...>& tuple, std::ostream* os) {
    *os << get<0>(tuple) << " ";
    PrintTo(tail(tuple), os);
}
template <class T>
void PrintTo(const multi::reference_tuple<T>& tuple, std::ostream* os) {
    *os << get<0>(tuple);
}

}  // namespace multi