#pragma once

#include <iterator>
#include <limits>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>

namespace RS::Intervals {

    namespace Detail {

        #define RS_INTERVAL_DETECT_PREFIX_OPERATOR(name, op) \
            template <typename T, typename = void> \
                struct HasPrefixOperator_ ## name: \
                std::false_type {}; \
            template <typename T> \
                struct HasPrefixOperator_ ## name<T, std::void_t<decltype(op std::declval<T&>())>>: \
                std::true_type {}; \
            template <typename T> \
                constexpr bool has_prefix_ ## name ## _operator = HasPrefixOperator_ ## name<T>::value;

        #define RS_INTERVAL_DETECT_POSTFIX_OPERATOR(name, op) \
            template <typename T, typename = void> \
                struct HasPostfixOperator_ ## name: \
                std::false_type {}; \
            template <typename T> \
                struct HasPostfixOperator_ ## name<T, std::void_t<decltype(std::declval<T&>() op)>>: \
                std::true_type {}; \
            template <typename T> \
                constexpr bool has_postfix_ ## name ## _operator = HasPostfixOperator_ ## name<T>::value;

        #define RS_INTERVAL_DETECT_BINARY_OPERATOR(name, op) \
            template <typename T, typename U = T, typename = void> \
                struct HasBinaryOperator ## name: \
                std::false_type {}; \
            template <typename T, typename U> \
                struct HasBinaryOperator ## name<T, U, std::void_t<decltype(std::declval<T>() op std::declval<U>())>>: \
                std::true_type {}; \
            template <typename T, typename U = T> \
                constexpr bool has_ ## name ## _operator = HasBinaryOperator ## name<T, U>::value;

        RS_INTERVAL_DETECT_PREFIX_OPERATOR(increment, ++)
        RS_INTERVAL_DETECT_PREFIX_OPERATOR(decrement, --)
        RS_INTERVAL_DETECT_POSTFIX_OPERATOR(increment, ++)
        RS_INTERVAL_DETECT_POSTFIX_OPERATOR(decrement, --)
        RS_INTERVAL_DETECT_BINARY_OPERATOR(plus, +)
        RS_INTERVAL_DETECT_BINARY_OPERATOR(minus, -)
        RS_INTERVAL_DETECT_BINARY_OPERATOR(multiply, *)
        RS_INTERVAL_DETECT_BINARY_OPERATOR(divide, /)
        RS_INTERVAL_DETECT_BINARY_OPERATOR(remainder, %)
        RS_INTERVAL_DETECT_BINARY_OPERATOR(equality, ==)
        RS_INTERVAL_DETECT_BINARY_OPERATOR(inequality, !=)
        RS_INTERVAL_DETECT_BINARY_OPERATOR(less_than, <)
        RS_INTERVAL_DETECT_BINARY_OPERATOR(greater_than, >)
        RS_INTERVAL_DETECT_BINARY_OPERATOR(less_or_equal, <=)
        RS_INTERVAL_DETECT_BINARY_OPERATOR(greater_or_equal, >=)

        template <typename T> constexpr bool has_stepwise_operators =
            (has_prefix_increment_operator<T> && has_prefix_decrement_operator<T>
            && has_postfix_increment_operator<T> && has_postfix_decrement_operator<T>);

        template <typename T> constexpr bool has_basic_arithmetic_operators =
            (has_plus_operator<T> && has_minus_operator<T>
            && has_multiply_operator<T> && has_divide_operator<T>);

        template <typename T> constexpr bool has_comparison_operators =
            (has_equality_operator<T> && has_inequality_operator<T>
            && has_less_than_operator<T> && has_greater_than_operator<T>
            && has_less_or_equal_operator<T> && has_greater_or_equal_operator<T>);

        template <typename T>
        struct LessThanComparable {
            bool operator!=(const T& rhs) const { return ! (static_cast<const T&>(*this) == rhs); }
            bool operator>(const T& rhs) const { return rhs < static_cast<const T&>(*this); }
            bool operator<=(const T& rhs) const { return ! (rhs < static_cast<const T&>(*this)); }
            bool operator>=(const T& rhs) const { return ! (static_cast<const T&>(*this) < rhs); }
        };

        template <typename T, typename = void> struct HasStrMethod: std::false_type {};
        template <typename T> struct HasStrMethod<T,
            std::void_t<decltype(std::declval<std::string&>() = std::declval<const T&>().str())>>: std::true_type {};
        template <typename T> constexpr bool has_str_method = HasStrMethod<T>::value;

        template <typename T, typename = void> struct HasAdlToStringFunction: std::false_type {};
        template <typename T> struct HasAdlToStringFunction<T,
            std::void_t<decltype(std::declval<std::string&>() = to_string(std::declval<const T&>()))>>: std::true_type {};
        template <typename T> constexpr bool has_adl_to_string_function = HasAdlToStringFunction<T>::value;

        template <typename T, typename = void> struct HasStdToStringFunction: std::false_type {};
        template <typename T> struct HasStdToStringFunction<T,
            std::void_t<decltype(std::declval<std::string&>() = std::to_string(std::declval<const T&>()))>>: std::true_type {};
        template <typename T> constexpr bool has_std_to_string_function = HasStdToStringFunction<T>::value;

        template <typename T, typename = void> struct HasAdlBeginFunction: std::false_type {};
        template <typename T> struct HasAdlBeginFunction<T, std::void_t<decltype(begin(std::declval<const T&>()))>>: std::true_type {};
        template <typename T, typename = void> struct HasAdlEndFunction: std::false_type {};
        template <typename T> struct HasAdlEndFunction<T, std::void_t<decltype(end(std::declval<const T&>()))>>: std::true_type {};
        template <typename T, typename = void> struct HasStdBeginFunction: std::false_type {};
        template <typename T> struct HasStdBeginFunction<T, std::void_t<decltype(std::begin(std::declval<const T&>()))>>: std::true_type {};
        template <typename T, typename = void> struct HasStdEndFunction: std::false_type {};
        template <typename T> struct HasStdEndFunction<T, std::void_t<decltype(std::end(std::declval<const T&>()))>>: std::true_type {};

        template <typename T> constexpr bool is_range = (HasAdlBeginFunction<T>::value && HasAdlEndFunction<T>::value)
            || (HasStdBeginFunction<T>::value && HasStdEndFunction<T>::value);

        enum class BoundaryType {
            minus_infinity,
            value_minus_epsilon,
            exact_value,
            value_plus_epsilon,
            plus_infinity
        };

        constexpr bool no_boundary(BoundaryType b) noexcept {
            return b == BoundaryType::minus_infinity || b == BoundaryType::plus_infinity;
        }

    }

    // Supporting types

    enum class IntervalBound: int {
        empty,    // The interval is empty
        closed,   // The interval includes the boundary value
        open,     // The interval does not include the boundary value
        unbound,  // The interval is unbounded in this direction
    };

    constexpr IntervalBound operator~(IntervalBound b) noexcept { return IntervalBound(3 - int(b)); }

    inline std::ostream& operator<<(std::ostream& out, IntervalBound ib) {
        switch (ib) {
            case IntervalBound::empty:    out << "empty"; break;
            case IntervalBound::closed:   out << "closed"; break;
            case IntervalBound::open:     out << "open"; break;
            case IntervalBound::unbound:  out << "unbound"; break;
            default:                      out << int(ib); break;
        }
        return out;
    }

    enum class IntervalCategory: int {
        none,        // Not usable in an interval
        ordered,     // Ordered but not an arithmetic type (e.g. string)
        stepwise,    // Incrementable and decrementable (e.g. pointer)
        integral,    // Integer arithmetic operations (e.g. integer)
        continuous,  // Models a continuous arithmetic type (e.g. floating point)
    };

    inline std::ostream& operator<<(std::ostream& out, IntervalCategory ic) {
        switch (ic) {
            case IntervalCategory::none:        out << "none"; break;
            case IntervalCategory::ordered:     out << "ordered"; break;
            case IntervalCategory::stepwise:    out << "stepwise"; break;
            case IntervalCategory::integral:    out << "integral"; break;
            case IntervalCategory::continuous:  out << "continuous"; break;
            default:                            out << int(ic); break;
        }
        return out;
    }

    enum class IntervalMatch: int {
        low = -1,  // The value is less than the interval's lower bound
        match,     // The value is an element of the interval
        high,      // The value is greater than the interval's upper bound
        empty,     // The interval is empty
    };

    inline std::ostream& operator<<(std::ostream& out, IntervalMatch im) {
        switch (im) {
            case IntervalMatch::low:    out << "low"; break;
            case IntervalMatch::match:  out << "match"; break;
            case IntervalMatch::high:   out << "high"; break;
            case IntervalMatch::empty:  out << "empty"; break;
            default:                    out << int(im); break;
        }
        return out;
    }

    enum class IntervalOrder: int {
        // Name             Index  Picture    Description
        b_only = -7,        // -7  BBB        A is empty, B is not
        a_below_b,          // -6  AAA...BBB  Upper bound of A is less than lower bound of B, with a gap
        a_touches_b,        // -5  AAABBB     Upper bound of A is less than lower bound of B, with no gap
        a_overlaps_b,       // -4  AAA***BBB  Upper bound of A overlaps lower bound of B
        a_extends_below_b,  // -3  AAA***     B is a subset of A, with the same upper bound
        a_encloses_b,       // -2  AAA***AAA  B is a subset of A, matching neither bound
        b_extends_above_a,  // -1  ***BBB     A is a subset of B, with the same lower bound
        equal,              // 0   ***        A and B are the same (this includes the case where both are empty)
        a_extends_above_b,  // 1   ***AAA     B is a subset of A, with the same lower bound
        b_encloses_a,       // 2   BBB***BBB  A is a subset of B, matching neither bound
        b_extends_below_a,  // 3   BBB***     A is a subset of B, with the same upper bound
        b_overlaps_a,       // 4   BBB***AAA  Upper bound of B overlaps lower bound of A
        b_touches_a,        // 5   BBBAAA     Upper bound of B is less than lower bound of A, with no gap
        b_below_a,          // 6   BBB...AAA  Upper bound of B is less than lower bound of A, with a gap
        a_only,             // 7   AAA        B is empty, A is not
    };

    inline std::ostream& operator<<(std::ostream& out, IntervalOrder io) {
        switch (io) {
            case IntervalOrder::b_only:             out << "b_only"; break;
            case IntervalOrder::a_below_b:          out << "a_below_b"; break;
            case IntervalOrder::a_touches_b:        out << "a_touches_b"; break;
            case IntervalOrder::a_overlaps_b:       out << "a_overlaps_b"; break;
            case IntervalOrder::a_extends_below_b:  out << "a_extends_below_b"; break;
            case IntervalOrder::a_encloses_b:       out << "a_encloses_b"; break;
            case IntervalOrder::b_extends_above_a:  out << "b_extends_above_a"; break;
            case IntervalOrder::equal:              out << "equal"; break;
            case IntervalOrder::a_extends_above_b:  out << "a_extends_above_b"; break;
            case IntervalOrder::b_encloses_a:       out << "b_encloses_a"; break;
            case IntervalOrder::b_extends_below_a:  out << "b_extends_below_a"; break;
            case IntervalOrder::b_overlaps_a:       out << "b_overlaps_a"; break;
            case IntervalOrder::b_touches_a:        out << "b_touches_a"; break;
            case IntervalOrder::b_below_a:          out << "b_below_a"; break;
            case IntervalOrder::a_only:             out << "a_only"; break;
            default:                                out << int(io); break;
        }
        return out;
    }

    template <typename T>
    struct IntervalTraits {
        using type = std::remove_cv_t<T>;
        static constexpr IntervalCategory category =
            ! std::is_default_constructible_v<type> || ! std::is_copy_constructible_v<type> || ! std::is_copy_assignable_v<type>
                    || ! Detail::has_comparison_operators<type> || std::is_same_v<type, bool> ?
                IntervalCategory::none :
            std::is_integral_v<T> || (std::numeric_limits<type>::is_specialized && std::numeric_limits<type>::is_integer) ?
                IntervalCategory::integral :
            std::is_floating_point_v<T> || (std::numeric_limits<type>::is_specialized && ! std::numeric_limits<type>::is_integer) ?
                IntervalCategory::continuous :
            Detail::has_basic_arithmetic_operators<type> && Detail::has_stepwise_operators<type> ?
                IntervalCategory::integral :
            Detail::has_basic_arithmetic_operators<type> ?
                IntervalCategory::continuous :
            Detail::has_stepwise_operators<type> ?
                IntervalCategory::stepwise :
                IntervalCategory::ordered;
    };

    template <typename T> constexpr auto interval_category = IntervalTraits<T>::category;

}
