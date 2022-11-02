#include "c2mm/matchers/Tuple_Matcher.hpp"

#include <catch2/catch_test_macros.hpp>

#include "c2mm/matchers/Comparison_Matcher.hpp"

TEST_CASE ("c2mock::matchers::Tuple_Matcher") {
    using c2mock::matchers::less_than;
    using c2mock::matchers::matches;

    auto matcher = matches(std::tuple{7, less_than(3.14159)});

    SECTION (".match()") {
        CHECK(matcher.match(std::tuple{7, 3}));
        CHECK(matcher.match(std::tuple{7.0, 3}));
        CHECK(not matcher.match(std::tuple{7.2, 3.1}));
        CHECK(not matcher.match(std::tuple{7, 3.5}));
    }

    // TODO(emery): test ".describe()"
}
