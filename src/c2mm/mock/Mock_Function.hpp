#ifndef C2MOCK__MOCK__MOCK_FUNCTION_HPP_
#define C2MOCK__MOCK__MOCK_FUNCTION_HPP_

#include <utility>
#include <type_traits>

#include <catch2/catch_test_macros.hpp>

#include "c2mm/matchers/Tuple_Matcher.hpp"
#include "c2mm/mock/Call_Log.hpp"
#include "c2mm/mock/Default_Action.hpp"
#include "c2mm/mock/args.hpp"
#include "c2mm/mock/reporters/Fail.hpp"
#include "c2mm/mock/reporters/Fail_Check.hpp"

namespace c2mm::mock {
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
        // TODO: check expectations

        calls_.log(std::forward<T_Parameters>(args)...);

        // TODO: check call handlers

        return Default_Action<T_Return>{}();
    }

    /**
     * Check for a past call whose arguments match @p arg_constraints.
     *
     * Checks each unconsumed logged call in turn until one matches the given @c
     * arg_constraints. For each constraint that is a matcher, the comparison
     * is equivalent to `constraint.match(value)`. Otherwise, the comparison is
     * `operator ==`. All comparisons must resolve to @c true for a call to be
     * matched.
     *
     * Once a call is matched, it is consumed and cannot match another set of
     * constraints in a future validation.
     *
     * This is a lower level function that is typically not used by users of
     * this library. Prefer one of `check_called` or `require_called` bellow.
     *
     * @param[out] reporter Callable used to report a failure to find a logged
     *     call that matches the constraints.
     * @param[in] arg_constraints Constraints to check against arguments of
     *     logged calls.
     */
    template <typename T_Reporter, typename... T_Constraints>
    auto validate_called (
        T_Reporter& reporter,
        T_Constraints const&... arg_constraints
    ) {
        auto matcher = matchers::matches(bind_args(arg_constraints...));

        if (not calls_.consume_match(matcher)) {
            // TODO(emery): Print out constraints.
            reporter("No call whose arguments match.");
        }
    }

    /**
     * Check for a past call whose arguments match @p arg_constraints.
     *
     * If no call matches @p arg_constraints this is effectively a failed Catch2
     * CHECK. It fails the test but continues executing.
     *
     * @param[in] arg_constraints Constraints to check against arguments.
     */
    template <typename... T_Constraints>
    void check_called (T_Constraints const&... arg_constraints) {
        validate_called(reporters::Fail_Check{}, arg_constraints...);
    }

    /**
     * Check for a past call whose arguments match @p arg_constraints.
     *
     * If no call matches @p arg_constraints this is effectively a failed Catch2
     * REQUIRE. It fails the test stops execution.
     *
     * @param[in] arg_constraints Constraints to check against arguments.
     */
    template <typename... T_Constraints>
    void require_called (T_Constraints const&... arg_constraints) {
        validate_called(reporters::Fail{}, arg_constraints...);
    }

  private:
    Call_Log_Type calls_;
};
}  // namespace c2mm::mock

#endif  // C2MOCK__MOCK__MOCK_FUNCTION_HPP_
