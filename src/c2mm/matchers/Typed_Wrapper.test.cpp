#include "c2mm/matchers/Typed_Wrapper.hpp"

#include <string>
#include <utility>

#include <catch2/catch_test_macros.hpp>

#include "c2mm/matchers/Comparison_Matcher.hpp"

// TODO: Rewrite these tests using mocks.

TEST_CASE ("matchers::Typed_Wrapper constructors") {
    using c2mm::matchers::Typed_Wrapper;
    using Wrapper = Typed_Wrapper<std::string, std::string>;

    SECTION ("value constructor") {
        // ARRANGE
        std::string const constraint = "asdf";

        // ACT
        Wrapper matcher{constraint};

        // ASSERT
        CHECK(matcher.constraint() == constraint);
    }

    SECTION ("in-place constructor") {
        // ACT
        Wrapper matcher{std::in_place, 4, 't'};

        // ASSERT
        CHECK(matcher.constraint() == "tttt");
    }
}

SCENARIO ("matchers::Typed_Wrapper wraps the constraint") {
    using c2mm::matchers::wrap_for;

    GIVEN ("a value to wrap") {
        std::string const constraint = "asdf";

        WHEN ("wrapped") {
            auto const matcher = wrap_for<std::string>(constraint);

            THEN ("match checks for equality") {
                CHECK(matcher.match(constraint));
                CHECK(matcher.match(constraint.c_str()));
                CHECK(not matcher.match("asdf+"));
                CHECK(not matcher.match("asd"));
            }

            THEN ("the matcher describes the wrapped constraint") {
                CHECK(matcher.describe() == R"(is "asdf")");
            }
        }
    }

    GIVEN ("a matcher to wrap") {
        using c2mm::matchers::not_equal_to;
        auto constraint = not_equal_to(7);

        WHEN ("wrapped") {
            auto const matcher = wrap_for<double>(constraint);

            THEN ("match delegates to the underlying matcher") {
                CHECK(matcher.match(7.1));
                CHECK(matcher.match(6.9));
                CHECK(not matcher.match(7.0));
            }

            THEN ("the matcher describes the underlying matcher") {
                CHECK(matcher.describe() == "is != 7");
            }
        }
    }
}
