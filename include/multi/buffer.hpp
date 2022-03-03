//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <multi/details/type_sequence.hpp>
#include <memory_resource>
#include <cstring>

#include <iostream>

namespace multi {

template<class T, class... Ts>
struct buffer {
    using allocator_type = std::pmr::polymorphic_allocator<std::byte>;
    using type_sequence = details::type_sequence<T, Ts...>;
    using index_sequence = std::make_index_sequence<type_sequence::size()>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    template<std::size_t I>
    using pointer = details::sequence_element_t<I, type_sequence>*;
    template<std::size_t I>
    using const_pointer = const details::sequence_element_t<I, type_sequence>*;

    constexpr static std::size_t member_sizeof = details::packed_sizeof<type_sequence>;
    constexpr static std::size_t alignment = details::max_alignof<type_sequence>;

    constexpr static auto round_capacity(const size_type capacity) -> size_type {
        return capacity + (capacity % alignment ? (alignment - (capacity % alignment)) : 0);
    }

    constexpr buffer(allocator_type allocator = {}) : data(nullptr), capacity(0), _allocator(allocator) {}

    constexpr buffer(const buffer& other) 
    :   buffer(other, other._allocator) {}

    constexpr buffer(const buffer& other, allocator_type _allocator)
    :   buffer(_allocator) {
        operator=(other);
    }

    constexpr auto operator=(const buffer& other) -> buffer& {
        if (this != &other) {
            clear();
            capacity = other.capacity;
            if (capacity) {
                data = static_cast<std::byte*>(
                    _allocator.allocate_bytes(capacity*member_sizeof, alignment)
                );
                std::memcpy(data, other.data, member_sizeof*capacity);
            }
        }
        return *this;
    }

    auto resize(const size_type new_capacity) -> void{
        if (new_capacity) {
            std::byte* new_data = static_cast<std::byte*>(
                _allocator.allocate_bytes(new_capacity*member_sizeof, alignment)
            );
            if (data) {
                [this] <std::size_t... Is> 
                (std::byte* new_data, const size_type new_capacity, std::index_sequence<Is...>) {
                    return std::min({copy_column<Is>(new_data, new_capacity, data, capacity)...,});
                }(new_data, new_capacity, index_sequence{});
                clear();
            }
            data = new_data;
            capacity = new_capacity;
        }
    }

    constexpr auto clear() -> void {
        if (data)
            _allocator.deallocate_bytes(data, capacity*member_sizeof, alignment);
        data = nullptr;
        capacity = 0;
    }

    constexpr ~buffer() {
        clear();
    }
    
    template<std::size_t I>
        requires (I < type_sequence::size())
    constexpr auto column() -> pointer<I> {
        return column<I>(data, capacity);
    }

    template<std::size_t I>
        requires (I < type_sequence::size())
    constexpr auto column() const -> const_pointer<I> {
        return column<I>(data, capacity);
    }

    constexpr auto get_allocator() const -> allocator_type {
        return _allocator;
    }

    std::byte* data;
    size_type capacity;

private:
    allocator_type _allocator;
    
    template<std::size_t I>
    constexpr static auto column(const std::byte* data, const std::size_t& capacity) -> const details::sequence_element_t<I, type_sequence>* {
        return reinterpret_cast<const details::sequence_element_t<I, type_sequence>*>(
            data + (capacity * details::packed_sizeof<details::take_subsequence_t<I, type_sequence>>));
    }

    template<std::size_t I>
    constexpr static auto column(std::byte* data, const std::size_t& capacity) -> details::sequence_element_t<I, type_sequence>* {
        return reinterpret_cast<details::sequence_element_t<I, type_sequence>*>(
            data + (capacity * details::packed_sizeof<details::take_subsequence_t<I, type_sequence>>));
    }

    template<std::size_t I>
    constexpr static auto copy_column(std::byte* dst, const std::size_t& dst_capacity, const std::byte* src, const std::size_t& src_capacity) -> std::byte* {
        return reinterpret_cast<std::byte*>(std::memcpy(
            column<I>(dst, dst_capacity),
            column<I>(src, src_capacity), 
            std::min(dst_capacity, src_capacity)*sizeof(details::sequence_element_t<I, type_sequence>)
        ));
    }
};

} // namespace multi