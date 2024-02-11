//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <compare>

namespace multi {

namespace details {

struct index {
    using difference_type = std::ptrdiff_t;

    constexpr index() = default;
    constexpr index(const index&) = default;
    constexpr index(index&&) = default;

    constexpr auto operator<=>(const index& other) const
        -> std::strong_ordering {
        return _index <=> other._index;
    }
    constexpr auto operator==(const index& other) const -> bool = default;
    constexpr auto operator!=(const index& other) const -> bool = default;

protected:
    constexpr auto operator=(const index&) -> index& = default;
    constexpr auto operator=(index&&) -> index& = default;

    constexpr index(const difference_type& idx) : _index(idx) {}
    difference_type _index;
};

}  // namespace details

}  // namespace multi

namespace std {

// Specialise basic common reference to enable three-way comparison between
// multi::details::index iterators for the same containers
template <class T, class U, template <class> class TQual,
          template <class> class UQual>
    requires std::derived_from<T, multi::details::index> &&
             std::derived_from<U, multi::details::index> &&
             std::is_same_v<typename T::container_type,
                            typename U::container_type>
struct basic_common_reference<T, U, TQual, UQual> {
    using type = multi::details::index;
};

}  // namespace std