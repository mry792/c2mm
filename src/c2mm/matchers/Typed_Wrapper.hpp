#ifndef C2MM__MATCHERS__TYPED_WRAPPER_HPP_
#define C2MM__MATCHERS__TYPED_WRAPPER_HPP_

#include <string>
#include <utility>

#include <catch2/matchers/catch_matchers.hpp>

#include "c2mm/matchers/utils.hpp"

namespace c2mm::matchers {
/**
 * Wrap an untyped matcher or value constraint to conform to a typed matcher
 * interface.
 *
 * This is an adaptor to facilitate using untyped matchers where a typed matcher
 * is expected. This could either be to support legacy tools or to leverage
 * run-time polymorphism.
 *
 * @tparam T_Actual Type of value to match.
 * @tparam T_Constraint The constraint type to match against.
 */
template <typename T_Actual, typename T_Constraint>
class Typed_Wrapper : public Catch::Matchers::MatcherBase<T_Actual> {
  public:
    /**
     * Wrap @p constraint in a @c Typed_Wrapper object.
     *
     * This constructor is intentionally not @c explicit.
     *
     * @param[in] constraint The constraint to wrap.
     */
    constexpr Typed_Wrapper (T_Constraint constraint)
          : constraint_{std::move(constraint)} {}

    /**
     * Construct the underlying @c constraint in-place.
     * @param (unused)
     * @param args... Arguments to forward to the constraint's constructor.
     */
    template <typename... T_Args>
    constexpr Typed_Wrapper (std::in_place_t, T_Args&&... args)
          : constraint_(std::forward<T_Args>(args)...) {}

    /**
     * Read-only accessor for the underlying constraint.
     * @return Non-modifiable reference to the constraint.
     */
    T_Constraint const& constraint () const { return constraint_; }

    /**
     * Match @p value against the underlying constraint.
     * @param[in] value The value to match.
     * @return @c true if @p value conforms to the wrapped @c constraint.
     */
    bool match (T_Actual const& value) const override {
        return utils::matches(value, constraint_);
    }

    /**
     * Describe the underlying constraint.
     * @return A human-readable description of the wrapped @c constraint.
     */
    std::string describe () const override {
        return utils::describe(constraint_);
    }

  private:
    T_Constraint constraint_;
};

/**
 * Helper function to wrap an existing @p constraint.
 *
 * @tparam T_Actual The type expected in the @c match(actual) call. This must be
 *     explicitly specified in calls to @c wrap_for.
 * @tparam T_Constraint (deduced forwarding reference)
 *
 * @param[in] constraint The constraint to wrap.
 *
 * @return The typed wrapper around @p constraint.
 */
template <typename T_Actual, typename T_Constraint>
auto wrap_for (T_Constraint&& constraint) {
    using Wrapper = Typed_Wrapper<T_Actual, std::remove_cvref_t<T_Constraint>>;
    return Wrapper{std::forward<T_Constraint>(constraint)};
}
}  // namespace c2mm::matchers

#endif  // C2MM__MATCHERS__TYPED_WRAPPER_HPP_
