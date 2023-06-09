#ifndef C2MM__MATCHERS__ANY_HPP_
#define C2MM__MATCHERS__ANY_HPP_

#include <string>

#include <catch2/matchers/catch_matchers_templated.hpp>

namespace c2mm::matchers {
/**
 * Basic matcher that matches anything.
 */
class Any final : public Catch::Matchers::MatcherGenericBase {
  public:
    /**
     * Always returns @c true.
     * @param[in]
     * @return @c true regardless of the value or type of the argument.
     */
    template <typename T>
    constexpr bool match (T const&) const {
        return true;
    }

    /**
     * Provides a human-readable description of what this matcher object does.
     * @return A string describing this matcher object.
     */
    std::string describe () const override {
        return "is anything";
    }
};

static inline Any anything{};
static inline Any _{};

/**
 * Helper function for creating an @c Any matcher.
 * @return An instance of the matcher.
 */
static inline Any matches_anything () {
    return {};
}
}  // namespace c2mm::matchers

#endif  // C2MM__MATCHERS__ANY_HPP_
