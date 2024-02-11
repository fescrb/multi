//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


#include <limits>
#include <concepts>


template<class T>
struct rand_value {
    T value;
    rand_value() {
        value = static_cast<T>(rand() % std::numeric_limits<T>::max());
    }
};

template<>
struct rand_value<bool> {
    bool value;
    rand_value() {
        value = rand() % 2 == 0;
    }
};

template<class T>
requires std::floating_point<T>
struct rand_value<T> {
    T value;
    rand_value() {
        value = static_cast<T>(rand()) / std::numeric_limits<int>::max();
    }
};

template<class T>
T make_rand() {
    return rand_value<T>().value;
}

template<class... Ts>
std::tuple<Ts...> rand_tuple() {
    return std::tuple<Ts...>(make_rand<Ts>()...);
}