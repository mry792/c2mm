#ifndef C2MOCK__MOCK__ARGS_HPP_
#define C2MOCK__MOCK__ARGS_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

namespace c2mock::mock {
/**
 * Owning representation of the arguments of a function call.
 *
 * This type stores all elements by value (it owns all the arguments it stores),
 * although some elements may themselves be non-owning indirections, e.g. raw
 * pointer or @c std::reference_wrapper.
 */
template <typename... T_Args>
using Captured_Args = std::tuple<std::remove_cvref_t<T_Args>...>;

/**
 * Capture @p args by value.
 * @param[in] args... The arguments to capture.
 * @return An owning tuple of values initialized from @p args.
 */
template <typename... T_Args>
Captured_Args<T_Args...> capture_args (T_Args&&... args) {
    return {std::forward<T_Args>(args)...};
}

/**
 * Non-owning representation of the arguments of a function call.
 *
 * This type stores all elements as constant lvalue references. Use this type
 * ideally in only transient contexts where the arguments will definitely
 * outlive the @c Bound_Args object.
 */
template <typename... T_Args>
using Bound_Args = std::tuple<std::remove_cvref_t<T_Args> const&...>;

/**
 * Bind @p args by const l-value reference.
 * @param[in] args... The arguments to bind.
 * @return A non-owning tuple of references bound to @p args.
 */
template <typename... T_Args>
Bound_Args<T_Args...> bind_args (T_Args&&... args) {
    return {args...};
}
}  // namespace c2mock::mock

#endif  // C2MOCK__MOCK__ARGS_HPP_
