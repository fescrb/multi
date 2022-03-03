//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <multi/details/type_sequence.hpp>
#include <multi/details/index.hpp>

#include <multi/buffer.hpp>

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
    using buffer_type = buffer<T, Ts...>;
    using size_type = buffer_type::size_type;
    using type_sequence = typename buffer<T, Ts...>::type_sequence;
    using index_sequence = typename buffer<T, Ts...>::index_sequence;
    using allocator_type = typename buffer<T, Ts...>::allocator_type;
    template<std::size_t I, std::size_t... Is>
    using collect_reference = std::tuple<details::sequence_element_t<I, type_sequence>&, details::sequence_element_t<Is, type_sequence>&...>;
    template<std::size_t I, std::size_t... Is>
    using collect_const_reference =std::tuple<const details::sequence_element_t<I, type_sequence>&, const details::sequence_element_t<Is, type_sequence>&...>;
    template<std::size_t I>
    using pointer = details::sequence_element_t<I, type_sequence>*;
    template<std::size_t I>
    using const_pointer = const details::sequence_element_t<I, type_sequence>*;

    constexpr vector(allocator_type allocator = {}) : _buffer(allocator), _size(0) {}

    constexpr vector(const vector& other)
    :   _buffer(other._buffer), _size(other._size) {}

    constexpr ~vector() {}

    constexpr auto operator=(const vector&) -> vector& = default;

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
        return _buffer.capacity;
    }

    constexpr auto get_allocator() const noexcept -> allocator_type {
        return _buffer.get_allocator();
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
        return _buffer.template column<I>();
    }

    template<std::size_t I>
    inline auto data() const -> const_pointer<I> {
        return _buffer.template column<I>();
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
        return std::tie((*(_buffer.template column<I>()+index)), (*(_buffer.template column<Is>()+index))...);
    }

    template<std::size_t I, std::size_t... Is>
    constexpr auto collect(const std::size_t& index) const -> collect_const_reference<I, Is...> {
        return std::tie((*(_buffer.template column<I>()+index)), (*(_buffer.template column<Is>()+index))...);
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
            return iterator<I,Is...>(this, 0);
        }(index_sequence{});
    }

    constexpr auto end() noexcept {
        return [this] <std::size_t I, std::size_t... Is>
        (std::index_sequence<I, Is...>) {
            return iterator<I,Is...>(this, _size);
        }(index_sequence{});
    }

    constexpr auto begin() const noexcept {
        return [this] <std::size_t I, std::size_t... Is>
        (std::index_sequence<I, Is...>) {
            return const_iterator<I,Is...>(this, 0);
        }(index_sequence{});
    }

    constexpr auto end() const noexcept {
        return [this] <std::size_t I, std::size_t... Is>
        (std::index_sequence<I, Is...>) {
            return const_iterator<I,Is...>(this, _size);
        }(index_sequence{});
    }

    /*
     * Modifiers
     */
    constexpr auto push_back(const value_type& value) -> void {
        if(_size + 1 > _buffer.capacity) {
            reserve(std::max(_buffer.capacity, 1ul) * 2ul);
        }
        operator[](_size) = value;
        ++_size;
    }

    constexpr auto reserve(const std::size_t& capacity) -> void {
        if(capacity > _buffer.capacity);
            _buffer.resize(capacity);
    }

private:
    buffer_type _buffer;
    size_type _size;
};

template<class T, class... Ts>
template<std::size_t I, std::size_t... Is>
class vector<T, Ts...>::iterator : public details::index {
public:
    using container_type = vector<T, Ts...>;
    using value_type = std::tuple<details::sequence_element_t<I, container_type::type_sequence>, details::sequence_element_t<Is, container_type::type_sequence>...>;
    using reference = std::tuple<details::sequence_element_t<I, container_type::type_sequence>&, details::sequence_element_t<Is, container_type::type_sequence>&...>;
    using iterator_category = std::random_access_iterator_tag;

    /*
     * Constructors
     */
    constexpr iterator() = default;
    constexpr iterator(const iterator&) = default;
    constexpr iterator(iterator&&) = default;
    constexpr iterator(container_type* vector, const std::size_t& index)
    :   details::index(index), _vector(vector) {}

    /*
     * Assignments
     */ 
    template<std::size_t J, std::size_t... Js>
    constexpr auto operator=(const iterator<J, Js...>& other) -> iterator& {
        _index = other._index;
        _vector = other._vector;
        return *this;
    }
    template<std::size_t J, std::size_t... Js>
    constexpr auto operator=(iterator<J, Js...>&& other) -> iterator& {
        _index = std::move(other._index);
        _vector = std::move(other._vector);
        return *this;
    }

    constexpr auto operator=(const iterator&) -> iterator& = default;
    constexpr auto operator=(iterator&&) -> iterator& = default;

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
        return iterator(_vector, _index + diff);
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
        return iterator(_vector, _index - diff);
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
        return container_type::iterator<J, Js...>(_vector, _index);
    }

private:
    mutable container_type* _vector;
};

template<class T, class... Ts>
template<std::size_t I, std::size_t... Is>
class vector<T, Ts...>::const_iterator : public details::index {
public:
    using container_type = vector<T, Ts...>;
    using value_type = std::tuple<const details::sequence_element_t<I, container_type::type_sequence>&, const details::sequence_element_t<Is, container_type::type_sequence>&...>;
    using reference = std::tuple<const details::sequence_element_t<I, container_type::type_sequence>&, const details::sequence_element_t<Is, container_type::type_sequence>&...>;
    using iterator_category = std::random_access_iterator_tag;

    /*
     * Constructors
     */
    constexpr const_iterator() = default;
    constexpr const_iterator(const const_iterator&) = default;
    constexpr const_iterator(const_iterator&&) = default;
    constexpr const_iterator(const container_type* vector, const std::size_t& index)
    :    details::index(index), _vector(vector) {}

    /*
     * Assignments
     */ 
    template<std::size_t J, std::size_t... Js>
    constexpr auto operator=(const const_iterator<J, Js...>& other) -> const_iterator& {
        _index = other._index;
        _vector = other._vector;
        return *this;
    }
    template<std::size_t J, std::size_t... Js>
    constexpr auto operator=(const_iterator<J, Js...>&& other) -> const_iterator& {
        _index = std::move(other._index);
        _vector = std::move(other._vector);
        return *this;
    }

    constexpr auto operator=(const const_iterator&) -> const_iterator& = default;
    constexpr auto operator=(const_iterator&&) -> const_iterator& = default;

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
        return const_iterator(_vector, _index + diff);
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
        return const_iterator(_vector, _index - diff);
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
        return container_type::const_iterator<J, Js...>(_vector, _index);
    }

private:
    const container_type* _vector;
};

} // namespace multi