//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include <multi/array.hpp>
#include <multi/vector.hpp>

namespace multi::bench {

template <class T>
struct container;

template <class T>
struct container<std::vector<T>> {
    using container_type = std::vector<T>;
    static std::unique_ptr<container_type> allocate(std::size_t size) {
        return std::make_unique<container_type>(size);
    }
};

template <class T, std::size_t S>
struct container<std::array<T, S>> {
    using container_type = std::array<T, S>;
    static std::unique_ptr<container_type> allocate(std::size_t size) {
        assert(size <= S);
        return std::make_unique<container_type>();
    }
};

template <class... Ts>
struct container<multi::vector<Ts...>> {
    using container_type = multi::vector<Ts...>;
    static std::unique_ptr<container_type> allocate(std::size_t size) {
        return std::make_unique<container_type>(size);
    }
};

template <std::size_t S, class... Ts>
struct container<multi::array<S, Ts...>> {
    using container_type = multi::array<S, Ts...>;
    static std::unique_ptr<container_type> allocate(std::size_t size) {
        assert(size <= S);
        return std::make_unique<container_type>();
    }
};

}  // namespace multi::bench