#ifndef C2MM__MATCHERS__EXCEPTION_MATCHER_HPP_
#define C2MM__MATCHERS__EXCEPTION_MATCHER_HPP_

#include <exception>
#include <string>
#include <type_traits>
#include <utility>

#include <catch2/matchers/catch_matchers_templated.hpp>

#include "c2mm/matchers/utils.hpp"

namespace c2mm::matchers {
template <typename T_Base_Exception, typename T_Message_Constraint>
class Exception_Matcher final : public Catch::Matchers::MatcherGenericBase {
  public:
    Exception_Matcher (
        std::string base_exception_description,
        T_Message_Constraint message_constraint
    )     : base_exception_description_{std::move(base_exception_description)},
            message_constraint_{std::move(message_constraint)} {}

    template <typename T_Value>
    bool match (T_Value const& value) const {
        return std::is_base_of_v<T_Base_Exception, T_Value> and
            utils::matches(value.what(), message_constraint_);
    }

    std::string describe () const override {
        return "<<TODO>>"
    }

  private:
    std::string base_exception_description_;
    T_Message_Constraint message_constraint_;
};
}  // namespace c2mm::matchers

#endif  // C2MM__MATCHERS__EXCEPTION_MATCHER_HPP_
