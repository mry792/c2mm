#include "c2mock/args.hpp"

#include <tuple>
#include <type_traits>
#include <utility>

#include <catch2/catch_test_macros.hpp>

SCENARIO ("Arguments can be captured and bound", "[unit]") {
    GIVEN ("some input arguments") {
        double const d = 3.2;
        short i = -4;

        WHEN ("capturing the arguments") {
            auto args = c2mock::capture_args(7, d, std::move(i));

            THEN ("the values are captured") {
                STATIC_CHECK((std::is_same_v<
                    decltype(args),
                    std::tuple<int, double, short>
                >));

                CHECK(args == std::tuple{7, 3.2, -4});
            }
        }

        WHEN ("binding the arguments") {
            auto args = c2mock::bind_args(d, std::move(i));

            THEN ("the values are bound") {
                STATIC_CHECK((std::is_same_v<
                    decltype(args),
                    std::tuple<double const&, short const&>
                >));

                CHECK(&std::get<0>(args) == &d);
                CHECK(&std::get<1>(args) == &i);
            }
        }
    }
}
