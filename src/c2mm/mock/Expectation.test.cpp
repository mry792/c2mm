#include "c2mm/mock/Expectation.hpp"

#include <memory>
#include <type_traits>
#include <utility>

#include <catch2/catch_test_macros.hpp>

#include "c2mm/matchers/Tuple_Matcher.hpp"
#include "c2mm/matchers/Typed_Wrapper.hpp"
#include "c2mm/mock/args.hpp"
#include "c2mm/mp/utils.hpp"

SCENARIO ("Class c2mm::mock::Expectation controls access to an action.") {
    using c2mm::mock::bind_args;
    using c2mm::mock::capture_args;
    using c2mm::mp::utils::wrap_unique;

    GIVEN ("an Expectation instance") {
        using c2mm::matchers::wrap_for;
        using c2mm::matchers::matches;
        using Expectation = c2mm::mock::Expectation<int(int, int)>;
        using Args_Tuple = typename Expectation::Args_Tuple;

        auto matcher_ptr = wrap_unique(wrap_for<Args_Tuple>(
            matches(capture_args(3, 4))
        ));
        auto& matcher = *matcher_ptr;
        Expectation expectation{std::move(matcher_ptr)};

        THEN ("the expectation contains the constructed matcher") {
            CHECK(&expectation.matcher() == &matcher);
        }

        THEN ("only calls with matching arguments can be consumed") {
            CHECK(expectation.can_consume(bind_args(3, 4)));
            CHECK(not expectation.can_consume(bind_args(3 + 1, 4)));
            CHECK(not expectation.can_consume(bind_args(3, 4 - 1)));
        }

        WHEN ("a call is handled") {
            auto result = expectation.handle_call(3, 4);

            THEN ("the default action is executed") {
                CHECK(result == 0);
            }

            THEN ("no more calls can be consumed") {
                CHECK(not expectation.can_consume(bind_args(3, 4)));
            }
        }
    }
}
