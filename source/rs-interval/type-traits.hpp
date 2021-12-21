#pragma once

#include "rs-format/enum.hpp"
#include "rs-format/format.hpp"
#include <algorithm>
#include <cstdlib>
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

        template <typename T> constexpr bool is_totally_ordered =
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
            std::is_same_v<type, bool> || ! std::is_default_constructible_v<type>
                    || ! std::is_copy_constructible_v<type> || ! std::is_copy_assignable_v<type>
                    || ! Detail::is_totally_ordered<type> ?
                IntervalCategory::none :
            std::is_floating_point_v<T>
                    || (std::numeric_limits<type>::is_specialized && ! std::numeric_limits<type>::is_integer) ?
                IntervalCategory::continuous :
            std::is_integral_v<T>
                    || (std::numeric_limits<type>::is_specialized && std::numeric_limits<type>::is_integer) ?
                IntervalCategory::integral :
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

        RS_FORMAT_DEFINE_ENUM_CLASS(BoundaryType, int, -3,
            null,
            minus_infinity,
            minus_epsilon,
            exact,
            plus_epsilon,
            plus_infinity
        )

        template <typename T>
        struct Boundary:
        public Arithmetic<Boundary<T>>,
        public TotalOrder<Boundary<T>> {
            T value = T();
            BoundaryType type = BoundaryType::null;
            Boundary() = default;
            Boundary(T v, BoundaryType t);
            bool adjacent(const Boundary& b) const noexcept;
            int compare(const Boundary& b) const noexcept;
            bool has_value() const noexcept;
            std::string str() const;
            Boundary operator-() const;
            Boundary operator+(const Boundary& b) const;
            Boundary operator-(const Boundary& b) const { return *this + - b; }
            Boundary operator*(const Boundary& b) const;
            bool operator==(const Boundary& b) const noexcept { return compare(b) == 0; }
            bool operator<(const Boundary& b) const noexcept { return compare(b) < 0; }
        };

            template <typename T>
            Boundary<T>::Boundary(T v, BoundaryType t):
            value(), type(t) {
                if (has_value())
                    value = v;
            }

            template <typename T>
            bool Boundary<T>::adjacent(const Boundary& b) const noexcept {
                using BT = BoundaryType;
                using IC = IntervalCategory;
                if (! has_value() || ! b.has_value())
                    return false;
                if (type != BT::exact && b.type != BT::exact)
                    return false;
                if (value == b.value)
                    return std::abs(int(type) - int(b.type)) == 1;
                if constexpr (interval_category<T> == IC::stepwise || interval_category<T> == IC::integral) {
                    if (type == BT::exact && b.type == BT::exact) {
                        if (value < b.value) {
                            T t = value;
                            return ++t == b.value;
                        } else if (b.value < value) {
                            T t = b.value;
                            return ++t == value;
                        }
                    }
                }
                return false;
            }

            template <typename T>
            int Boundary<T>::compare(const Boundary& b) const noexcept {
                if (has_value() && b.has_value() && value != b.value)
                    return value < b.value ? -1 : 1;
                else
                    return type < b.type ? -1 : type > b.type ? 1 : 0;
            }

            template <typename T>
            bool Boundary<T>::has_value() const noexcept {
                using BT = BoundaryType;
                return type >= BT::minus_epsilon && type <= BT::plus_epsilon;
            }

            template <typename T>
            std::string Boundary<T>::str() const {
                using namespace RS::Format;
                using BT = BoundaryType;
                switch (type) {
                    case BT::null:            return "{}";
                    case BT::minus_infinity:  return "-inf";
                    case BT::plus_infinity:   return "+inf";
                    default:                  break;
                }
                auto s = RS::Format::format_object(value);
                switch (type) {
                    case BT::minus_epsilon:  s += "-eps"; break;
                    case BT::plus_epsilon:   s += "+eps"; break;
                    default:                 break;
                }
                return s;
            }

            template <typename T>
            Boundary<T> Boundary<T>::operator-() const {
                using BT = BoundaryType;
                if (type == BT::null)
                    return {};
                else
                    return {- value, BT(- int(type))};
            }

            template <typename T>
            Boundary<T> Boundary<T>::operator+(const Boundary& b) const {
                // We will only be adding like to like (lower or upper
                // bounds), so the combinations minus_epsilon+plus_epsilon and
                // minus_infinity+plus_infinity will never happen.
                using BT = BoundaryType;
                if (type == BT::null || b.type == BT::null)
                    return {};
                else if (! has_value())
                    return *this;
                else if (! b.has_value())
                    return b;
                T sum = value + b.value;
                if (type == BT::exact)
                    return {sum, b.type};
                else if (b.type == BT::exact)
                    return {sum, type};
                else
                    return {sum, type};
            }

            // TODO
            // template <typename T>
            // Boundary<T> Boundary<T>::operator*(const Boundary& b) const {
            // }

            template <typename T>
            std::ostream& operator<<(std::ostream& out, const Boundary<T>& b) {
                return out << b.str();
            }

    }

}
