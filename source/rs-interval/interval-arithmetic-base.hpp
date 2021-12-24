#pragma once

#include "rs-interval/interval-category-base.hpp"
#include "rs-interval/interval-type-base.hpp"
#include "rs-interval/types.hpp"
#include <algorithm>
#include <type_traits>

namespace RS::Intervals {

    template <typename T> class Interval;

    namespace Detail {

        template <typename T, IntervalCategory Cat = interval_category<T>> struct IsArithmeticInterval: std::false_type {};
        template <typename T> struct IsArithmeticInterval<T, IntervalCategory::integral>: std::true_type {};
        template <typename T> struct IsArithmeticInterval<T, IntervalCategory::continuous>: std::true_type {};
        template <typename T> constexpr bool is_arithmetic_interval = IsArithmeticInterval<T>::value;

        template <typename T>
        bool contains_zero(const Interval<T>& i) noexcept {
            using IB = IntervalBound;
            if (i.empty())
                return false;
            bool right_zero = i.right() == IB::unbound
                || (i.right() == IB::closed && i.max() >= T())
                || (i.right() == IB::open && i.max() > T());
            if (! right_zero)
                return false;
            bool left_zero = i.left() == IB::unbound
                || (i.left() == IB::closed && i.min() <= T())
                || (i.left() == IB::open && i.min() < T());
            return left_zero;
        }

        template <typename T>
        Boundary<T> left_boundary_of(const Interval<T>& i) {
            using BT = BoundaryType;
            if (i.empty())
                return {};
            else if (i.is_left_closed())
                return {i.min(), BT::closed};
            else if (i.is_left_open())
                return {i.min(), BT::open};
            else
                return {{}, BT::minus_infinity};
        }

        template <typename T>
        Boundary<T> right_boundary_of(const Interval<T>& i) {
            using BT = BoundaryType;
            if (i.empty())
                return {};
            else if (i.is_right_closed())
                return {i.max(), BT::closed};
            else if (i.is_right_open())
                return {i.max(), BT::open};
            else
                return {{}, BT::plus_infinity};
        }

        template <typename T>
        Interval<T> interval_from_boundaries(const Boundary<T>& l, const Boundary<T>& r) {
            using BT = BoundaryType;
            using IB = IntervalBound;
            static constexpr auto convert_bound = [] (BT t) constexpr {
                switch (t) {
                    case BT::empty:   return IB::empty;
                    case BT::open:    return IB::open;
                    case BT::closed:  return IB::closed;
                    default:          return IB::unbound;
                }
            };
            IB lbound = convert_bound(l.type);
            IB rbound = convert_bound(r.type);
            return {l.value, r.value, lbound, rbound};
        }

    }

    // Interval arithmetic operators

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>>
    operator+(const Interval<T>& i) {
        return i;
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>>
    operator-(const Interval<T>& i) {
        return Interval<T>(- i.max(), - i.min(), i.right(), i.left());
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>>
    operator+(const Interval<T>& a, const Interval<T>& b) {
        auto l = Detail::left_boundary_of(a) + Detail::left_boundary_of(b);
        auto r = Detail::right_boundary_of(a) + Detail::right_boundary_of(b);
        return Detail::interval_from_boundaries(l, r);
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>>
    operator-(const Interval<T>& a, const Interval<T>& b) {
        return a + - b;
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>>
    operator*(const Interval<T>& a, const Interval<T>& b) {

        using namespace RS::Intervals::Detail;

        using B = Boundary<T>;
        using BT = BoundaryType;

        static const B zero = {T(), BT::closed};

        if (a.empty() || b.empty())
            return {};

        CappedVector<B, 9> boundaries;

        B al = left_boundary_of(a);
        B ar = right_boundary_of(a);
        B bl = left_boundary_of(b);
        B br = right_boundary_of(b);

        boundaries.push_back(al * bl);
        boundaries.push_back(al * br);
        boundaries.push_back(ar * bl);
        boundaries.push_back(ar * br);

        bool a_zero = contains_zero(a);
        bool b_zero = contains_zero(b);

        if (a_zero) {
            boundaries.push_back(zero * bl);
            boundaries.push_back(zero * br);
        }

        if (b_zero) {
            boundaries.push_back(zero * al);
            boundaries.push_back(zero * ar);
        }

        if (a_zero && b_zero)
            boundaries.push_back(zero);

        auto i = std::min_element(boundaries.begin(), boundaries.end(),
            [] (auto& a, auto& b) { return a.compare_ll(b); });
        auto j = std::max_element(boundaries.begin(), boundaries.end(),
            [] (auto& a, auto& b) { return a.compare_rr(b); });

        return interval_from_boundaries(*i, *j);

    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>>
    operator/(const Interval<T>& a, const Interval<T>& b) {
        // TODO
        (void)a;
        (void)b;
        return {};
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>>
    operator+(const Interval<T>& a, const T& b) {
        return a + Interval<T>(b);
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>>
    operator-(const Interval<T>& a, const T& b) {
        return a - Interval<T>(b);
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>>
    operator*(const Interval<T>& a, const T& b) {
        return a * Interval<T>(b);
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>>
    operator/(const Interval<T>& a, const T& b) {
        return a / Interval<T>(b);
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>>
    operator+(const T& a, const Interval<T>& b) {
        return Interval<T>(a) + b;
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>>
    operator-(const T& a, const Interval<T>& b) {
        return Interval<T>(a) - b;
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>>
    operator*(const T& a, const Interval<T>& b) {
        return Interval<T>(a) * b;
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>>
    operator/(const T& a, const Interval<T>& b) {
        return Interval<T>(a) / b;
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>&>
    operator+=(Interval<T>& a, const Interval<T>& b) {
        return a = a + b;
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>&>
    operator-=(Interval<T>& a, const Interval<T>& b) {
        return a = a - b;
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>&>
    operator*=(Interval<T>& a, const Interval<T>& b) {
        return a = a * b;
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>&>
    operator/=(Interval<T>& a, const Interval<T>& b) {
        return a = a / b;
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>&>
    operator+=(Interval<T>& a, const T& b) {
        return a = a + Interval<T>(b);
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>&>
    operator-=(Interval<T>& a, const T& b) {
        return a = a - Interval<T>(b);
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>&>
    operator*=(Interval<T>& a, const T& b) {
        return a = a * Interval<T>(b);
    }

    template <typename T>
    std::enable_if_t<Detail::is_arithmetic_interval<T>, Interval<T>&>
    operator/=(Interval<T>& a, const T& b) {
        return a = a / Interval<T>(b);
    }

}
