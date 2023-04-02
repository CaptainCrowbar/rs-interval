// This header is private to the implementation and should not be included by users

#pragma once

#include "rs-intervals/category-base-class.hpp"
#include "rs-intervals/interval-base-class.hpp"
#include "rs-intervals/types.hpp"
#include <algorithm>
#include <array>

namespace RS::Intervals {

    template <IntervalCompatible T> class Interval;
    template <IntervalCompatible T> class IntervalSet;

    namespace Detail {

        template <Arithmetic T>
        bool contains_zero(const Interval<T>& i) noexcept {
            using Bound = Bound;
            if (i.empty())
                return false;
            bool right_zero = i.right() == Bound::unbound
                || (i.right() == Bound::closed && i.max() >= T())
                || (i.right() == Bound::open && i.max() > T());
            if (! right_zero)
                return false;
            bool left_zero = i.left() == Bound::unbound
                || (i.left() == Bound::closed && i.min() <= T())
                || (i.left() == Bound::open && i.min() < T());
            return left_zero;
        }

        template <IntervalCompatible T>
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

        template <IntervalCompatible T>
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

        template <Arithmetic T>
        Interval<T> interval_from_boundaries(const Boundary<T>& l, const Boundary<T>& r) {
            using BT = BoundaryType;
            using Bound = Bound;
            static constexpr auto convert_bound = [] (BT t) constexpr {
                switch (t) {
                    case BT::empty:   return Bound::empty;
                    case BT::open:    return Bound::open;
                    case BT::closed:  return Bound::closed;
                    default:          return Bound::unbound;
                }
            };
            Bound lbound = convert_bound(l.type);
            Bound rbound = convert_bound(r.type);
            return {l.value, r.value, lbound, rbound};
        }

        template <Continuous T>
        Interval<T> reciprocal_interval(const Interval<T>& i) {
            using Bound = Bound;
            if (i.empty())
                return {};
            T lvalue = {};
            T rvalue = {};
            Bound lbound, rbound;
            if (i.left() == Bound::unbound) {
                rbound = Bound::open;
            } else if (i.min() == T()) {
                rbound = Bound::unbound;
            } else {
                rvalue = T(1) / i.min();
                rbound = i.left();
            }
            if (i.right() == Bound::unbound) {
                lbound = Bound::open;
            } else if (i.max() == T()) {
                lbound = Bound::unbound;
            } else {
                lvalue = T(1) / i.max();
                lbound = i.right();
            }
            return {lvalue, rvalue, lbound, rbound};
        }

        template <Continuous T>
        IntervalSet<T> reciprocal_set(const Interval<T>& i) {
            using Bound = Bound;
            if (i.empty()) {
                return {};
            } else if (contains_zero(i)) {
                Interval<T> negative_part(i.min(), T(), i.left(), Bound::open);
                Interval<T> positive_part(T(), i.max(), Bound::open, i.right());
                auto negative_reciprocal = reciprocal_interval(negative_part);
                auto positive_reciprocal = reciprocal_interval(positive_part);
                return {negative_reciprocal, positive_reciprocal};
            } else {
                return reciprocal_interval(i);
            }
        }

    }

    // Interval arithmetic operators

    template <Arithmetic T>
    Interval<T> operator+(const Interval<T>& i) {
        return i;
    }

    template <Arithmetic T>
    Interval<T> operator-(const Interval<T>& i) {
        return Interval<T>(- i.max(), - i.min(), i.right(), i.left());
    }

    template <Arithmetic T>
    Interval<T> operator+(const Interval<T>& a, const Interval<T>& b) {
        using namespace Detail;
        if (a.empty() || b.empty())
            return {};
        auto l = left_boundary_of(a) + left_boundary_of(b);
        auto r = right_boundary_of(a) + right_boundary_of(b);
        return interval_from_boundaries(l, r);
    }

    template <Arithmetic T>
    Interval<T> operator-(const Interval<T>& a, const Interval<T>& b) {
        return a + - b;
    }

    template <Arithmetic T>
    Interval<T> operator*(const Interval<T>& a, const Interval<T>& b) {

        using namespace Detail;

        using B = Boundary<T>;

        if (a.empty() || b.empty())
            return {};

        B al = left_boundary_of(a);
        B ar = right_boundary_of(a);
        B bl = left_boundary_of(b);
        B br = right_boundary_of(b);

        std::array<B, 4> boundaries = {{
            al * bl,
            al * br,
            ar * bl,
            ar * br,
        }};

        auto i = std::min_element(boundaries.begin(), boundaries.end(),
            [] (auto& a, auto& b) { return a.compare_ll(b); });
        auto j = std::max_element(boundaries.begin(), boundaries.end(),
            [] (auto& a, auto& b) { return a.compare_rr(b); });

        return interval_from_boundaries(*i, *j);

    }

    template <Continuous T>
    IntervalSet<T> operator/(const Interval<T>& a, const Interval<T>& b) {
        using namespace Detail;
        if (a.empty() || b.empty())
            return {};
        auto b_reciprocals = reciprocal_set(b);
        IntervalSet<T> set;
        for (auto& br: b_reciprocals)
            set.insert(a * br);
        return set;
    }

    template <Arithmetic T>
    Interval<T> operator+(const Interval<T>& a, const T& b) {
        return a + Interval<T>(b);
    }

    template <Arithmetic T>
    Interval<T> operator-(const Interval<T>& a, const T& b) {
        return a - Interval<T>(b);
    }

    template <Arithmetic T>
    Interval<T> operator*(const Interval<T>& a, const T& b) {
        return a * Interval<T>(b);
    }

    template <Continuous T>
    IntervalSet<T> operator/(const Interval<T>& a, const T& b) {
        return a / Interval<T>(b);
    }

    template <Arithmetic T>
    Interval<T> operator+(const T& a, const Interval<T>& b) {
        return Interval<T>(a) + b;
    }

    template <Arithmetic T>
    Interval<T> operator-(const T& a, const Interval<T>& b) {
        return Interval<T>(a) - b;
    }

    template <Arithmetic T>
    Interval<T> operator*(const T& a, const Interval<T>& b) {
        return Interval<T>(a) * b;
    }

    template <Continuous T>
    IntervalSet<T> operator/(const T& a, const Interval<T>& b) {
        return Interval<T>(a) / b;
    }

    template <Arithmetic T>
    Interval<T>& operator+=(Interval<T>& a, const Interval<T>& b) {
        return a = a + b;
    }

    template <Arithmetic T>
    Interval<T>& operator-=(Interval<T>& a, const Interval<T>& b) {
        return a = a - b;
    }

    template <Arithmetic T>
    Interval<T>& operator*=(Interval<T>& a, const Interval<T>& b) {
        return a = a * b;
    }

    template <Arithmetic T>
    Interval<T>& operator+=(Interval<T>& a, const T& b) {
        return a = a + Interval<T>(b);
    }

    template <Arithmetic T>
    Interval<T>& operator-=(Interval<T>& a, const T& b) {
        return a = a - Interval<T>(b);
    }

    template <Arithmetic T>
    Interval<T>& operator*=(Interval<T>& a, const T& b) {
        return a = a * Interval<T>(b);
    }

}
