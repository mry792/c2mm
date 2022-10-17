#include "c2mock/mp/all.hpp"

#include <tuple>

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("catch2::mp::all") {
    using c2mock::mp::all;
    using Ptr = void*;

    CHECK(all(std::tuple{true, true, Ptr(0x1), 7}));
    CHECK(not all(std::tuple{true, Ptr(0x1), 0.0}));
    CHECK(not all(std::tuple{false, Ptr(0x1), true}));
    CHECK(not all(std::tuple{nullptr, 1, true}));
}
