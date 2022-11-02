#include "c2mm/mock/Call_Log.hpp"

#include <tuple>

#include <catch2/catch_test_macros.hpp>

#include "c2mm/matchers/Tuple_Matcher.hpp"

SCENARIO (
    "If all calls are consumed, Call_Log::check_no_calls() doesn't fail."
) {
    GIVEN ("a Call_Log") {
        c2mock::mock::Call_Log<std::tuple<int>> call_log{};

        WHEN ("some calls are logged") {
            call_log.log(7);
            call_log.log(-4);

            AND_WHEN ("those calls are all matched") {
                using c2mock::matchers::matches;
                CHECK(call_log.consume_match(matches(std::tuple{-4})));
                CHECK(not call_log.consume_match(matches(std::tuple{4})));
                CHECK(call_log.consume_match(matches(std::tuple{7})));

                THEN ("Call_Log::check_no_calls() doesn't fail") {
                    call_log.check_no_calls();
                }
            }
        }
    }
}

SCENARIO (
    "If some calls are unconsumed, Call_Log::check_no_calls() fails.",
    "[!shouldfail]"
) {
    GIVEN ("a Call_Log") {
        c2mock::mock::Call_Log<std::tuple<int>> call_log{};

        WHEN ("some calls are logged") {
            call_log.log(7);
            call_log.log(-4);
            call_log.log(2);

            AND_WHEN ("not all calls are matched") {
                using c2mock::matchers::matches;
                CHECK(call_log.consume_match(matches(std::tuple{-4})));

                THEN ("Call_Log::check_no_calls() fails") {
                    call_log.check_no_calls();
                }
            }
        }
    }
}
