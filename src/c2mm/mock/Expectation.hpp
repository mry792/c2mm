#ifndef C2MM__MOCK__EXPECTATION_HPP_
#define C2MM__MOCK__EXPECTATION_HPP_

#include <cstddef>
#include <memory>
#include <utility>

#include <catch2/matchers/catch_matchers.hpp>

#include "c2mm/mock/Default_Action.hpp"
#include "c2mm/mock/args.hpp"

namespace c2mm::mock {
/**
 * Primary template for @c Expectation is intentionally not defined.
 *
 * See specializations for full documentation.
 */
template <typename T_Signature>
class Expectation;

/**
 * Wraps and controls access to a particular action a mock function can execute
 * in response to a call.
 */
template <typename T_Return, typename... T_Parameters>
class Expectation<T_Return(T_Parameters...)> {
  public:
    using Args_Tuple = Bound_Args<T_Parameters...>;
    using Matcher = Catch::Matchers::MatcherBase<Args_Tuple>;

    /**
     * Construct from required components.
     *
     * @param[in] matcher Tuple matcher indicating whether this expectation can
     *     accept a call based on it's arguments.
     */
    Expectation (std::unique_ptr<Matcher> matcher)
          : matcher_{std::move(matcher)} {}

    /**
     * Read-only accessor to the internal matcher.
     * @return Constant reference to the @c matcher field.
     */
    Matcher const& matcher () const {
        return *matcher_;
    }

    /**
     * Indicates whether this expectation can consume the call identified by the
     * specified arguments.
     *
     * @param[in] args Tuple of references to the arguments of the call.
     *
     * @return @c true if this expectation has not yet handled any calls and if
     *     the matcher specified at construction matches @p args.
     */
    bool can_consume (Args_Tuple const& args) const {
        // TODO: retirement

        // TODO: more complex cardinalities
        if (call_count_ > 0) {
            return false;
        }

        // TODO: cross-argument matcher
        return matcher_->match(args);
    }

    /**
     * Handle the call by delegating to the default action.
     *
     * @todo Handle the call by delegating to the action specified at
     *     construction.
     *
     * @param[in] args... Arguments of the call being handled, forwarded to the
     *     internal action.
     *
     * @return The result of the action.
     */
    T_Return handle_call (T_Parameters&&... /* args */) {
        ++call_count_;

        // TODO: execute custom action
        return Default_Action<T_Return>{}();
    }

  private:
    std::unique_ptr<Matcher> matcher_;

    std::size_t call_count_ = 0;
};
}  // namespace c2mm::mock

#endif  // C2MM__MOCK__EXPECTATION_HPP_
