#include "c2mm/mp/zip_with.hpp"

#include <functional>
#include <tuple>
#include <vector>

#include <catch2/catch_test_macros.hpp>

// TODO(emery): Add test using a mock function.

TEST_CASE ("c2mm::mp::zip_with") {
    using c2mm::mp::zip_with;
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

    {
        std::vector<std::tuple<double, int>> vals{};

        zip_with(
            [&vals] (double d, int i) { vals.emplace_back(d, i); },
            std::tuple{3.2, 4.3, 5},
            std::tuple{6, 7, 8.2}
        );

        CHECK(vals == decltype(vals){
            {3.2, 6},
            {4.3, 7},
            {5.0, 8},
        });
    }

    CHECK(std::is_same_v<
        void,
        decltype(
            zip_with(
                [](auto){},
                std::tuple{7, 3.2}
            )
        )
    >);
}
