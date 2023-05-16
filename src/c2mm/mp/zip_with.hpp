#ifndef C2MOCK__MP__ZIP_WITH_HPP_
#define C2MOCK__MP__ZIP_WITH_HPP_

#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#define FWD(X) std::forward<decltype(X)>(X)

namespace c2mm::mp {
namespace zip_with_impl_ {
template <
    std::size_t t_idx,
    typename T_Func,
    typename... T_Arg_Sets
>
constexpr auto zip_with_call_ (
    T_Func&& func,
    T_Arg_Sets&&... arg_sets
) {
    return std::invoke(FWD(func), std::get<t_idx>(FWD(arg_sets))...);
}

template <
    typename T_Func,
    typename... T_Arg_Sets,
    std::size_t... t_idxs
>
constexpr auto zip_with_ (
    T_Func&& func,
    std::index_sequence<t_idxs...>,
    T_Arg_Sets&&... arg_sets
) {
    if constexpr (
        std::is_same_v<
            void,
            std::invoke_result_t<
                T_Func,
                decltype(std::get<0ul>(arg_sets))...
            >
        >
    ) {
        (void)(
            zip_with_call_<t_idxs>(
                FWD(func),
                FWD(arg_sets)...
            ), ...
        );
    } else {
        return std::tuple{
            zip_with_call_<t_idxs>(
                FWD(func),
                FWD(arg_sets)...
            )...
        };
    }
}

template <typename T, T... t_values>
constexpr auto to_tuple (std::integer_sequence<T, t_values...>) {
    return std::tuple{t_values...};
}
}  // namespace zip_with_impl_

/**
 * Zip one or more @c std::tuples and transform each group with @p func.
 *
 * The call `zip_with(func, std::tuple{a0, a1, ...}, std::tuple
 * {b0, b1, ...}, ...)` is equivalent to `std::tuple{func(a0, b0, ...), func
 * (a1, b1, ...), ...}`.
 *
 * @param[in] func The function to call each time with the ith element of each
 *     of the argument sets.
 * @param[in] first_set The first set of function arguments.
 * @param[in] arg_sets The remaining sets of function arguments.
 *
 * @@return A @c std::tuple of the results of the invocations.
 */
template <typename T_Func, typename T_First_Set, typename... T_Arg_Sets>
constexpr auto zip_with (
    T_Func&& func,
    T_First_Set&& first_set,
    T_Arg_Sets&&... arg_sets
) {
    constexpr int num_elements =
        std::tuple_size_v<std::remove_cvref_t<T_First_Set>>;
    // static_assert(num_elements == std::tuple_size_v<T_Arg_Sets>)...;

    return zip_with_impl_::zip_with_(
        FWD(func),
        std::make_index_sequence<num_elements>{},
        FWD(first_set), FWD(arg_sets)...
    );
}

/**
 * Zip indices and one or more @c std::tuples and transform each group with @p
 * func.
 *
 * Other than the addition of the index at the start of the argument list, this
 * is identical to @c zip_with above.
 */
template <typename T_Func, typename T_First_Set, typename... T_Arg_Sets>
constexpr auto zip_with_idx (
    T_Func&& func,
    T_First_Set&& first_set,
    T_Arg_Sets&&... arg_sets
) {
    constexpr int num_elements =
        std::tuple_size_v<std::remove_cvref_t<T_First_Set>>;
    // static_assert(num_elements == std::tuple_size_v<T_Arg_Sets>)...;

    constexpr std::make_index_sequence<num_elements> idx_seq{};

    return zip_with_impl_::zip_with_(
        FWD(func),
        idx_seq,
        zip_with_impl_::to_tuple(idx_seq),
        FWD(first_set), FWD(arg_sets)...
    );
}
}  // namespace c2mm::mp

#undef FWD

#endif  // C2MOCK__MP__ZIP_WITH_HPP_
