#ifndef C2MM__MATCHERS__RANGE_MATCHER_HPP_
#define C2MM__MATCHERS__RANGE_MATCHER_HPP_

#include <ranges>

#include <catch2/matchers/catch_matchers_templated.hpp>

namespace c2mm::matchers {
template <typename T_Constraint, std::ranges::forward_range T_Range>
class Range_Matcher
}  // namespace c2mm::matchers

#endif  // C2MM__MATCHERS__RANGE_MATCHER_HPP_
