#ifndef C2MOCK__MP__ALL_HPP_
#define C2MOCK__MP__ALL_HPP_

#include <cstddef>
#include <tuple>

namespace c2mock::mp {
namespace impl_ {
template <typename... Ts, std::size_t... t_idxs>
constexpr bool all (
    std::tuple<Ts...> const& values,
    std::integer_sequence<std::size_t, t_idxs...>
) {
    return (std::get<t_idxs>(values) and ...);
}
}  // namespace impl_

/**
 * Unpacks @p values into a logical conjunction fold expression.
 * @param[in] values A @c std::tuple of boolean-like values.
 * @return @c true if every element of @p values is @c true.
 */
template <typename T>
constexpr bool all (T const& values) {
    return impl_::all(
        values,
        std::make_index_sequence<std::tuple_size_v<T>>{}
    );
}
}  // namespace c2mock::mp

#endif  // C2MOCK__MP__ALL_HPP_
