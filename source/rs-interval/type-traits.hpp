#pragma once

#include <iterator>
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

#define RS_INTERVAL_ENUM_IMPL_(EnumType, enum_class, IntType, first_value, first_name, ...) \
    enum enum_class EnumType: IntType { \
        first_name = first_value, \
        __VA_ARGS__ \
    }; \
    inline std::string to_string(EnumType t) { \
        static const auto names = ::RS::Intervals::Detail::split_string(# first_name "," # __VA_ARGS__, " ,"); \
        IntType index = static_cast<IntType>(t) - static_cast<IntType>(first_value); \
        if (index >= 0 && index < static_cast<IntType>(names.size())) \
            return names[size_t(index)]; \
        else \
            return std::to_string(static_cast<IntType>(t)); \
    } \
    inline std::ostream& operator<<(std::ostream& out, EnumType t) { \
        return out << to_string(t); \
    }
#define RS_INTERVAL_ENUM(EnumType, IntType, first_value, first_name, ...) \
    RS_INTERVAL_ENUM_IMPL_(EnumType,, IntType, first_value, first_name, __VA_ARGS__)
#define RS_INTERVAL_ENUM_CLASS(EnumType, IntType, first_value, first_name, ...) \
    RS_INTERVAL_ENUM_IMPL_(EnumType, class, IntType, first_value, first_name, __VA_ARGS__)

namespace RS::Intervals {

    constexpr const char* ascii_whitespace = "\t\n\f\r ";
    constexpr size_t npos = std::string::npos;

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
        struct TotalOrder {
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

        template <typename T, bool UseAdl = HasAdlBeginFunction<T>::value, bool UseStd = HasStdBeginFunction<T>::value> struct RangeValueType
            { using type = void; };
        template <typename T, bool UseStd> struct RangeValueType<T, true, UseStd>
            { using type = std::decay_t<decltype(*begin(std::declval<T>()))>; };
        template <typename T> struct RangeValueType<T, false, true>
            { using type = std::decay_t<decltype(*std::begin(std::declval<T>()))>; };

        template <typename T, typename = void> struct HasFirstMember: std::false_type {};
        template <typename T> struct HasFirstMember<T,
            std::void_t<decltype(std::declval<T>().first)>>: std::true_type {};
        template <typename T, typename = void> struct HasSecondMember: std::false_type {};
        template <typename T> struct HasSecondMember<T,
            std::void_t<decltype(std::declval<T>().second)>>: std::true_type {};
        template <typename T> constexpr bool is_pairlike = HasFirstMember<T>::value && HasSecondMember<T>::value;

        template <typename T> constexpr bool is_maplike = is_range<T> && is_pairlike<typename RangeValueType<T>::type>;

        template <typename T>
        inline int compare_3way(const T& a, const T& b) noexcept {
            return a == b ? 0 : a < b ? -1 : 1;
        }

        inline std::vector<std::string> split_string(const std::string& str, const std::string& chars = ascii_whitespace) {
            std::vector<std::string> vec;
            size_t i = 0, j = 0;
            while (j < str.size()) {
                i = str.find_first_not_of(chars, j);
                if (i == npos)
                    break;
                j = str.find_first_of(chars, i);
                vec.push_back(str.substr(i, j - i));
            }
            return vec;
        }

    }

    RS_INTERVAL_ENUM_CLASS(IntervalBound, unsigned char, 0,
        empty,   // The interval is empty
        closed,  // The interval includes the boundary value
        open,    // The interval does not include the boundary value
        unbound  // The interval is unbounded in this direction
    )

    constexpr IntervalBound operator~(IntervalBound b) noexcept { return IntervalBound(3 - int(b)); }

    RS_INTERVAL_ENUM_CLASS(IntervalCategory, int, 0,
        none,       // Not usable in an interval
        ordered,    // Ordered but not an arithmetic type (e.g. string)
        stepwise,   // Incrementable and decrementable (e.g. pointer)
        integral,   // Integer arithmetic operations (e.g. integer)
        continuous  // Models a continuous arithmetic type (e.g. floating point)
    )

    RS_INTERVAL_ENUM_CLASS(IntervalMatch, int, -1,
        low,    // The value is less than the interval's lower bound
        match,  // The value is an element of the interval
        high,   // The value is greater than the interval's upper bound
        empty   // The interval is empty
    )

    RS_INTERVAL_ENUM_CLASS(IntervalOrder, int, -7,
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

        template <typename T>
        struct Boundary:
        public TotalOrder<Boundary<T>> {
            T value;
            IntervalBound bound;
            bool upper;
            Boundary() = default;
            Boundary(T v, IntervalBound b, bool u): value(v), bound(b), upper(u) {}
            bool operator==(const Boundary& r) const noexcept { return compare(*this, r) == 0; }
            bool operator<(const Boundary& r) const noexcept { return compare(*this, r) == -1; }
            static bool adjacent(const Boundary& l, const Boundary& r) noexcept;
            static int compare(const Boundary& l, const Boundary& r) noexcept;
        };

            template <typename T>
            bool Boundary<T>::adjacent(const Boundary& l, const Boundary& r) noexcept {
                using IB = IntervalBound;
                using IC = IntervalCategory;
                if (l.bound == IB::empty || l.bound == IB::unbound || r.bound == IB::empty || r.bound == IB::unbound)
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

    }

}
