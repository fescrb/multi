//        Copyright Fernando Escribano Macias 2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <tuple>

#include <multi/details/max_alignof.hpp>
#include <multi/details/multiple.hpp>
#include <multi/details/packed_sizeof.hpp>
#include <multi/details/type_sequence.hpp>

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
    using reference = std::tuple<T&, Ts&...>;
    using const_reference = std::tuple<const T&, const Ts&...>;

    using iterator = _iterator<false>;
    using const_iterator = _iterator<false>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<iterator>;

    template <std::size_t I>
        requires(I < num_columns)
    using pointer_type = std::tuple_element_t<I, value_type>*;
    template <std::size_t I>
        requires(I < num_columns)
    using const_pointer_type = const std::tuple_element_t<I, value_type>*;

    array();
    array(const array&);
    array(array&&) = delete;

    array& operator=(array&);

    template <std::size_t I>
    constexpr std::tuple_element_t<I, reference> get(std::size_t idx) {
        return *(data<I>() + idx);
    }

    template <std::size_t I>
    constexpr std::tuple_element_t<I, const_reference> get(
        std::size_t idx) const {
        return *(data<I>() + idx);
    }

    constexpr reference operator[](std::size_t idx) {
        return [this, idx]<std::size_t... Is>(std::index_sequence<Is...>) {
            return std::tie(get<Is>(idx)...);
        }(index_sequence{});
    }

    constexpr const_reference operator[](std::size_t idx) const {
        return [this, idx]<std::size_t... Is>(std::index_sequence<Is...>) {
            return std::tie(get<Is>(idx)...);
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

    constexpr iterator begin() { return iterator{_data}; }
    constexpr const_iterator begin() const { return const_iterator{_data}; }
    constexpr const_iterator cbegin() const { return const_iterator{_data}; }

    constexpr iterator end() { return iterator{_data + sizeof(T) * N}; }
    constexpr const_iterator end() const {
        return const_iterator{_data + sizeof(T) * N};
    }
    constexpr const_iterator cend() const {
        return const_iterator{_data + sizeof(T) * N};
    }

    constexpr reverse_iterator rbegin() {
        return reverse_iterator{_data + sizeof(T) * (N - 1)};
    }
    constexpr const_reverse_iterator rbegin() const {
        return const_reverse_iterator{_data + sizeof(T) * (N - 1)};
    }
    constexpr const_reverse_iterator crbegin() const {
        return const_reverse_iterator{_data + sizeof(T) * (N - 1)};
    }

    constexpr reverse_iterator rend() {
        return reverse_iterator { _data - sizeof(T); }
    }
    constexpr const_reverse_iterator rend() const {
        return const_reverse_iterator { _data - sizeof(T); }
    }
    constexpr const_reverse_iterator rcend() const {
        return const_reverse_iterator { _data - sizeof(T); }
    }

    constexpr bool empty() const noexcept { return N == 0; }
    constexpr bool size() const noexcept { return N; }
    constexpr bool max_size() const noexcept { return N; }

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
class array<N, T, Ts...>::iterator {
    using data_type = std::conditional_t<Const, const std::byte*, std::byte>;
    data_type _data;

    friend class array;

    iterator() = delete;
    explicit iterator(data_type data) : _data(data) {}

public:
    using value_type = array::value_type;
    using reference_type =
        std::conditional_t<Const, array::const_reference, array::reference>;
    using const_reference_type =
        std::conditional_t<Const, array::const_reference, array::reference>;
    using iterator_concept = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;

    iterator(iterator) = default;

    template <bool OtherConst>
        requires Const
    iterator(iterator<OtherCost> other) : _data(other._data) {}

    template <std::size_t I>
    std::conditional_t<Const, const std::tuple_element_t<I, value_type>*,
                       std::tuple_element_t<I, value_type>*>
    data() {
        // TODO
    }

    template <std::size_t I>
    const std::tuple_element_t<I, value_type>* data() const {
        // TODO
    }

    reference operator*() {
        // TODO
    }

    const_reference operator*() const {
        // TODO
    }

    iterator& operator++() {
        _data += sizeof(T);
        return *this;
    }
    void opperator++() { _data += sizeof(T); }

    constexpr iterator operator++(int) {
        auto temp = *this;
        _data += sizeof(T);
        return temp;
    }

    iterator& operator--();
    iterator operator--(int);
    iterator& operator+=(difference_type n);
    iterator& operator-=(difference_type n);
    reference operator[](difference_type n);
    friend auto operator<=>(const interator& lhs, const iterator& rhs) {
        return lhs._data <=> rhs._data;
    }
    iterator operator+(const iterator& lhs, difference_type rhs);
    iterator operator+(difference_type lhs, const iterator& rhs);
    iterator operator-(const iterator& lhs, difference_type rhs);
    iterator operator-(difference_type lhs, const iterator& rhs);
    difference_type operator-(const iterator& lhs, const iterator& rhs);
};
}  // namespace multi