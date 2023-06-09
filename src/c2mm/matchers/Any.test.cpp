#include "c2mm/matchers/Any.hpp"

#include <string>
#include <string_view>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

template <typename T>
void check_any (c2mm::matchers::Any const& matcher, T const& obj) {
    CHECK(matcher.match(obj));
    CHECK_THAT(obj, matcher);
    CHECK_THAT(obj, c2mm::matchers::anything);
    CHECK_THAT(obj, c2mm::matchers::_);
    CHECK_THAT(obj, c2mm::matchers::matches_anything());
}

TEST_CASE ("class c2mm::matchers::Any") {
    auto const matcher = c2mm::matchers::Any{};

    SECTION (".describe()") {
        CHECK(matcher.describe() == "is anything");
        CHECK(c2mm::matchers::anything.describe() == "is anything");
        CHECK(c2mm::matchers::_.describe() == "is anything");
        CHECK(c2mm::matchers::matches_anything().describe() == "is anything");
    }

    SECTION (".match(obj)") {
        using namespace std::literals;
        check_any(matcher, GENERATE(-7, -1, 0, 1, 13));
        check_any(matcher, GENERATE("asdf"s, ""s, "\0"s));
        check_any(matcher, GENERATE("asdf"sv, ""sv, "\0"sv));
        check_any(matcher, std::vector{2.1, -3.3, 0.0, 1.0});
    }
}
