#include "c2mock/mp/zip_with.hpp"

#include <functional>
#include <tuple>

#include <catch2/catch_test_macros.hpp>

// TODO(emery): Add test using a mock function.

TEST_CASE ("c2mock::mp::zip_with") {
    using c2mock::mp::zip_with;
    std::less<> comp{};

    CHECK(zip_with(comp, std::tuple{1, 2.3}, std::tuple{1.2, 3}) == std::tuple{true, true});
    CHECK(zip_with(comp, std::tuple{1, 2.3}, std::tuple{0.2, 3}) == std::tuple{false, true});
    CHECK(zip_with(comp, std::tuple{1, 2.3}, std::tuple{2, 2.2}) == std::tuple{true, false});

    CHECK(
        zip_with(
            [] (auto... xs) { return (xs + ...); },
            std::tuple{1, 3, 5},
            std::tuple{2, 2, 2},
            std::tuple{1.1, 2, 0.1}
        ) == std::tuple{4.1, 7, 7.1}
    );
}
