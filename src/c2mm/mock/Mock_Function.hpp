#ifndef C2MOCK__MOCK__MOCK_FUNCTION_HPP_
#define C2MOCK__MOCK__MOCK_FUNCTION_HPP_

#include <utility>
#include <type_traits>

#include <catch2/catch_test_macros.hpp>

#include "c2mm/matchers/Tuple_Matcher.hpp"
#include "c2mm/mock/Call_Log.hpp"
#include "c2mm/mock/args.hpp"

namespace c2mock::mock {
/**
 * Primary template for @c Mock_Function is intentionally not defined.
 *
 * See specializations for full documentation.
 */
template <typename T_Signature>
class Mock_Function;

/**
 * A @c Mock_Function to help test interface boundaries.
 *
 * Instances are callable and record the arguments for each call. Before the
 * object is destroyed, every logged call must be consumed by either @c
 * check_call() or @c require_call(). See the documentation for those member
 * functions below.
 *
 * @tparam T_Return The return type of the function. (Currently only supports @c
 *     void.)
 * @tparam T_Parameters Types of the mocked function's parameters.
 */
template <typename T_Return, typename... T_Parameters>
class Mock_Function<T_Return(T_Parameters...)> {
    template <typename T>
    using MatcherBase = Catch::Matchers::MatcherBase<T>;

  public:
    using Signature = T_Return(T_Parameters...);
    using Call_Log_Type = Call_Log<Captured_Args<T_Parameters...>>;

    /**
     * When a @c Mock_Function is destroyed, it fails the test if there are any
     * unconsumed calls.
     */
    ~Mock_Function () {
        calls_.check_no_calls();
    }

    /**
     * Read-only accessor for yet-unmatched calls.
     * @return Constant reference to the collection of call metadata.
     */
    typename Call_Log_Type::Call_List const& calls () const {
        return calls_.calls();
    }

    /**
     * "Call" the mock function.
     *
     * This logs the call to be checked later. If not consumed by a @c
     * check_called or a @c require_called before the @c Mock_Function object
     * is destroyed, the current Catch2 test will fail.
     *
     * @param[in] args The arguments of the call.
     *
     * @return @c void
     */
    T_Return operator () (T_Parameters... args) {
        static_assert(
            std::is_same_v<T_Return, void>,
            "This function only supports void-return types for now."
        );

        calls_.log(static_cast<T_Parameters&&>(args)...);
    }

    /**
     * Check for a past call whose arguments match @p arg_constraints.
     *
     * Checks each unconsumed logged call in turn until one matches the given @p
     * arg_constraints. For each constraint that is a matcher, the comparison
     * is equivalent to `constraint.match(value)`. Otherwise, the comparison is
     * `operator ==`. All comparisons must resolve to @c true for a call to be
     * matched.
     *
     * Once a call is matched, it is consumed and cannot match another set of
     * constraints in a future call.
     *
     * If no call matches @p arg_constraints this is effectively a failed Catch2
     * CHECK. It fails the test but continues executing.
     *
     * @param[in] arg_constraints Constraints to check against arguments.
     */
    template <typename... T_Constraints>
    void check_called (T_Constraints const&... arg_constraints) {
        static_assert(
            std::is_same_v<T_Return, void>,
            "This function is only available for void-return mock functions."
        );

        auto matcher = matchers::matches(bind_args(arg_constraints...));

        if (not calls_.consume_match(matcher)) {
            // TODO(emery): Print out matchers.
            FAIL_CHECK("No call whose arguments match.");
        }
    }

    /**
     * Check for a past call whose arguments match @p arg_constraints.
     *
     * Checks each unconsumed logged call in turn until one matches the given @p
     * arg_constraints. For each constraint that is a matcher, the comparison
     * is equivalent to `constraint.match(value)`. Otherwise, the comparison is
     * `operator ==`. All comparisons must resolve to @c true for a call to be
     * matched.
     *
     * Once a call is matched, it is consumed and cannot match another set of
     * constraints in a future call.
     *
     * If no call matches @p arg_constraints this is effectively a failed Catch2
     * REQUIRE. It fails the test stops execution.
     *
     * @param[in] arg_constraints Constraints to check against arguments.
     */
    template <typename... T_Constraints>
    void require_called (T_Constraints const&... arg_constraints) {
        static_assert(
            std::is_same_v<T_Return, void>,
            "This function is only available for void-return mock functions."
        );

        auto matcher = matchers::matches(bind_args(arg_constraints...));

        if (not calls_.consume_match(matcher)) {
            // TODO(emery): Print out matchers.
            FAIL("No call whose arguments match.");
        }
    }

  private:
    Call_Log_Type calls_;
};
}  // namespace c2mock::mock

#endif  // C2MOCK__MOCK__MOCK_FUNCTION_HPP_
