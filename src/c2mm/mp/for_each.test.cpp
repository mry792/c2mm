#include "c2mm/mp/for_each.hpp"

#include <sstream>
#include <string>
#include <tuple>

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("c2mm::mp::for_each") {
    using c2mm::mp::for_each;

    std::tuple<int, std::string, double> tup{7, "asdf", -3.2};
    std::ostringstream buf{};

    SECTION ("no index") {
        for_each(tup, [&buf] (auto const& item) {
            buf << item << " ";
        });

        CHECK(buf.str() == "7 asdf -3.2 ");
    }

    SECTION ("with index") {
        for_each(tup, [&buf] (int idx, auto const& item) {
            buf << idx << ": " << item << " - ";
        });

        CHECK(buf.str() == "0: 7 - 1: asdf - 2: -3.2 - ");
    }
}
