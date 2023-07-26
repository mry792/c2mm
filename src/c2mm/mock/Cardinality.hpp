#ifndef C2MM__MOCK__CARDINALITY_HPP_
#define C2MM__MOCK__CARDINALITY_HPP_

#include <cstddef>
#include <functional>
#include <string_view>
#include <variant>

namespace c2mm::mock {
/**
 *
 */
class Cardinality {
  public:
    /**
     * Indicates the status of the associated expectation.
     */
    enum struct Status {
        /**
         * The expectation is unmet. Destroying the mock at this point will fail
         * the test.
         */
        unsatisfied,

        /**
         * The expectation is met. Destroying the mock will not prompt this
         * expectation to fail the test. However, more calls can still be
         * consumed by the expectation.
         */
        satisfied,

        /**
         * The expectation is met and it can consume no more calls. It will be
         * skipped when the mock function is looking for an expectation to
         * handle a call.
         */
        retired,
    };

    /**
     * Helper type for implementing single threshold cardinalities.
     * @tparam t_over_result The @c Status if the @c call_count is over the
     *     threshold.
     */
    template <Status t_over_result>
    struct Threshold {
        std::size_t threshold_;

        constexpr Status operator () (std::size_t call_count) const {
            return
                call_count < threshold_ ?
                Status::unsatisfied :
                t_over_result;
        }
    };

    struct Exact : Threshold<Status::retired> {};
    struct At_Least : Threshold<Status::satisfied> {};

    /**
     * Helper type for implementing double threshold cardinalities.
     */
    struct Between {
        std::size_t satisfied_threshold_;
        std::size_t retired_threshold_;

        constexpr Status operator () (std::size_t call_count) const {
            if (call_count >= retired_threshold_) {
                return Status::retired;
            } else if (call_count >= satisfied_threshold_) {
                return Status::satisfied;
            }
            return Status::unsatisfied;
        }
    };

    struct Optional {
        constexpr Status operator () (std::size_t /* call_count */) const {
            return Status::satisfied;
        }
    };

    template <
        typename T,
        typename = std::enable_if_t<
            not std::is_same_v<
                std::remove_cvref_t<T>,
                Cardinality
            >
        >
    >
    constexpr Cardinality (T&& impl) : impl_{std::forward<T>(impl)} {}

    /**
     * Check the status based on the @p call_count.
     * @param[in] call_count
     * @return The status of the associated expectation.
     */
    constexpr Status status (std::size_t call_count) const {
        return std::visit(
            [call_count] (auto const& impl) -> Status {
                return impl(call_count);
            },
            impl_
        );
    }

  private:
    std::variant<
        Exact,
        At_Least,
        Between,
        Optional,
        std::function<Status(std::size_t)>
    > impl_;
};

std::string_view to_string (Cardinality::Status status) {
    switch (status) {
        case Cardinality::Status::unsatisfied:

        case Cardinality::Status::satisfied:

        case Cardinality::Status::retired:

}
}  // namespace c2mm::mock

#endif  // C2MM__MOCK__CARDINALITY_HPP_
