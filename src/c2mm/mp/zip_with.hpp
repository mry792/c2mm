#ifndef C2MOCK__MP__ZIP_WITH_HPP_
#define C2MOCK__MP__ZIP_WITH_HPP_

#include <cstddef>
#include <tuple>
#include <utility>

namespace c2mock::mp {
namespace impl_ {
template <
    std::size_t t_idx,
    typename T_Func,
    typename... T_Arg_Sets
>
constexpr auto zip_with_call (
    T_Func func,
    T_Arg_Sets const&... arg_sets
) {
    return func(std::get<t_idx>(arg_sets)...);
}

template <
    typename T_Func,
    typename... T_Arg_Sets,
    std::size_t... t_idxs
>
constexpr auto zip_with (
    T_Func func,
    std::index_sequence<t_idxs...>,
    T_Arg_Sets const&... arg_sets
) {
    return std::tuple{zip_with_call<t_idxs>(func, arg_sets...)...};
}
}  // namespace impl_

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
    T_Func func,
    T_First_Set const& first_set,
    T_Arg_Sets const&... arg_sets
) {
    constexpr int num_elements = std::tuple_size_v<T_First_Set>;
    // static_assert(num_elements == std::tuple_size_v<T_Arg_Sets>)...;

    return impl_::zip_with(
        std::move(func),
        std::make_index_sequence<num_elements>{},
        first_set, arg_sets...
    );
}
}  // namespace c2mock::mp

#endif  // C2MOCK__MP__ZIP_WITH_HPP_
