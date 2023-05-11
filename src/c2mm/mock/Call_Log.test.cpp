#include "c2mm/mock/Call_Log.hpp"

#include <exception>
#include <functional>
#include <string>
#include <tuple>

#include <catch2/catch_test_macros.hpp>

#include "c2mm/matchers/Tuple_Matcher.hpp"
#include "c2mm/mock/Mock_Function.hpp"
#include "c2mm/mock/reporters/Mock.hpp"

SCENARIO (
    "If all calls are consumed, Call_Log::check_no_calls() doesn't fail."
) {
    GIVEN ("a Call_Log") {
        c2mm::mock::Call_Log<std::tuple<int>> call_log{};

        WHEN ("some calls are logged") {
            call_log.log(7);
            call_log.log(-4);

            AND_WHEN ("those calls are all matched") {
                using c2mm::matchers::matches;
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

namespace reporters = c2mm::mock::reporters;
SCENARIO ("If some calls are unconsumed, Call_Log::check_no_calls() fails.") {
    using c2mm::mock::Call_Log;

    GIVEN ("a Call_Log") {
        using c2mm::mock::Call_Log;
        using Test_Call_Log = Call_Log<std::tuple<int>>;

        reporters::Mock mock_reporter{};
        Test_Call_Log call_log{};

        WHEN ("some calls are logged") {
            call_log.log(7);
            call_log.log(-4);
            call_log.log(2);

            AND_WHEN ("not all calls are matched") {
                using c2mm::matchers::matches;
                CHECK(call_log.consume_match(matches(std::tuple{-4})));

                THEN ("Call_Log::check_no_calls() fails (twice)") {
                    call_log.check_no_calls(std::ref(mock_reporter));
                    mock_reporter.check_called("unconsumed call");
                    mock_reporter.check_called("unconsumed call");
                }
            }
        }
    }
}
