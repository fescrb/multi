//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <multi/details/type_sequence.hpp>

#include <tuple>
#include <memory_resource>

namespace multi {

template<class T, class... Ts>
class vector {
public:
    using value_type = std::tuple<T, Ts...>;
    using allocator_type = std::pmr::polymorphic_allocator<std::byte>;

    constexpr vector(allocator_type allocator = {}) : _data(nullptr), _size(0), _capacity(0), _allocator(allocator) {}

    constexpr auto size() const noexcept -> std::size_t {
        return _size;
    }

    constexpr auto empty() const noexcept -> bool {
        return size() == 0;
    }

private:
    std::byte* _data;
    std::size_t _size;
    std::size_t _capacity;
    allocator_type _allocator;
};

} // namespace multi