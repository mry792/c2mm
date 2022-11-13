#ifndef C2MM__MOCK__REPORTERS__FAIL_CHECK_HPP_
#define C2MM__MOCK__REPORTERS__FAIL_CHECK_HPP_

#include <string_view>

#include <catch2/catch_test_macros.hpp>

namespace c2mm::mock::reporters {
/**
 * A reporter policy which delegates to Catch2's @c FAIL_CHECK macro.
 */
struct Fail_Check {
    /**
     * Invoke FAIL_CHECK.
     * @param[in] message The message to print on failure.
     */
    void operator () (std::string_view message) const {
        FAIL_CHECK(message);
    }
};
}  // namespace c2mm::mock::reporters

#endif  // C2MM__MOCK__REPORTERS__FAIL_CHECK_HPP_
