#ifndef C2MM__MP__UTILS_HPP_
#define C2MM__MP__UTILS_HPP_

#include <memory>
#include <type_traits>

namespace c2mm::mp::utils {
/**
 * Wrap @p obj in a @c std::unique_ptr.
 *
 * Intended for when the type of @p obj is known but unspecified, such as the
 * return type of a function.
 *
 * @param[in] obj The object to wrap. Will be forwarded to @c std::make_unique.
 *
 * @return A @c std::unique_ptr owning the object.
 */
template <typename T>
std::unique_ptr<T> wrap_unique (T&& obj) {
    return std::make_unique<std::remove_reference_t<T>>(std::forward<T>(obj));
}
}  // namespace c2mm::mp::utils

#endif  // C2MM__MP__UTILS_HPP_
