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

    constexpr iterator end();
    constexpr const_iterator end() const;
    constexpr const_iterator cend() const;

    constexpr reverse_iterator rbegin();
    constexpr const_reverse_iterator rbegin() const;
    constexpr const_reverse_iterator crbegin() const;

    constexpr reverse_iterator rend();
    constexpr const_reverse_iterator rend() const;
    constexpr const_reverse_iterator rcend() const;

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

}  // namespace multi