#include "c2mm/mock/Cardinality.hpp"

#include <catch2/catch_test_macros.hpp>

SCENARIO ("Cardinalities assess status") {
    using c2mm::mock::Cardinality;

    GIVEN ("An exact Cardinality") {
        Cardinality card{Cardinality::Exact{3}};


    }
}
