#include "c2mm/matchers/Comparison_Matcher.hpp"

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

// TODO(emery): Test class Comparison_Matcher with a mock function.

TEST_CASE ("class c2mm::matchers::Comparison_Matcher - less_than(int)") {
    auto const matcher = c2mm::matchers::less_or_equal_to(7);

    SECTION (".describe()") {
        CHECK(matcher.describe() == "<= 7");
    }

    SECTION (".match(int)") {
        auto const [value, result] = GENERATE(table<int, bool>({
            {-4, true},
            {6, true},
            {7, true},
            {8, false},
            {999, false},
        }));

        CAPTURE(value, matcher.describe());
        CHECK(matcher.match(value) == result);
    }

    SECTION (".match(double)") {
        auto const [value, result] = GENERATE(table<double, bool>({
            {-2.1, true},
            {6.89, true},
            {7.0, true},
            {7.01, false},
            {12345.6, false},
        }));

        CAPTURE(value, matcher.describe());
        CHECK(matcher.match(value) == result);
    }
}
