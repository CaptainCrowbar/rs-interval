#pragma once

#include "rs-format/enum.hpp"
#include "rs-format/format.hpp"
#include <algorithm>
#include <limits>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

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

namespace RS::Intervals {

    namespace Detail {

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
        struct Arithmetic {
            friend T& operator+=(T& lhs, const T& rhs) { lhs = lhs + rhs; return lhs; }
            friend T& operator-=(T& lhs, const T& rhs) { lhs = lhs - rhs; return lhs; }
            friend T& operator*=(T& lhs, const T& rhs) { lhs = lhs * rhs; return lhs; }
            friend T& operator/=(T& lhs, const T& rhs) { lhs = lhs / rhs; return lhs; }
        };

        template <typename T>
        struct TotalOrder {
            friend bool operator!=(const T& lhs, const T& rhs) { return ! (lhs == rhs); }
            friend bool operator>(const T& lhs, const T& rhs) { return rhs < lhs; }
            friend bool operator<=(const T& lhs, const T& rhs) { return ! (rhs < lhs); }
            friend bool operator>=(const T& lhs, const T& rhs) { return ! (lhs < rhs); }
        };

        template <typename T>
        inline int compare_3way(const T& a, const T& b) noexcept {
            return a == b ? 0 : a < b ? -1 : 1;
        }

        inline std::vector<std::string> split_string(const std::string& str, const std::string& chars = "\t\n\f\r ") {
            std::vector<std::string> vec;
            size_t i = 0, j = 0;
            while (j < str.size()) {
                i = str.find_first_not_of(chars, j);
                if (i == std::string::npos)
                    break;
                j = str.find_first_of(chars, i);
                vec.push_back(str.substr(i, j - i));
            }
            return vec;
        }

    }

    RS_FORMAT_DEFINE_ENUM_CLASS(IntervalBound, unsigned char, 0,
        empty,   // The interval is empty
        closed,  // The interval includes the boundary value
        open,    // The interval does not include the boundary value
        unbound  // The interval is unbounded in this direction
    )

    constexpr IntervalBound operator~(IntervalBound b) noexcept { return IntervalBound(3 - int(b)); }

    RS_FORMAT_DEFINE_ENUM_CLASS(IntervalCategory, int, 0,
        none,       // Not usable in an interval
        ordered,    // Ordered but not an arithmetic type (e.g. string)
        stepwise,   // Incrementable and decrementable (e.g. pointer)
        integral,   // Integer arithmetic operations (e.g. integer)
        continuous  // Models a continuous arithmetic type (e.g. floating point)
    )

    RS_FORMAT_DEFINE_ENUM_CLASS(IntervalMatch, int, -1,
        low,    // The value is less than the interval's lower bound
        match,  // The value is an element of the interval
        high,   // The value is greater than the interval's upper bound
        empty   // The interval is empty
    )

    RS_FORMAT_DEFINE_ENUM_CLASS(IntervalOrder, int, -7,
        // Name             Index  Picture    Description
        b_only,             // -7  BBB        A is empty, B is not
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
        a_only              // 7   AAA        B is empty, A is not
    )

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

    namespace Detail {

        constexpr bool is_bounded(IntervalBound b) noexcept {
            using IB = IntervalBound;
            return b == IB::closed || b == IB::open;
        }

        template <typename T>
        struct Boundary:
        public Arithmetic<Boundary<T>>,
        public TotalOrder<Boundary<T>> {
            T value;
            IntervalBound bound;
            bool upper;
            Boundary() = default;
            Boundary(T v, IntervalBound b, bool u): value(v), bound(b), upper(u) {}
            std::string str() const;
            static bool adjacent(const Boundary& l, const Boundary& r) noexcept;
            static int compare(const Boundary& l, const Boundary& r) noexcept;
        };

            template <typename T>
            std::string Boundary<T>::str() const {
                using namespace RS::Format;
                std::string s = upper ? "right" : "left";
                s += " " + to_string(bound);
                if (is_bounded(bound))
                    s += " " + format_object(value);
                return s;
            }

            template <typename T>
            bool Boundary<T>::adjacent(const Boundary& l, const Boundary& r) noexcept {
                using IB = IntervalBound;
                using IC = IntervalCategory;
                if (! is_bounded(l.bound) || ! is_bounded(r.bound))
                    return false;
                if (l.bound != r.bound && l.value == r.value)
                    return true;
                if constexpr (interval_category<T> == IC::stepwise || interval_category<T> == IC::integral) {
                    if (l.bound == IB::closed && r.bound == IB::closed && l.value < r.value) {
                        T t = l.value;
                        return ++t == r.value;
                    }
                }
                return false;
            }

            template <typename T>
            int Boundary<T>::compare(const Boundary& l, const Boundary& r) noexcept {
                using IB = IntervalBound;
                if (l.bound > r.bound) // case 1
                    return - compare(r, l);
                else if (l.bound == IB::empty && r.bound == IB::empty) // case 2
                    return 0;
                else if (l.bound == IB::empty) // case 3
                    return l.upper ? 1 : -1;
                else if (l.bound == IB::unbound && r.bound == IB::unbound) // case 4
                    return compare_3way(l.upper, r.upper);
                else if (r.bound == IB::unbound) // case 5
                    return r.upper ? -1 : 1;
                else if (l.bound == IB::closed && r.bound == IB::closed) // case 6
                    return compare_3way(l.value, r.value);
                else if (l.bound == IB::open && r.bound == IB::open && l.upper == r.upper) // case 7
                    return compare_3way(l.value, r.value);
                else if (r.upper) // case 8
                    return l.value < r.value ? -1 : 1;
                else // case 9
                    return l.value <= r.value ? -1 : 1;
            }

            template <typename T>
            Boundary<T> operator-(const Boundary<T>& b) {
                if (is_bounded(b.bound))
                    return {- b.value, b.bound, ! b.upper};
                else
                    return {{}, b.bound, ! b.upper};
            }

            template <typename T>
            Boundary<T> operator+(const Boundary<T>& l, const Boundary<T>& r) {
                // We will only be adding like to like (lower or upper bounds),
                // so we can assume l.upper==r.upper.
                using IB = IntervalBound;
                if (l.bound == IB::empty || r.bound == IB::empty)
                    return {{}, IB::empty, l.upper};
                else if (l.bound == IB::unbound || r.bound == IB::unbound)
                    return {{}, IB::unbound, l.upper};
                else
                    return {l.value + r.value, std::max(l.bound, r.bound), l.upper};
            }

            template <typename T>
            Boundary<T> operator-(const Boundary<T>& l, const Boundary<T>& r) {
                return l + - r;
            }

            template <typename T>
            bool operator==(const Boundary<T>& l, const Boundary<T>& r) noexcept {
                return Boundary<T>::compare(l, r) == 0;
            }

            template <typename T>
            bool operator<(const Boundary<T>& l, const Boundary<T>& r) noexcept {
                return Boundary<T>::compare(l, r) == -1;
            }

    }

}
