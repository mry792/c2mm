#include "c2mm/matchers/utils.hpp"

#include <catch2/catch_test_macros.hpp>

#include "c2mm/matchers/Comparison_Matcher.hpp"

TEST_CASE ("utils::is_matcher") {
    using c2mm::matchers::equal_to;
    using c2mm::matchers::utils::is_matcher_v;

    CHECK(not is_matcher_v<bool>);
    CHECK(not is_matcher_v<double>);
    CHECK(is_matcher_v<decltype(equal_to(7.2))>);
}

TEST_CASE ("utils::matches") {
    using c2mm::matchers::less_or_equal_to;
    using c2mm::matchers::less_than;
    using c2mm::matchers::utils::matches;

    CHECK(matches(3, less_than(3.2)));
    CHECK(not matches(3.21, less_or_equal_to(3.2)));
    CHECK(matches(3, 3.0));
    CHECK(not matches(3.1, 3));
}

TEST_CASE ("utils::describe") {
    using c2mm::matchers::greater_than;
    using c2mm::matchers::utils::describe;

    CHECK(describe(greater_than(-4)) == "> -4");
    CHECK(describe(-4) == "-4");
}
