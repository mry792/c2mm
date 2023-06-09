#ifndef C2MM__MATCHERS_UTILS_HPP_
#define C2MM__MATCHERS_UTILS_HPP_

#include <type_traits>

#include <catch2/catch_tostring.hpp>
#include <catch2/matchers/catch_matchers.hpp>

namespace c2mm::matchers::utils {
/**
 * Type trait identifying whether or not @p T is a matcher.
 */
template <typename T>
struct is_matcher : public std::integral_constant<
    bool,
    std::is_base_of_v<Catch::Matchers::MatcherUntypedBase, T>
> {};

template <typename T>
inline constexpr bool is_matcher_v = is_matcher<T>::value;

/**
 * Ensure that @p value matches @p constraint.
 *
 * This is mainly intended as an implementation helper for composite matchers
 * which can take as an argument either a value for exact equality or another
 * matcher.
 *
 * If @p constraint is a matcher, then this is is equivalent to:
 * @code
 *     constraint.match(value)
 * @endcode
 * Otherwise, this is equivalent to:
 * @code
 *     value == constraint
 * @endcode
 *
 * @param[in] value The value to check against the constraint.
 * @param[in] constraint @p value should conform to this as described above.
 *
 * @return @c true if @p value conforms to the @p constraint.
 */
inline constexpr auto matches = [] (
    auto const& value,
    auto const& constraint
) -> bool {
    using Constraint = std::remove_cvref_t<decltype(constraint)>;

    if constexpr (is_matcher_v<Constraint>) {
        return constraint.match(value);
    } else {
        return value == constraint;
    }
};

/**
 * Describe the @p constraint either as a matcher or an explicit value.
 *
 * This is mainly intended as an implementation helper for composite matchers
 * which can take as an argument either a value for exact equality or another
 * matcher.
 *
 * If @p constraint is a matcher, then this is is equivalent to:
 * @code
 *     constraint.describe()
 * @endcode
 * Otherwise, this uses Catch2's stringification utilities to convert @p
 * constraint to a string.
 *
 * @param[in] constraint Either a matcher or a value.
 *
 * @return A string describing @p constraint.
 */
inline constexpr auto describe = [] (auto const& constraint) -> std::string {
    using Constraint = std::remove_cvref_t<decltype(constraint)>;

    if constexpr (is_matcher_v<Constraint>) {
        return constraint.describe();
    } else {
        return "is " + ::Catch::Detail::stringify(constraint);
    }
};
}  // namespace c2mm::matchers::utils

#endif  // C2MM__MATCHERS_UTILS_HPP_
