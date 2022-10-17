#ifndef C2MOCK__MATCHERS__TUPLE_MATCHER_HPP_
#define C2MOCK__MATCHERS__TUPLE_MATCHER_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include <catch2/matchers/catch_matchers_templated.hpp>

#include "c2mock/matchers/utils.hpp"
#include "c2mock/mp/all.hpp"
#include "c2mock/mp/zip_with.hpp"

namespace c2mock::matchers {
/**
 * A composite matcher for @c std::tuple.
 *
 * Constructed with a tuple of constraints. Each element could either be another
 * matcher or a value and are matched element-wise against the given @c
 * std::tuple of values. For each constraint that is a value, `operator ==` is
 * used for the comparison. All comparisons must be true in order for the @c
 * std::tuple to match.
 *
 * @tparam T_Constraints Types of the constraints.
 */
template <typename... T_Constraints>
class Tuple_Matcher final : Catch::Matchers::MatcherGenericBase {
  public:
    /**
     * Construct from a @c std::tuple of @p constraints.
     * @param[in] constraints The constraints to match against.
     */
    explicit Tuple_Matcher (std::tuple<T_Constraints...> constraints)
          : constraints_{std::move(constraints)} {}

    /**
     * Compares elements of @p values to corresponding constraints.
     *
     * For each constraint that is a matcher, the comparison is equivalent to
     * `constraint.match(value)`. Otherwise, the comparison is `operator ==`.
     * All comparisons must resolve to @c true for this @c match function to
     * be @c true.
     *
     * @param[in] values The values to check for a match.
     *
     * @return @c true if all constraints match their corresponding values.
     */
    template <typename T_Tuple>
    bool match (T_Tuple const& values) const {
        using mp::all;
        using mp::zip_with;

        return all(zip_with(utils::matches, values, constraints_));
    }

    /**
     * Provides a human-oriented description of what this matcher object does.
     * @return A string describing this matcher object.
     */
    std::string describe () const override {
        // TODO
        return "";
    }

  private:
    std::tuple<T_Constraints...> constraints_;
};

/**
 * Helper function for creating a @c Tuple_Matcher.
 * @param[in] constraints A @c std::tuple of constraints. The constructed @c
 *     Tuple_Matcher will have exactly the same types including reference-ness.
 */
template <typename... T_Constraints>
Tuple_Matcher<T_Constraints...> matches (
    std::tuple<T_Constraints...> constraints
) {
    return Tuple_Matcher<T_Constraints...>{std::move(constraints)};
}
}  // namespace c2mock::matchers

#endif  // C2MOCK__MATCHERS__TUPLE_MATCHER_HPP_
