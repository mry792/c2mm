#ifndef C2MOCK__MOCK__CALL_LOG_HPP_
#define C2MOCK__MOCK__CALL_LOG_HPP_

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "c2mm/mock/reporters/Fail_Check.hpp"

namespace c2mm::mock {
/**
 * Represents a log of calls to some function.
 *
 * @tparam T_Arg_Tuple A @c std::tuple representing the arguments to the
 *     function in question.
 * @tparam T_Reporter Policy dictating how failures are reported.
 */
template <typename T_Arg_Tuple>
class Call_Log {
  public:
    using Arg_Tuple = T_Arg_Tuple;
    using Call_List = std::vector<std::unique_ptr<Arg_Tuple const>>;

    /**
     * Read-only accessor for the unconsumed calls logged with this object.
     */
    Call_List const& calls () const { return calls_; }

    /**
     * Log a call with this object. This owns the new argument objects.
     * @param[in] args Arguments of the call to log.
     */
    template <typename... Args>
    void log (Args&&... args) {
        calls_.emplace_back(
            std::make_unique<Arg_Tuple>(
                std::forward<Args>(args)...
            )
        );
    }

    /**
     * Consume the first call found that matches @p matcher. This will remove it
     * from the @c Call_Log.
     * @param[in] matcher A matcher than can accept @p T_Arg_Tuple.
     * @return @c true if a call was found to match. Else returns @c false.
     */
    template <typename T_Arg_Matcher>
    bool consume_match (T_Arg_Matcher const& matcher) {
        auto match = [&matcher] (auto const& call) {
            return matcher.match(*call);
        };
        auto iter = std::ranges::find_if(calls_, match);

        if (iter == calls_.end()) {
            return false;
        }

        calls_.erase(iter);
        return true;
    }

    /**
     * Verifies that no unconsumed calls remain logged with this instance.
     *
     * This should only be called as part of a Catch2 @c TEST_CASE. It is a
     * check-style verification where failure fails the test but continues
     * executing.
     */
    template <typename T_Reporter = reporters::Fail_Check>
    void check_no_calls (T_Reporter reporter = T_Reporter{}) {
        for (auto const& call : calls_) {
            (void)call;
            // TODO(emery): print out function name and arguments
            reporter("unconsumed call");
        }

        calls_.clear();
    }

  private:
    Call_List calls_;
};
}  // namespace c2mm::mock

#endif  // C2MOCK__MOCK__CALL_LOG_HPP_
