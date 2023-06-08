#ifndef C2MM__MP__FOR_EACH_HPP_
#define C2MM__MP__FOR_EACH_HPP_

#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#define FWD(X) std::forward<decltype(X)>(X)

namespace c2mm::mp {
namespace for_each_impl_ {
template <
    std::size_t t_idx,
    typename T_Func,
    typename T_Value
>
constexpr void call_maybe_with_index_ (T_Func&& func, T_Value&& value) {
    static_assert(
        std::is_invocable_v<T_Func, std::size_t, decltype(value)> ||
        std::is_invocable_v<T_Func, decltype(value)>
    );

    if constexpr (std::is_invocable_v<T_Func, std::size_t, decltype(value)>) {
        std::invoke(FWD(func), t_idx, FWD(value));
    } else {
        std::invoke(FWD(func), FWD(value));
    }
}

template <
    typename T_Tuple,
    typename T_Func,
    std::size_t... t_idxs
>
constexpr void for_each_ (
    T_Tuple&& tuple,
    T_Func&& func,
    std::index_sequence<t_idxs...>
) {
    (void)(
        call_maybe_with_index_<t_idxs>(
            func,
            std::get<t_idxs>(FWD(tuple))
        ), ...
    );
}
}  // namespace for_each_impl_

/**
 * Invoke @p func for every element of @p tuple.
 *
 * Invocations are ordered by their order in @p tuple. If @p func is invocable
 * with `func(idx, std::get<idx>(tuple))`, then that overload will be called,
 * otherwise `func(std::get<idx>(tuple))` will be invoked.
 *
 * Any return value of @p func will be ignored.
 *
 * @param[in] tuple The sequence of values to "iterate" over. Must support
 *     extracting elements with @c std::get.
 * @param[in] func The callable to invoke for each element of @p tuple.
 */
template <
    typename T_Tuple,
    typename T_Func
>
constexpr void for_each (T_Tuple&& tuple, T_Func&& func) {
    constexpr int num_elements = std::tuple_size_v<std::remove_cvref_t<T_Tuple>>;

    return for_each_impl_::for_each_(
        FWD(tuple),
        FWD(func),
        std::make_index_sequence<num_elements>{}
    );
}
}  // namespace c2mm::mp

#undef FWD

#endif  // C2MM__MP__FOR_EACH_HPP_
