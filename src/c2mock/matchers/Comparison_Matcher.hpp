#ifndef C2MOCK__MATCHERS__COMPARISON_MATCHER_HPP_
#define C2MOCK__MATCHERS__COMPARISON_MATCHER_HPP_

#include <functional>
#include <string>
#include <utility>

#include <catch2/catch_tostring.hpp>
#include <catch2/matchers/catch_matchers_templated.hpp>

namespace c2mock::matchers {
/**
 * Matcher comparing to an expected value using a binary predicate.
 *
 * This is similar to @c Catch::Matchers::PredicateMatcher except it helps
 * generate the description based on the expected value.
 *
 * @tparam T_Expected The type of the expected value.
 * @tparam T_Binary_Pred A binary predicate callable. Must support the following
 *     signature: `predicate(value, expected)` where @c value is the variable
 *     passed to `match(value)`.
 */
template <typename T_Expected, typename T_Binary_Pred>
class Comparison_Matcher final : public Catch::Matchers::MatcherGenericBase {
  public:
    /**
     * Construct from required components.
     * @param[in] expected The value expected by the matcher.
     * @param[in] pred_description Description of the predicate.
     * @param[in] predicate A callable to compare a value against @p expected.
     */
    template <typename T>
    Comparison_Matcher (
        T&& expected,
        std::string pred_description,
        T_Binary_Pred predicate = T_Binary_Pred{}
    ) : expected_{std::forward<T>(expected)},
        pred_description_{std::move(pred_description)},
        predicate_{std::move(predicate)}
    {}

    /**
     * Execute the predicate to compare @p value against the stored @c expected.
     * @param[in] value The value to compare.
     * @return The result of the predicate.
     */
    template <typename T>
    bool match (T const& value) const {
        return predicate_(value, expected_);
    }

    /**
     * Describe the matcher.
     * @return The combined description of the @c predicate and @c expected
     *     value.
     */
    std::string describe () const override {
        return pred_description_ + ::Catch::Detail::stringify(expected_);
    }

  private:
    T_Expected expected_;
    std::string pred_description_;
    T_Binary_Pred predicate_;
};

/**
 * Create a matcher to check for exact equality with @p expected.
 * @param[in] expected The expected value. Used as the second argument in the
 *     comparison.
 * @return A matcher to check if values are equal to @p expected.
 */
template <typename T>
Comparison_Matcher<std::remove_cvref_t<T>, std::equal_to<>>
equal_to (T&& expected) {
    return {std::forward<T>(expected), "== "};
}

/**
 * Create a matcher to check for exact inequality with @p expected.
 * @param[in] expected The expected value. Used as the second argument in the
 *     comparison.
 * @return A matcher to check if values are not equal to @p expected.
 */
template <typename T>
Comparison_Matcher<std::remove_cvref_t<T>, std::not_equal_to<>>
not_equal_to (T&& expected) {
    return {std::forward<T>(expected), "!= "};
}

/**
 * Create a matcher to check that values are greater than @p expected.
 * @param[in] expected The expected value. Used as the second argument in the
 *     comparison.
 * @return A matcher to check if values are greater than @p expected.
 */
template <typename T>
Comparison_Matcher<std::remove_cvref_t<T>, std::greater<>>
greater_than (T&& expected) {
    return {std::forward<T>(expected), "> "};
}

/**
 * Create a matcher to check that values are less than @p expected.
 * @param[in] expected The expected value. Used as the second argument in the
 *     comparison.
 * @return A matcher to check if values are less than @p expected.
 */
template <typename T>
Comparison_Matcher<std::remove_cvref_t<T>, std::less<>>
less_than (T&& expected) {
    return {std::forward<T>(expected), "< "};
}

/**
 * Create a matcher to check that values are greater than or equal to @p
 * expected.
 * @param[in] expected The expected value. Used as the second argument in the
 *     comparison.
 * @return A matcher to check if values are greater than or equal to @p
 *     expected.
 */
template <typename T>
Comparison_Matcher<std::remove_cvref_t<T>, std::greater_equal<>>
greater_or_equal_to (T&& expected) {
    return {std::forward<T>(expected), ">= "};
}

/**
 * Create a matcher to check that values are less than or equal to @p expected.
 * @param[in] expected The expected value. Used as the second argument in the
 *     comparison.
 * @return A matcher to check if values are less than or equal to @p expected.
 */
template <typename T>
Comparison_Matcher<std::remove_cvref_t<T>, std::less_equal<>>
less_or_equal_to (T&& expected) {
    return {std::forward<T>(expected), "<= "};
}
}  // namespace c2mock::matchers

#endif  // C2MOCK__MATCHERS__COMPARISON_MATCHER_HPP_
