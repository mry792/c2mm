#ifndef C2MM__MOCK__REPORTERS__MOCK_HPP_
#define C2MM__MOCK__REPORTERS__MOCK_HPP_

#include <functional>
#include <string>

#include "c2mm/mock/Mock_Function.hpp"

namespace c2mm::mock::reporters {
using Mock = Mock_Function<void(std::string)>;
using Mock_Ref = std::reference_wrapper<Mock>;
}  // namespace c2mm::mock::reporters

#endif  // C2MM__MOCK__REPORTERS__MOCK_HPP_
