#ifndef C2MM__MOCK__REPORTERS__THROWS_HPP_
#define C2MM__MOCK__REPORTERS__THROWS_HPP_

#include <utility>

namespace c2mm::mock::reporters {
/**
 * A reporter policy which throws an exception of the specified type.
 * @tparam T_Exception The type of exception to throw.
 */
template <typename T_Exception>
struct Throws {
    /**
     * Throw a new exception.
     * @param[in] args... Arguments to forward to the exception's constructor.
     * @throws T_Exception Always.
     */
    template <typename... T_Args>
    void operator () (T_Args&&... args) const {
        throw T_Exception{std::forward<T_Args>(args)...};
    }
};
}  // namespace c2mm::mock::reporters

#endif  // C2MM__MOCK__REPORTERS__THROWS_HPP_
