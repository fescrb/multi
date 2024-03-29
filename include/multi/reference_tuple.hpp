//        Copyright Fernando Escribano Macias 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <tuple>

#include <multi/details/head.hpp>
#include <multi/details/prepend.hpp>
#include <multi/details/size.hpp>
#include <multi/details/tail.hpp>
#include <multi/get.hpp>
#include <multi/tuple_element.hpp>

namespace multi {

template <class... Ts>
    requires(sizeof...(Ts) > 0)
class reference_tuple {
    using pointer = std::tuple<Ts*...>;
    static constexpr std::size_t num_elements = sizeof...(Ts);
    using index_sequence = std::make_index_sequence<num_elements>;
    pointer _refs;

    template <class T>
    static T& identity(T& t) {
        return t;
    }

    template <class T>
    static T& identity(T&& t) = delete;

    template <class T, size_t... Is>
        requires requires(T t) {
            pointer{std::addressof(identity(multi::get<Is>(t)))...};
        }
    static pointer as_pointer(T&& t, std::index_sequence<Is...>) {
        return pointer{std::addressof(identity(multi::get<Is>(t)))...};
    }

public:
    using value_type = std::tuple<Ts...>;
    using reference = std::tuple<Ts&...>;

    template <std::size_t I>
    std::tuple_element_t<I, reference> get() const {
        return *std::get<I>(this->_refs);
    }

    template <class... Us>
        requires(sizeof...(Us) == num_elements) && requires(Us... us) {
            pointer{std::addressof(identity(std::forward<Us>(us)))...};
        }
    explicit reference_tuple(Us&&... refs)
        : _refs{std::addressof(identity(std::forward<Us>(refs)))...} {}

    template <class... Us>
        requires(sizeof...(Us) == num_elements) &&
                requires(const std::tuple<Us...>& t) {
                    as_pointer(t, index_sequence{});
                }
    reference_tuple(const std::tuple<Us...>& refs)
        : _refs{as_pointer(refs, index_sequence{})} {}

    template <class... Us>
        requires(sizeof...(Us) == num_elements) &&
                requires(std::tuple<Us...>& t) {
                    as_pointer(t, index_sequence{});
                }
    reference_tuple(std::tuple<Us...>& refs)
        : _refs{as_pointer(refs, index_sequence{})} {}

    template <class... Us>
        requires(sizeof...(Us) == num_elements) &&
                requires(std::tuple<Us&...>& t) {
                    as_pointer(t, index_sequence{});
                }
    reference_tuple(const reference_tuple<Us...>& rt)
        : reference_tuple(rt.tie()) {}

    template <class... Us>
        requires(sizeof...(Us) == num_elements) &&
                requires(std::tuple<Us&...>& t) {
                    as_pointer(t, index_sequence{});
                }
    reference_tuple(reference_tuple<Us...>&& rt) : reference_tuple(rt.tie()) {}

    template <class... Us>
    friend auto tail(const reference_tuple& rhs) {
        return [&rhs]<std::size_t I, std::size_t... Is>(
                   std::index_sequence<I, Is...>) {
            return reference_tuple<std::tuple_element_t<Is, value_type>...>{
                rhs.get<Is>()...};
        }(index_sequence{});
    }

    reference tie() const {
        return [this]<std::size_t... Is>(std::index_sequence<Is...>) {
            return reference{get<Is>()...};
        }(index_sequence{});
    }

    operator reference() const { return tie(); }

    value_type tuple() const {
        return [this]<std::size_t... Is>(std::index_sequence<Is...>) {
            return value_type{get<Is>()...};
        }(index_sequence{});
    }

    explicit operator value_type() const { return tuple(); }

    template <class... Us>
        requires std::assignable_from<reference&, std::tuple<Us...>>
    reference_tuple& operator=(const std::tuple<Us...>& val) {
        tie() = val;
        return *this;
    }

    template <class... Us>
        requires std::assignable_from<reference&, std::tuple<Us&...>>
    reference_tuple& operator=(const reference_tuple<Us...>& val) {
        tie() = val.tie();
        return *this;
    }

    template <class... Us>
        requires std::assignable_from<reference&, std::tuple<Us...>>
    const reference_tuple& operator=(const std::tuple<Us...>& val) const {
        tie() = val;
        return *this;
    }

    template <class... Us>
        requires std::assignable_from<reference&, std::tuple<Us&...>>
    const reference_tuple& operator=(const reference_tuple<Us...>& val) const {
        tie() = val.tie();
        return *this;
    }

    reference_tuple& operator=(const reference_tuple& val) {
        tie() = val.tie();
        return *this;
    }

    const reference_tuple& operator=(const reference_tuple& val) const {
        tie() = val.tie();
        return *this;
    }

    template <class... Us>
    bool operator==(const reference_tuple<Us...>& rhs) const {
        return tie() == rhs.tie();
    }

    template <class... Us>
    auto operator<=>(const reference_tuple<Us...>& rhs) const {
        return tie() <=> rhs.tie();
    }
    template <class... Us>
    bool operator==(const std::tuple<Us...>& rhs) const {
        return tie() == rhs;
    }

    template <class... Us>
    auto operator<=>(const std::tuple<Us...>& rhs) const {
        return tie() <=> rhs;
    }
};

template <class... Ts>
reference_tuple(Ts&...) -> reference_tuple<Ts...>;

template <std::size_t I, class... Ts>
decltype(auto) get(const reference_tuple<Ts...>& t) noexcept {
    return t.template get<I>();
}

template <std::size_t I, class... Ts>
struct tuple_element<I, reference_tuple<Ts...>>
    : std::tuple_element<I, std::tuple<Ts&...>> {};

template <class T>
struct is_reference_tuple : public std::false_type {};

template <class... Ts>
struct is_reference_tuple<reference_tuple<Ts...>> : public std::true_type {};

template <class T>
constexpr bool is_reference_tuple_v = is_reference_tuple<T>::value;

namespace details {

template <class T, class... Ts>
struct head<reference_tuple<T, Ts...>> {
    using type = T;
};

template <class T, class... Ts>
struct tail<reference_tuple<T, Ts...>> {
    using type = reference_tuple<Ts...>;
};

template <class T, class... Us>
struct prepend<T, reference_tuple<Us...>> {
    using type = reference_tuple<T, Us...>;
};

template <class... Ts>
struct size<reference_tuple<Ts...>> {
    static constexpr size_t value = sizeof...(Ts);
};

template <class T>
struct is_tuple : std::false_type {};

template <class... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type {};

template <class T>
constexpr bool is_tuple_v = is_tuple<T>::value;

}  // namespace details

template <class R, class T, class RQual, class TQual>
concept reference_tuple_common_reference_exists_with =
    is_reference_tuple_v<R>;  // &&
/*requires {
    typename std::common_reference_t<typename R::reference, TQual>;
} &&
std::is_convertible_v<
    RQual, std::common_reference_t<typename R::reference, TQual>>;*/

// TODO define tuple_size and tail for reference tuple

}  // namespace multi

namespace std {
// template <class T, class R, template <class> class TQual,
//           template <class> class RQual>
//// requires T & U are tuple-like and at least one is ref tuple
//// See https://en.cppreference.com/w/cpp/types/common_reference
//// "at least one of them depends on a program-defined type."
//// also requires std::is_same_v<T, std::decay_t<T>> and std::is_same_v<U,
//// std::decay_t<U>>
// struct basic_common_reference {
//     using type =
//         std::tuple < common_reference_t<TQual<TTypes>..., UQual<UTypes>>;
// };

// implementation as per reference_wrapper
// https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper/basic_common_reference

template <class R, class T, template <class> class RQual,
          template <class> class TQual>
    requires(
        multi::is_reference_tuple_v<R> && multi::details::is_tuple_v<T> &&
        !multi::is_reference_tuple_v<T> && multi::details::size_v<R> == 1 &&
        multi::details::size_v<T> == 1
        //! multi::is_reference_tuple<T> && multi::is_tuple<T>
        // multi::reference_tuple_common_reference_exists_with<R, T, RQual<R>,
        //                                                     TQual<T>>  //&&
        //! multi::reference_tuple_common_reference_exists_with<T, R, TQual<T>,
        // RQual<R>>
        )
struct basic_common_reference<R, T, RQual, TQual> {
    using type = multi::reference_tuple<std::remove_reference_t<
        std::common_reference_t<multi::details::head_t<typename R::reference>,
                                TQual<multi::details::head_t<T>>>>>;
};

template <class R, class T, template <class> class RQual,
          template <class> class TQual>
    requires(multi::is_reference_tuple_v<R> && multi::details::is_tuple_v<T> &&
             !multi::is_reference_tuple_v<T> && multi::details::size_v<R> > 1 &&
             multi::details::size_v<T> == multi::details::size_v<R>)
struct basic_common_reference<R, T, RQual, TQual> {
    using type = multi::details::prepend_t<
        std::remove_reference_t<std::common_reference_t<
            multi::details::head_t<typename R::reference>,
            TQual<multi::details::head_t<T>>>>,
        std::common_reference_t<multi::details::tail_t<R>,
                                TQual<multi::details::tail_t<T>>>>;
};

template <class T, class R, template <class> class TQual,
          template <class> class RQual>
    requires(multi::is_reference_tuple_v<R> && multi::details::is_tuple_v<T> &&
             !multi::is_reference_tuple_v<T>)
struct basic_common_reference<T, R, TQual, RQual> {
    using type = std::common_reference_t<RQual<R>, TQual<T>>;
};

template <class R, class T, template <class> class RQual,
          template <class> class TQual>
    requires(multi::is_reference_tuple_v<R> && multi::is_reference_tuple_v<T> &&
             multi::details::size_v<R> == 1 && multi::details::size_v<T> == 1)
struct basic_common_reference<R, T, RQual, TQual> {
    using type =
        multi::reference_tuple<std::remove_reference_t<std::common_reference_t<
            multi::details::head_t<typename R::reference>,
            multi::details::head_t<typename T::reference>>>>;
};

template <class R, class T, template <class> class RQual,
          template <class> class TQual>
    requires(multi::is_reference_tuple_v<R> && multi::is_reference_tuple_v<T> &&
             multi::details::size_v<R> > 1 &&
             multi::details::size_v<T> == multi::details::size_v<R>)
struct basic_common_reference<R, T, RQual, TQual> {
    using type = multi::details::prepend_t<
        std::remove_reference_t<std::common_reference_t<
            multi::details::head_t<typename R::reference>,
            multi::details::head_t<typename T::reference>>>,
        std::common_reference_t<multi::details::tail_t<R>,
                                multi::details::tail_t<T>>>;
};

/*
template <class T, class R, template <class> class TQual,
          template <class> class RQual>
    requires(multi::reference_tuple_common_reference_exists_with<R, T, RQual<R>,
                                                                 TQual<T>>)
struct basic_common_reference<T, R, TQual, RQual>
    : basic_common_reference<R, T, RQual, TQual> {
    using type = std::common_reference_t<TQual<T>, typename R::reference>;
};*/

// Maybe the easier solution is
// struct basic_common_reference {
//     using type = std::tuple<<TQual<T::reference>..., U>;
// };
// If T is a reference type and vice-versa

}  // namespace std