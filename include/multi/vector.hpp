//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <multi/details/type_sequence.hpp>

#include <tuple>
#include <iterator>
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
    template<std::size_t I, std::size_t... Is>
    using collect_reference = std::tuple<details::sequence_element_t<I, type_sequence>&, details::sequence_element_t<Is, type_sequence>&...>;
    template<std::size_t I, std::size_t... Is>
    using collect_const_reference =std::tuple<const details::sequence_element_t<I, type_sequence>&, const details::sequence_element_t<Is, type_sequence>&...>;
    template<std::size_t I>
    using pointer = details::sequence_element_t<I, type_sequence>*;
    template<std::size_t I>
    using const_pointer = const details::sequence_element_t<I, type_sequence>*;

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
        _data = static_cast<std::byte*>(_allocator.allocate_bytes(_capacity*packed_sizeof, max_alignof));
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

    constexpr operator bool() const noexcept {
        return !empty();
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
    inline auto data() -> pointer<I> {
        return column<I>(_data, _capacity);
    }

    template<std::size_t I>
    inline auto data() const -> const_pointer<I> {
        return column<I>(_data, _capacity);
    }

    inline auto operator[](const std::size_t& index) -> reference {
        return [this] <std::size_t I, std::size_t... Is> 
        (const std::size_t index, std::index_sequence<I, Is...>) {
            return collect<I, Is...>(index);
        }(index, index_sequence{});
    }

    inline auto operator[](const std::size_t& index) const -> const_reference {
        return [this] <std::size_t I, std::size_t... Is> 
        (const std::size_t& index, std::index_sequence<I, Is...>) {
            return collect<I, Is...>(index);
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

    template<std::size_t I, std::size_t... Is>
    constexpr auto collect(const std::size_t& index) -> collect_reference<I, Is...> {
        return std::tie((*(data<I>()+index)), (*(data<Is>()+index))...);
    }

    template<std::size_t I, std::size_t... Is>
    constexpr auto collect(const std::size_t& index) const -> collect_const_reference<I, Is...> {
        return std::tie((*(data<I>()+index)), (*(data<Is>()+index))...);
    }

    /*
     * Iterators
     */
    template<std::size_t I, std::size_t... Is>
    class iterator;

    template<std::size_t I, std::size_t... Is>
    class const_iterator;

    constexpr auto begin() noexcept {
        return [this] <std::size_t I, std::size_t... Is>
        (std::index_sequence<I, Is...>) {
            return iterator<I,Is...>(*this, 0);
        }(index_sequence{});
    }

    constexpr auto end() noexcept {
        return [this] <std::size_t I, std::size_t... Is>
        (std::index_sequence<I, Is...>) {
            return iterator<I,Is...>(*this, _size);
        }(index_sequence{});
    }

    constexpr auto begin() const noexcept {
        return [this] <std::size_t I, std::size_t... Is>
        (std::index_sequence<I, Is...>) {
            return const_iterator<I,Is...>(*this, 0);
        }(index_sequence{});
    }

    constexpr auto end() const noexcept {
        return [this] <std::size_t I, std::size_t... Is>
        (std::index_sequence<I, Is...>) {
            return const_iterator<I,Is...>(*this, _size);
        }(index_sequence{});
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

template<class T, class... Ts>
template<std::size_t I, std::size_t... Is>
class vector<T, Ts...>::iterator final {
public:
    using vector_type = vector<T, Ts...>;
    using value_type = std::tuple<details::sequence_element_t<I, vector_type::type_sequence>, details::sequence_element_t<Is, vector_type::type_sequence>...>;
    using reference = std::tuple<details::sequence_element_t<I, vector_type::type_sequence>&, details::sequence_element_t<Is, vector_type::type_sequence>&...>;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;

    /*
     * Constructors
     */
    constexpr iterator() = default;
    constexpr iterator(const iterator&) = default;
    constexpr iterator(iterator&&) = default;
    constexpr iterator(vector_type& vector, const std::size_t& index)
    :    _index(index), _vector(&vector) {}

    /*
     * Assignments
     */ 
    constexpr auto operator=(const iterator&) -> iterator& = default;
    constexpr auto operator=(iterator&&) -> iterator& = default;

    // Will compare vector pointer. Comparing iterators for different vectors undefined
    constexpr auto operator<=>(const iterator&) const = default;

    /*
     * Dereferencing
     */
    constexpr auto operator*() const -> reference {
        return _vector->template collect<I, Is...>(_index);
    }

    /*
     * Increment
     */
    constexpr auto operator++() -> iterator& {
        return operator+=(1);
    }

    constexpr auto operator++(int) -> iterator {
        iterator ret = *this; 
        operator+=(1);
        return ret;
    }

    constexpr auto operator+=(const difference_type& diff) -> iterator& {
        _index += diff; 
        return *this;
    }

    constexpr auto operator+(const difference_type& diff) const -> iterator {
        return iterator(*_vector, _index + diff);
    }

    /*
     * Decrement
     */
    constexpr auto operator--() -> iterator& {
        return operator-=(1);
    }

    constexpr auto operator--(int) -> iterator {
        iterator ret = *this; 
        operator-=(1);
        return ret;
    }

    constexpr auto operator-=(const difference_type& diff) -> iterator& {
        _index -= diff; 
        return *this;
    }

    constexpr auto operator-(const difference_type& diff) const -> iterator {
        return iterator(*_vector, _index - diff);
    }

    /*
     * Difference
     */
    constexpr auto operator-(const iterator& rhs) const -> difference_type {
        return _index - rhs._index;
    }

    constexpr auto operator[](const std::size_t& idx) const -> reference {
        return _vector->template collect<I, Is...>(_index+idx);
    }

    friend constexpr auto operator+(const difference_type diff, const iterator& it) {
        return it + diff;
    }

    /*
     * Select
     */
    template<std::size_t J, std::size_t... Js>
    constexpr auto select() const {
        return vector_type::iterator<J, Js...>(*_vector, _index);
    }

private:
    difference_type _index;
    mutable vector_type* _vector;
};

template<class T, class... Ts>
template<std::size_t I, std::size_t... Is>
class vector<T, Ts...>::const_iterator final {
public:
    using vector_type = vector<T, Ts...>;
    using value_type = std::tuple<const details::sequence_element_t<I, vector_type::type_sequence>&, const details::sequence_element_t<Is, vector_type::type_sequence>&...>;
    using reference = std::tuple<const details::sequence_element_t<I, vector_type::type_sequence>&, const details::sequence_element_t<Is, vector_type::type_sequence>&...>;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;

    /*
     * Constructors
     */
    constexpr const_iterator() = default;
    constexpr const_iterator(const const_iterator&) = default;
    constexpr const_iterator(const_iterator&&) = default;
    constexpr const_iterator(const vector_type& vector, const std::size_t& index)
    :    _index(index), _vector(&vector) {}

    /*
     * Assignments
     */ 
    constexpr auto operator=(const const_iterator&) -> const_iterator& = default;
    constexpr auto operator=(const_iterator&&) -> const_iterator& = default;

    // Will compare vector pointer. Comparing iterators for different vectors undefined
    constexpr auto operator<=>(const const_iterator&) const = default;

    /*
     * Dereferencing
     */
    constexpr auto operator*() const -> reference {
        return _vector->template collect<I, Is...>(_index);
    }

    /*
     * Increment
     */
    constexpr auto operator++() -> const_iterator& {
        return operator+=(1);
    }

    constexpr auto operator++(int) -> const_iterator {
        const_iterator ret = *this; 
        operator+=(1);
        return ret;
    }

    constexpr auto operator+=(const difference_type& diff) -> const_iterator& {
        _index += diff; 
        return *this;
    }

    constexpr auto operator+(const difference_type& diff) const -> const_iterator {
        return const_iterator(*_vector, _index + diff);
    }

    /*
     * Decrement
     */
    constexpr auto operator--() -> const_iterator& {
        return operator-=(1);
    }

    constexpr auto operator--(int) -> const_iterator {
        const_iterator ret = *this; 
        operator-=(1);
        return ret;
    }

    constexpr auto operator-=(const difference_type& diff) -> const_iterator& {
        _index -= diff; 
        return *this;
    }

    constexpr auto operator-(const difference_type& diff) const -> const_iterator {
        return const_iterator(*_vector, _index - diff);
    }

    /*
     * Difference
     */
    constexpr auto operator-(const const_iterator& rhs) const -> difference_type {
        return _index - rhs._index;
    }

    constexpr auto operator[](const std::size_t& idx) const -> reference {
        return _vector->template collect<I, Is...>(_index+idx);
    }

    friend constexpr auto operator+(const difference_type diff, const const_iterator& it) {
        return it + diff;
    }

    /*
     * Select
     */
    template<std::size_t J, std::size_t... Js>
    constexpr auto select() const {
        return vector_type::const_iterator<J, Js...>(*_vector, _index);
    }

private:
    difference_type _index;
    const vector_type* _vector;
};

} // namespace multi