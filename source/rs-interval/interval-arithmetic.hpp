#pragma once

#include "rs-interval/interval-category-base.hpp"
#include "rs-interval/interval-type-base.hpp"
#include "rs-interval/types.hpp"
#include <algorithm>
#include <type_traits>

namespace RS::Intervals {

    template <typename T> class Interval;
    template <typename T> class IntervalSet;

    namespace Detail {

        template <typename T> constexpr bool is_arithmetic_interval = interval_category<T> == IntervalCategory::integral
            || interval_category<T> == IntervalCategory::continuous;

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

        template <typename T>
        Interval<T> reciprocal_interval(const Interval<T>& i) {
            using IB = IntervalBound;
            if (i.empty())
                return {};
            T lvalue = {};
            T rvalue = {};
            IB lbound, rbound;
            if (i.left() == IB::unbound) {
                rbound = IB::open;
            } else if (i.min() == T()) {
                rbound = IB::unbound;
            } else {
                rvalue = T(1) / i.min();
                rbound = i.left();
            }
            if (i.right() == IB::unbound) {
                lbound = IB::open;
            } else if (i.max() == T()) {
                lbound = IB::unbound;
            } else {
                lvalue = T(1) / i.max();
                lbound = i.right();
            }
            return {lvalue, rvalue, lbound, rbound};
        }

        template <typename T>
        IntervalSet<T> reciprocal_set(const Interval<T>& i) {
            using IB = IntervalBound;
            if (i.empty()) {
                return {};
            } else if (contains_zero(i)) {
                Interval<T> negative_part(i.min(), T(), i.left(), IB::open);
                Interval<T> positive_part(T(), i.max(), IB::open, i.right());
                auto negative_reciprocal = reciprocal_interval(negative_part);
                auto positive_reciprocal = reciprocal_interval(positive_part);
                return {negative_reciprocal, positive_reciprocal};
            } else {
                return reciprocal_interval(i);
            }
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
        if (a.empty() || b.empty())
            return {};
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

        if (a.empty() || b.empty())
            return {};

        static const B zero = {T(), BT::closed};

        B al = left_boundary_of(a);
        B ar = right_boundary_of(a);
        B bl = left_boundary_of(b);
        B br = right_boundary_of(b);

        CappedVector<B, 9> boundaries;

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
    std::enable_if_t<interval_category<T> == IntervalCategory::continuous, IntervalSet<T>>
    operator/(const Interval<T>& a, const Interval<T>& b) {
        if (a.empty() || b.empty())
            return {};
        auto b_reciprocals = Detail::reciprocal_set(b);
        IntervalSet<T> set;
        for (auto& br: b_reciprocals)
            set.insert(a * br);
        return set;
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
