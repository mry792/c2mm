#ifndef C2MM__MOCK__EXPECTATION_HANDLE_HPP_
#define C2MM__MOCK__EXPECTATION_HANDLE_HPP_

#include <functional>
#include <utility>

namespace c2mm::mock {
/**
 * Helper providing a fluent interface for configuring an @c Expectation.
 *
 * @tparam T_Expectation The type being configured.
 */
template <typename T_Expectation>
class Expectation_Handle {
  public:
    /**
     * Construct with a reference to the object to configure.
     * @param[in] expectation Object to be configured.
     */
    Expectation_Handle (T_Expectation& expectation)
          : expectation_{expectation} {}

    /**
     * Set an action to be performed when the expectation is invoked.
     * @param[in] action The action to perform.
     */
    template <typename T_Action>
    Expectation_Handle& execute (T_Action&& action) {
        expectation_.get().action_ = std::forward<T_Action>(action);
        return *this;
    }

  private:
    std::reference_wrapper<T_Expectation> expectation_;
};
}  // namespace c2mm::mock

#endif  // C2MM__MOCK__EXPECTATION_HANDLE_HPP_
