//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <initializer_list>
#include <tuple>

#include <multi/details/max_alignof.hpp>
#include <multi/details/multiple.hpp>
#include <multi/details/packed_sizeof.hpp>
#include <multi/details/type_sequence.hpp>
#include <multi/reference_tuple.hpp>
#include <multi/tuple_element.hpp>

namespace multi {

template <std::size_t N, class T, class... Ts>
class array {
    static constexpr std::size_t memory_alignment =
        details::max_alignof<T, Ts...>;
    static constexpr std::size_t num_elements =
        details::is_multiple_of_v<memory_alignment, N>
            ? N
            : details::next_multiple_of_v<memory_alignment, N>;
    static constexpr std::size_t memory_size =
        details::packed_sizeof<T, Ts...> * num_elements;
    static constexpr std::size_t num_columns = 1 + sizeof...(Ts);
    using type_sequence = details::type_sequence<T, Ts...>;
    using index_sequence = std::make_index_sequence<num_columns>;
    template <std::size_t I>
    static constexpr std::size_t column_stride =
        num_elements *
        details::packed_sizeof<details::take_subsequence_t<I, type_sequence>>;
    static constexpr std::array<std::size_t, num_columns> strides =
        []<std::size_t... Is>(std::index_sequence<Is...>) {
            return std::array{column_stride<Is>...};
        }(index_sequence{});

    alignas(memory_alignment) std::byte _data[memory_size];

    template <bool Const>
    class _iterator;

public:
    using value_type = std::tuple<T, Ts...>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = reference_tuple<T, Ts...>;
    using const_reference = reference_tuple<const T, const Ts...>;

    using iterator = _iterator<false>;
    using const_iterator = _iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<iterator>;

    template <std::size_t I>
        requires(I < num_columns)
    using pointer_type = tuple_element_t<I, value_type>*;
    template <std::size_t I>
        requires(I < num_columns)
    using const_pointer_type = const tuple_element_t<I, value_type>*;

    array() {}
    array(const array& other) { operator=(other); }
    array(const value_type (&a)[N]) { std::copy(a, a + N, begin()); }

    template <std::input_iterator It, std::sentinel_for<It> Sent>
    explicit array(It&& start, Sent&& stop) {
        std::copy(std::forward<It>(start), std::forward<Sent>(stop), begin());
    }

    template <std::ranges::input_range R>
    explicit array(R&& range) {
        std::ranges::copy(std::forward<R>(range), begin());
    }

    array(array&&) = delete;

    template <std::ranges::input_range R>
    array& operator=(R&& range) {
        std::ranges::copy(std::forward<R>(range), begin());
        return *this;
    }

    template <std::size_t I>
    constexpr tuple_element_t<I, reference> get(std::size_t idx) {
        return *(data<I>() + idx);
    }

    template <std::size_t I>
    constexpr tuple_element_t<I, const_reference> get(std::size_t idx) const {
        return *(data<I>() + idx);
    }

    constexpr reference operator[](std::size_t idx) {
        return [this, idx]<std::size_t... Is>(std::index_sequence<Is...>) {
            return reference(get<Is>(idx)...);
        }(index_sequence{});
    }

    constexpr const_reference operator[](std::size_t idx) const {
        return [this, idx]<std::size_t... Is>(std::index_sequence<Is...>) {
            return const_reference(get<Is>(idx)...);
        }(index_sequence{});
    }

    constexpr reference at(std::size_t idx) {
        assert(idx < N);
        return operator[](idx);
    }

    constexpr const_reference at(std::size_t idx) const {
        assert(idx < N);
        return operator[](idx);
    }

    constexpr reference front() { return operator[](0); }
    constexpr const_reference front() const { return operator[](0); }

    constexpr reference back() { return operator[](N - 1); }
    constexpr const_reference back() const { return operator[](N - 1); }

    template <std::size_t I>
    constexpr pointer_type<I> data() {
        return reinterpret_cast<pointer_type<I>>(_data + column_stride<I>);
    }
    template <std::size_t I>
    constexpr const_pointer_type<I> data() const {
        return reinterpret_cast<const_pointer_type<I>>(_data +
                                                       column_stride<I>);
    }

    iterator begin() noexcept { return iterator{_data, 0}; }
    const_iterator begin() const noexcept { return const_iterator{_data, 0}; }
    constexpr const_iterator cbegin() const noexcept {
        return const_iterator{_data, 0};
    }

    constexpr iterator end() { return iterator{_data, N}; }
    constexpr const_iterator end() const { return const_iterator{_data, N}; }
    constexpr const_iterator cend() const { return const_iterator{_data, N}; }

    constexpr reverse_iterator rbegin() {
        return reverse_iterator{_data, N - 1};
    }
    constexpr const_reverse_iterator rbegin() const {
        return const_reverse_iterator{_data, N - 1};
    }
    constexpr const_reverse_iterator crbegin() const {
        return const_reverse_iterator{_data, N - 1};
    }

    constexpr reverse_iterator rend() { return reverse_iterator{_data, -1}; }
    constexpr const_reverse_iterator rend() const {
        return const_reverse_iterator{_data, -1};
    }
    constexpr const_reverse_iterator rcend() const {
        return const_reverse_iterator{_data, -1};
    }

    constexpr bool empty() const noexcept { return N == 0; }
    constexpr std::size_t size() const noexcept { return N; }
    constexpr std::size_t max_size() const noexcept { return N; }

    constexpr auto operator==(const array& rhs) const {
        return std::ranges::equal(*this, rhs);
    }

    constexpr auto operator<=>(const array& rhs) const {
        return std::lexicographical_compare_three_way(begin(), end(),
                                                      rhs.begin(), rhs.end());
    }
};

template <std::size_t N, class T, class... Ts>
template <bool Const>
class array<N, T, Ts...>::_iterator {
    using data_type = std::conditional_t<Const, const std::byte*, std::byte*>;
    data_type _data = nullptr;
    array::difference_type _index = 0;

    friend class array;

    explicit _iterator(data_type data, array::difference_type index)
        : _data(data), _index{index} {}

public:
    using value_type =
        std::conditional_t<Const, array::const_reference, array::reference>;
    using reference = value_type;
    using const_reference = array::const_reference;
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;

    _iterator() = default;
    _iterator(const _iterator&) = default;
    _iterator(_iterator&&) = default;

    _iterator& operator=(const _iterator&) = default;
    _iterator& operator=(_iterator&&) = default;

    template <bool OtherConst>
        requires Const
    _iterator(_iterator<OtherConst> other)
        : _data(other._data), _index(other._index) {}

    template <std::size_t I>
    using column_type =
        std::conditional_t<Const, const tuple_element_t<I, array::value_type>*,
                           tuple_element_t<I, array::value_type>*>;

    template <std::size_t I>
    column_type<I> data() {
        return reinterpret_cast<column_type<I>>(_data +
                                                array::column_stride<I>);
    }

    template <std::size_t I>
    column_type<I> data() const {
        return reinterpret_cast<column_type<I>>(_data +
                                                array::column_stride<I>);
    }

    template <std::size_t I>
    tuple_element_t<I, reference> get(difference_type n = 0) {
        return *(data<I>() + _index + n);
    }

    template <std::size_t I>
    tuple_element_t<I, reference> get(difference_type n = 0) const {
        return *(data<I>() + _index + n);
    }

    reference operator*() {
        return
            [this]<std::size_t... Is>(std::index_sequence<Is...>) -> reference {
                return reference{get<Is>()...};
            }(array::index_sequence{});
    }

    reference operator*() const {
        return
            [this]<std::size_t... Is>(std::index_sequence<Is...>) -> reference {
                return reference(get<Is>()...);
            }(array::index_sequence{});
    }

    constexpr _iterator& operator++() {
        ++_index;
        return *this;
    }

    constexpr _iterator operator++(int) {
        auto temp = *this;
        ++_index;
        return temp;
    }

    constexpr _iterator& operator--() {
        --_index;
        return *this;
    }

    constexpr _iterator operator--(int) {
        auto temp = *this;
        --_index;
        return temp;
    }

    _iterator& operator+=(difference_type n) {
        _index += n;
        return *this;
    }

    _iterator& operator-=(difference_type n) {
        _index -= n;
        return *this;
    }

    reference operator[](difference_type n) {
        return [this, n]<std::size_t... Is>(std::index_sequence<Is...>) {
            return reference(get<Is>(n)...);
        }(array::index_sequence{});
    }

    reference operator[](difference_type n) const {
        return [this, n]<std::size_t... Is>(std::index_sequence<Is...>) {
            return reference(get<Is>(n)...);
        }(array::index_sequence{});
    }

    template <bool OtherCost>
    friend bool operator==(const _iterator& lhs,
                           const _iterator<OtherCost>& rhs) {
        return lhs._index == rhs._index;
    }

    template <bool OtherCost>
    friend bool operator!=(const _iterator& lhs,
                           const _iterator<OtherCost>& rhs) {
        return lhs._index != rhs._index;
    }

    template <bool OtherCost>
    friend auto operator<=>(const _iterator& lhs,
                            const _iterator<OtherCost>& rhs) {
        return lhs._index <=> rhs._index;
    }

    friend _iterator operator+(const _iterator& lhs, difference_type rhs) {
        return _iterator{lhs._data, lhs._index + rhs};
    }

    friend _iterator operator+(difference_type lhs, const _iterator& rhs) {
        return _iterator{rhs._data, rhs._index + lhs};
    }

    friend _iterator operator-(const _iterator& lhs, difference_type rhs) {
        return _iterator{lhs._data, lhs._index - rhs};
    }

    friend _iterator operator-(difference_type lhs, const _iterator& rhs) {
        return _iterator{rhs._data, rhs._index - rhs};
    }

    friend difference_type operator-(const _iterator& lhs,
                                     const _iterator& rhs) {
        return lhs._index - rhs._index;
    }
};

}  // namespace multi