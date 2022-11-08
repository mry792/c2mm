#include "c2mm/mock/Default_Action.hpp"

#include <stdexcept>

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("mock::Default_Action") {
    using c2mm::mock::Default_Action;

    SECTION ("default constructible type is default constructed") {
        Default_Action<int> action{};
        CHECK(action() == 0);
        CHECK(action(-4, 17) == 0);
        CHECK(action(std::invalid_argument{""}, 333.2) == 0);
    }

    SECTION ("non-default constructible type throws") {
        struct Not_Default_Constructible {
            Not_Default_Constructible () = delete;
        };

        Default_Action<Not_Default_Constructible> action{};
        CHECK_THROWS_AS(action(), std::runtime_error);
        CHECK_THROWS_AS(action(7.5, "asdf"), std::runtime_error);
        CHECK_THROWS_AS(action(std::domain_error{""}), std::runtime_error);
    }
}
