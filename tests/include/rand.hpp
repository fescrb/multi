//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


#include <cstdlib>
#include <limits>
#include <type_traits>
#include <vector>
#include <ranges>

template<class T>
auto typed_rand() {
    return static_cast<T>(rand() % std::numeric_limits<T>::max());
}

template<class T>
    requires std::is_floating_point_v<T>
auto typed_rand() {
    return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
}


template<class... Ts>
auto rand_tuple() {
    return std::make_tuple(typed_rand<Ts>()...);
}
 
template<class T>
auto rand_buffer(const std::size_t size) -> std::vector<T> {
    std::vector<T> rand_v;
    rand_v.reserve(size);
    for(auto _ : std::views::iota(0u, size)) {
        rand_v.push_back(typed_rand<T>());
    }
    return rand_v;
}