// This header is private to the implementation and should not be included by users

#pragma once

#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <istream>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#define RS_INTERVAL_ENUM(EnumType, IntType, first_value, first_name, ...) \
    enum class EnumType: IntType { \
        first_name = first_value, \
        __VA_ARGS__ \
    }; \
    [[maybe_unused]] inline const std::vector<std::string>& list_enum_names(EnumType) { \
        static const auto names = [] { \
            std::string all = # first_name "," # __VA_ARGS__; \
            std::vector<std::string> vec; \
            std::size_t i = 0, j = 0; \
            while (j < all.size()) { \
                i = all.find_first_not_of(" ,", j); \
                if (i == std::string::npos) { \
                    break; \
                } \
                j = all.find_first_of(" ,", i); \
                vec.push_back(all.substr(i, j - i)); \
            } \
            return vec; \
        }(); \
        return names; \
    } \
    [[maybe_unused]] inline std::string to_string(EnumType t) { \
        auto& names = list_enum_names(EnumType()); \
        auto index = std::uintmax_t(t) - std::uintmax_t(first_value); \
        if (index < names.size()) { \
            return names[index]; \
        } else { \
            return std::to_string(static_cast<IntType>(t)); \
        } \
    } \
    [[maybe_unused]] void rs_interval_enum(EnumType);

namespace RS::Interval {

    namespace Detail {

        template <typename T>
        concept LocalEnumClass = requires (T t) { rs_interval_enum(t); };

        template <typename T>
        concept ArithmeticOperators = requires (T t) {
            { t + t } -> std::convertible_to<T>;
            { t - t } -> std::convertible_to<T>;
            { t * t } -> std::convertible_to<T>;
            { t / t } -> std::convertible_to<T>;
        };

        template <typename T>
        concept StepwiseOperators = requires (T t) {
            { ++t } -> std::convertible_to<T>;
            { --t } -> std::convertible_to<T>;
            { t++ } -> std::convertible_to<T>;
            { t-- } -> std::convertible_to<T>;
        };

        template <typename T>
        concept Hashable = requires (T t) {
            { std::hash<std::remove_cvref_t<T>>()(t) } -> std::convertible_to<std::size_t>;
        };

        template <std::constructible_from<std::string> T>
        T from_string(const std::string& s) {
            return static_cast<T>(s);
        }

        template <typename T>
        requires (! std::constructible_from<T, std::string>
            && requires (T& t, std::istream& in) { { in >> t }; })
        T from_string(const std::string& s) {
            T t;
            std::istringstream in(s);
            in >> t;
            return t;
        }

    }

    constexpr auto npos = static_cast<std::size_t>(-1);

    RS_INTERVAL_ENUM(Bound, unsigned char, 0,
        empty,    // The interval is empty
        closed,   // The interval includes the boundary value
        open,     // The interval does not include the boundary value
        unbound,  // The interval is unbounded in this direction
    )

    constexpr Bound operator~(Bound b) noexcept { return static_cast<Bound>(3 - static_cast<int>(b)); }

    RS_INTERVAL_ENUM(Category, unsigned char, 0,
        none,        // Not usable in an interval
        continuous,  // Models a continuous arithmetic type (e.g. floating point)
        integral,    // Integer arithmetic operations (e.g. integer)
        ordered,     // Ordered but not an arithmetic type (e.g. string)
        stepwise,    // Incrementable and decrementable (e.g. pointer)
    )

    RS_INTERVAL_ENUM(Match, signed char, -1,
        low,    // The value is less than the interval's lower bound
        ok,     // The value is an element of the interval
        high,   // The value is greater than the interval's upper bound
        empty,  // The interval is empty
    )

    RS_INTERVAL_ENUM(Order, signed char, -7,
        // Name             Index  Picture    Description
        b_only,             // -7  BBB        A is empty, B is not
        a_below_b,          // -6  AAA...BBB  Upper bound of A is less than lower bound of B, with a gap
        a_touches_b,        // -5  AAABBB     Upper bound of A is less than lower bound of B, with no gap
        a_overlaps_b,       // -4  AAA***BBB  Upper bound of A overlaps lower bound of B
        a_extends_below_b,  // -3  AAA***     B is a subset of A, with the same upper bound
        a_encloses_b,       // -2  AAA***AAA  B is a subset of A, matching neither bound
        b_extends_above_a,  // -1  ***BBB     A is a subset of B, with the same lower bound
        equal,              // 0   ***        A and B are the same (including both empty)
        a_extends_above_b,  // 1   ***AAA     B is a subset of A, with the same lower bound
        b_encloses_a,       // 2   BBB***BBB  A is a subset of B, matching neither bound
        b_extends_below_a,  // 3   BBB***     A is a subset of B, with the same upper bound
        b_overlaps_a,       // 4   BBB***AAA  Upper bound of B overlaps lower bound of A
        b_touches_a,        // 5   BBBAAA     Upper bound of B is less than lower bound of A, with no gap
        b_below_a,          // 6   BBB...AAA  Upper bound of B is less than lower bound of A, with a gap
        a_only,             // 7   AAA        B is empty, A is not
    )

    template <typename T>
    struct IntervalTraits {

        using U = std::remove_cv_t<T>;

        static constexpr Category category =
            std::same_as<U, bool> || ! std::regular<U> || ! std::totally_ordered<U> ?
                Category::none :
            std::floating_point<U> ?
                Category::continuous :
            std::numeric_limits<U>::is_specialized && ! std::numeric_limits<U>::is_integer ?
                Category::continuous :
            std::integral<U> ?
                Category::integral :
            std::numeric_limits<U>::is_specialized && std::numeric_limits<U>::is_integer ?
                Category::integral :
            Detail::ArithmeticOperators<U> && Detail::StepwiseOperators<U> ?
                Category::integral :
            Detail::ArithmeticOperators<U> ?
                Category::continuous :
            Detail::StepwiseOperators<U> ?
                Category::stepwise :
                Category::ordered;

    };

    template <typename T> constexpr auto interval_category = IntervalTraits<T>::category;
    template <typename T> concept Continuous = interval_category<T> == Category::continuous;
    template <typename T> concept Integral = interval_category<T> == Category::integral;
    template <typename T> concept Ordered = interval_category<T> == Category::ordered;
    template <typename T> concept Stepwise = interval_category<T> == Category::stepwise;
    template <typename T> concept Arithmetic = Continuous<T> || Integral<T>;
    template <typename T> concept IntervalCompatible = Continuous<T> || Integral<T> || Ordered<T> || Stepwise<T>;

    namespace Detail {

        RS_INTERVAL_ENUM(BoundaryType, int, -3,
            empty,
            minus_infinity,
            closed,
            open,
            plus_infinity,
        )

        template <IntervalCompatible T>
        struct Boundary {

            T value {};
            BoundaryType type = BoundaryType::empty;

            bool adjacent(const Boundary& b) const noexcept;
            bool compare_ll(const Boundary& b) const noexcept;  // Compare left bounds (closed<open)
            bool compare_rr(const Boundary& b) const noexcept;  // Compare right bounds (open<closed)
            bool compare_lr(const Boundary& b) const noexcept;  // Compare left bound with right
            bool compare_rl(const Boundary& b) const noexcept;  // Compare right bound with left

            bool has_value() const noexcept {
                return type == BoundaryType::closed || type == BoundaryType::open;
            }

            Boundary<T> operator-() const;
            Boundary<T> operator+(const Boundary<T>& b) const;
            Boundary<T> operator-(const Boundary<T>& b) const { return *this + - b; }
            Boundary<T> operator*(const Boundary<T>& b) const;
            bool operator==(const Boundary<T>& b) const noexcept;
            bool operator!=(const Boundary<T>& b) const noexcept { return ! (*this == b); }

        };

            template <IntervalCompatible T>
            bool Boundary<T>::adjacent(const Boundary& b) const noexcept {

                if (! has_value() || ! b.has_value()) {
                    return false;
                } else if (type == BoundaryType::open && b.type == BoundaryType::open) {
                    return false;
                } else if (value == b.value) {
                    return type != b.type;
                }

                if constexpr (interval_category<T> == Category::stepwise
                        || interval_category<T> == Category::integral) {
                    if (type == BoundaryType::closed && b.type == BoundaryType::closed) {
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

            template <IntervalCompatible T>
            bool Boundary<T>::compare_ll(const Boundary& b) const noexcept {
                if (has_value() && b.has_value() && value != b.value) {
                    return value < b.value;
                } else {
                    return type < b.type;
                }
            }

            template <IntervalCompatible T>
            bool Boundary<T>::compare_rr(const Boundary& b) const noexcept {
                if (! has_value() || ! b.has_value()) {
                    return type < b.type;
                } else if (value != b.value) {
                    return value < b.value;
                } else {
                    return type > b.type;
                }
            }

            template <IntervalCompatible T>
            bool Boundary<T>::compare_lr(const Boundary& b) const noexcept {
                if (! has_value() || ! b.has_value()) {
                    return type < b.type;
                } else if (value != b.value) {
                    return value < b.value;
                } else {
                    return false;
                }
            }

            template <IntervalCompatible T>
            bool Boundary<T>::compare_rl(const Boundary& b) const noexcept {
                if (! has_value() || ! b.has_value()) {
                    return type < b.type;
                } else if (value != b.value) {
                    return value < b.value;
                } else {
                    return type == BoundaryType::open || b.type == BoundaryType::open;
                }
            }

            template <IntervalCompatible T>
            Boundary<T> Boundary<T>::operator-() const {

                auto b = *this;

                if (has_value()) {
                    b.value = - b.value;
                }

                if (type == BoundaryType::minus_infinity) {
                    b.type = BoundaryType::plus_infinity;
                } else if (type == BoundaryType::plus_infinity) {
                    b.type = BoundaryType::minus_infinity;
                }

                return b;

            }

            template <IntervalCompatible T>
            Boundary<T> Boundary<T>::operator+(const Boundary<T>& b) const {

                // We will only be adding like to like (lower or upper bounds),
                // so minus_infinity+plus_infinity will never happen.

                if (type == BoundaryType::empty || b.type == BoundaryType::empty) {
                    return {};
                } else if (! has_value()) {
                    return *this;
                } else if (! b.has_value()) {
                    return b;
                }

                Boundary<T> c {value + b.value, BoundaryType::open};

                if (type == BoundaryType::closed && b.type == BoundaryType::closed) {
                    c.type = BoundaryType::closed;
                }

                return c;

            }

            template <IntervalCompatible T>
            Boundary<T> Boundary<T>::operator*(const Boundary& b) const {

                // If either argument is empty, the result is empty
                if (type == BoundaryType::empty || b.type == BoundaryType::empty) {
                    return {};
                }

                // Use symmetry to handle negative arguments
                bool a_minus = type == BoundaryType::minus_infinity || (has_value() && value < T{});
                bool b_minus = b.type == BoundaryType::minus_infinity || (b.has_value() && b.value < T{});
                if (a_minus && b_minus) {
                    return - *this * - b;
                } else if (a_minus) {
                    return - (- *this * b);
                } else if (b_minus) {
                    return - (*this * - b);
                }

                // Use symmetry to ensure a>=b
                if ((type == BoundaryType::minus_infinity && b.type != BoundaryType::minus_infinity)
                        || (b.type == BoundaryType::plus_infinity && type != BoundaryType::plus_infinity)
                        || (has_value() && b.has_value() && value < b.value)) {
                    return b * *this;
                }

                // If either argument is a closed zero, the result is a closed zero
                if ((type == BoundaryType::closed && value == T{})
                        || (b.type == BoundaryType::closed && b.value == T{})) {
                    return {{}, BoundaryType::closed};
                }

                // If either argument is an open zero, the result is an open zero
                if ((type == BoundaryType::open && value == T{})
                        || (b.type == BoundaryType::open && b.value == T{})) {
                    return {{}, BoundaryType::open};
                }

                // If either argument is positive infinity, the result is positive infinity
                if (type == BoundaryType::plus_infinity) {
                    return *this;
                }

                // If both arguments are closed, the result is closed; otherwise, the result is open
                BoundaryType t = type == BoundaryType::closed && b.type == BoundaryType::closed ?
                    BoundaryType::closed : BoundaryType::open;
                return {value * b.value, t};

            }

            template <IntervalCompatible T>
            bool Boundary<T>::operator==(const Boundary<T>& b) const noexcept {
                return type == b.type && (! has_value() || value == b.value);
            }

    }

}

template <RS::Interval::Detail::LocalEnumClass T>
struct std::formatter<T> {

    std::formatter<std::string> string_formatter;

    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const T& t, FormatContext& ctx) const {
        return string_formatter.format(to_string(t), ctx);
    }

};

template <RS::Interval::IntervalCompatible T>
requires (std::formattable<T, char>)
struct std::formatter<RS::Interval::Detail::Boundary<T>> {

    std::formatter<std::string> string_formatter;

    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const RS::Interval::Detail::Boundary<T>& b, FormatContext& ctx) const {
        using namespace RS::Interval::Detail;
        std::string s;
        switch (b.type) {
            case BoundaryType::empty:           s = "{}"; break;
            case BoundaryType::minus_infinity:  s = "-inf"; break;
            case BoundaryType::closed:          s = std::format("{}", b.value); break;
            case BoundaryType::open:            s = std::format("({})", b.value); break;
            case BoundaryType::plus_infinity:   s = "+inf"; break;
        }
        return string_formatter.format(s, ctx);
    }

};
