#include "c2mm/mock/Mock_Function.hpp"

#include <functional>
#include <optional>
#include <string>
#include <tuple>

#include <catch2/catch_test_macros.hpp>

#include "c2mm/matchers/Comparison_Matcher.hpp"
#include "c2mm/mock/reporters/Mock.hpp"

SCENARIO ("If all calls are consumed, Mock_Function doesn't fail.") {
    GIVEN ("a Mock_Function") {
        using c2mm::mock::Mock_Function;
        auto func_ptr = std::make_unique<Mock_Function<void(int, double)>>();
        auto& func = *func_ptr;

        WHEN ("some calls are logged") {
            func(7, 4.2);
            func(-4, 1.1);

            AND_WHEN ("those calls are all matched") {
                using c2mm::matchers::less_than;
                func.check_called(less_than(8), 4.2);
                func.require_called(-4, less_than(2));

                THEN ("Mock_Function::~Mock_Function() doesn't fail") {
                    func_ptr.reset();
                }
            }
        }
    }
}

namespace reporters = c2mm::mock::reporters;
SCENARIO ("Mock_Function fails when calls aren't matched.") {
    GIVEN ("a Mock_Function") {
        using c2mm::mock::Mock_Function;
        using Func = Mock_Function<void(double, int), reporters::Mock_Ref>;

        reporters::Mock mock_reporter{};

        auto func_ptr = std::make_unique<Func>(std::ref(mock_reporter));
        auto& func = *func_ptr;
        (void)func;

        WHEN ("some calls are logged") {
            func(2.2, 4);
            func(3.3, -2);

            AND_WHEN ("not all calls are matched") {
                using c2mm::matchers::greater_than;
                func.check_called(greater_than(2), greater_than(0));

                THEN ("Mock_Function::~Mock_Function() fails") {
                    func_ptr.reset();
                    mock_reporter.check_called("unconsumed call");
                }
            }

            AND_WHEN ("unmatched .validate_call() fails") {
                using c2mm::matchers::less_than;
                func.validate_called(
                    std::ref(mock_reporter),
                    less_than(3),
                    less_than(-2)
                );

                THEN ("the failure is reported") {
                    mock_reporter.check_called(
                        "No call whose arguments match."
                    );

                    func_ptr.reset();
                    mock_reporter.check_called("unconsumed call");
                    mock_reporter.check_called("unconsumed call");
                }
            }
        }
    }
}
