#ifndef C2MM__MOCK__DEFAULT_ACTION_HPP_
#define C2MM__MOCK__DEFAULT_ACTION_HPP_

#include <stdexcept>
#include <type_traits>

namespace c2mm::mock {
/**
 * The default action for any mock function call.
 *
 * @tparam T_Return The return type.
 */
template <typename T_Return>
struct Default_Action {
    /**
     * Default-construct a @p T_Return object. All parameters are ignored.
     * @return The new object.
     */
    template <typename... T_Args>
    constexpr T_Return operator () (T_Args&&...) {
        if constexpr (std::is_default_constructible_v<T_Return>) {
            return T_Return{};
        } else {
            throw std::runtime_error{
                "Return type is not default constructible. "
                "Cannot use default action."
            };
        }
    }
};

/**
 * Specialization for @c void return types.
 */
template <>
struct Default_Action<void> {
    /**
     * No-op. All parameters are ignored.
     */
    template <typename... T_Args>
    constexpr void operator () (T_Args&&...) {}
};
}  // namespace c2mm::mock

#endif  // C2MM__MOCK__DEFAULT_ACTION_HPP_
