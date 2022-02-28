//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <multi/details/type_sequence.hpp>

#include <tuple>
#include <memory_resource>
#include <cstring>
#include <cassert>

namespace multi {

template<class T, class... Ts>
class vector final {
public:
    using value_type = std::tuple<T, Ts...>;
    using reference = std::tuple<T&, Ts&...>;
    using const_reference = std::tuple<const T&, const Ts&...>;
    using type_sequence = details::type_sequence<T, Ts...>;
    using index_sequence = std::make_index_sequence<type_sequence::size()>;
    using allocator_type = std::pmr::polymorphic_allocator<std::byte>;

    constexpr vector(allocator_type allocator = {}) : _data(nullptr), _size(0), _capacity(0), _allocator(allocator) {}

    constexpr vector(const vector& other)
    :   _data(nullptr), _size(0), _capacity(0), _allocator(other._allocator) {
        operator=(other);
    }

    constexpr ~vector() {
        if (_data) {
            _allocator.deallocate_bytes(_data, _capacity*packed_sizeof, max_alignof);
        }
    }

    constexpr auto operator=(const vector& other) {
        if (_data) {
            _allocator.deallocate_bytes(_data, _capacity*packed_sizeof, max_alignof);
        }
        _size = other._size;
        _capacity = other._capacity;
        _data = _allocator.allocate_bytes(_capacity*packed_sizeof, max_alignof);
        std::memcpy(_data, other._data, _capacity*packed_sizeof);
    }

    /*
     * Member access
     */
    constexpr auto size() const noexcept -> std::size_t {
        return _size;
    }

    constexpr auto empty() const noexcept -> bool {
        return size() == 0;
    }

    constexpr auto capacity() const noexcept -> std::size_t {
        return _capacity;
    }

    constexpr auto get_allocator() const noexcept -> allocator_type {
        return _allocator;
    }

    /*
     * Element access
     */
    constexpr auto front() -> reference {
        return operator[](0);
    } 

    constexpr auto front() const -> const_reference {
        return operator[](0);
    }

    constexpr auto back() -> reference {
        return operator[](_size-1);
    } 

    constexpr auto back() const -> const_reference {
        return operator[](_size-1);
    } 

    template<std::size_t I>
    inline auto data() -> details::sequence_element_t<I, type_sequence>* {
        return column<I>(_data, _capacity);
    }

    template<std::size_t I>
    inline auto data() const -> const details::sequence_element_t<I, type_sequence>* {
        return column<I>(_data, _capacity);
    }

    inline auto operator[](const std::size_t& index) -> reference {
        return [this] <std::size_t... Is> 
        (const std::size_t index, std::index_sequence<Is...>) {
            return std::tie((*(data<Is>() + index))...);
        }(index, index_sequence{});
    }

    inline auto operator[](const std::size_t& index) const -> const_reference {
        return [this] <std::size_t... Is> 
        (const std::size_t& index, std::index_sequence<Is...>) {
            return std::tie((*(data<Is>()+index))...);
        }(index, index_sequence{});
    }

    constexpr auto at(const std::size_t& index) -> reference {
        assert(index < _size);
        return operator[](index);
    } 

    constexpr auto at(const std::size_t& index) const -> const_reference {
        assert(index < _size);
        return operator[](index);
    } 

    /*
     * Modifiers
     */
    constexpr auto push_back(const value_type& value) -> void {
        if(_size + 1 > _capacity) {
            reserve(std::max(_capacity, 1ul) * 2ul);
        }
        operator[](_size) = value;
        _size++;
    }

    constexpr auto reserve(const std::size_t& capacity) -> void {
        std::size_t new_capacity = round_capacity(capacity);
        std::byte* new_data = static_cast<std::byte*>(_allocator.allocate_bytes(new_capacity*packed_sizeof, max_alignof));
        if (_data) {
            [this] <std::size_t... Is>
            (std::byte* new_data, const std::size_t& new_capacity, std::index_sequence<Is...>) {
                return std::min({move_column<Is>(new_data, new_capacity, _data, _capacity, _size)...,});
            }(new_data, new_capacity, index_sequence{});
        }
        _allocator.deallocate_bytes(_data, _capacity*packed_sizeof, max_alignof);
        _data = new_data;
        _capacity = new_capacity;
    }

private:
    constexpr static std::size_t packed_sizeof = details::packed_sizeof<type_sequence>;
    constexpr static std::size_t max_alignof = details::max_alignof<type_sequence>;

    std::byte* _data;
    std::size_t _size;
    std::size_t _capacity;
    allocator_type _allocator;

    template<std::size_t I>
    constexpr static auto column(const std::byte* data, const std::size_t& capacity) -> const details::sequence_element_t<I, type_sequence>* {
        return reinterpret_cast<const details::sequence_element_t<I, type_sequence>*>(data + capacity * details::packed_sizeof<details::take_subsequence_t<I, type_sequence>>);
    }

    template<std::size_t I>
    constexpr static auto column(std::byte* data, const std::size_t& capacity) -> details::sequence_element_t<I, type_sequence>* {
        return reinterpret_cast<details::sequence_element_t<I, type_sequence>*>(data + capacity * details::packed_sizeof<details::take_subsequence_t<I, type_sequence>>);
    }

    template<std::size_t I>
    constexpr static auto move_column(std::byte* dst, const std::size_t& dst_capacity, const std::byte* src, const std::size_t& src_capacity, const std::size_t& size) -> std::byte* {
        return reinterpret_cast<std::byte*>(std::memcpy(
            column<I>(dst, dst_capacity),
            column<I>(src, src_capacity), 
            size*sizeof(details::sequence_element_t<I, type_sequence>)
        ));
    }

    constexpr static auto round_capacity(const std::size_t& capacity) -> std::size_t {
        return capacity + (max_alignof - (capacity % max_alignof));
    }
};

} // namespace multi