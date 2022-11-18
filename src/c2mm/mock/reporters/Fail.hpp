#ifndef C2MM__MOCK__REPORTERS__FAIL_HPP_
#define C2MM__MOCK__REPORTERS__FAIL_HPP_

#include <string_view>

#include <catch2/catch_test_macros.hpp>

namespace c2mm::mock::reporters {
/**
 * A reporter policy which delegates to Catch2's @c FAIL macro.
 */
struct Fail {
    /**
     * Invoke FAIL.
     * @param[in] message The message to print on failure.
     */
    void operator () (std::string_view message) const {
        FAIL(message);
    }
};
}  // namespace c2mm::mock::reporters

#endif  // C2MM__MOCK__REPORTERS__FAIL_HPP_
