#pragma once

#include "rs-interval/interval-category-base.hpp"
#include "rs-interval/interval-type-base.hpp"
#include "rs-interval/types.hpp"
#include "rs-format/format.hpp"
#include <algorithm>

namespace RS::Intervals {

    template <typename T> class Interval;

    namespace Detail {

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
            IB lbound, rbound;
            switch (l.type) {
                case BT::empty:   lbound = IB::empty; break;
                case BT::open:    lbound = IB::open; break;
                case BT::closed:  lbound = IB::closed; break;
                default:          lbound = IB::unbound; break;
            }
            switch (r.type) {
                case BT::empty:   rbound = IB::empty; break;
                case BT::open:    rbound = IB::open; break;
                case BT::closed:  rbound = IB::closed; break;
                default:          rbound = IB::unbound; break;
            }
            return {l.value, r.value, lbound, rbound};
        }

    }

    // Base class for arithmetic type intervals

    template <typename IntervalType, typename T, IntervalCategory Cat = interval_category<T>>
    class IntervalArithmeticBase {

    public:

        IntervalType operator+() const { auto& a = static_cast<const IntervalType&>(*this); return a; }
        IntervalType operator-() const { auto& a = static_cast<const IntervalType&>(*this); return negative_interval(a); }
        IntervalType& operator+=(const IntervalType& b) { auto& a = static_cast<IntervalType&>(*this); a = add_intervals(a, b); return a; }
        IntervalType& operator+=(const T& b) { auto& a = static_cast<IntervalType&>(*this); a = add_intervals(a, b); return a; }
        IntervalType& operator-=(const IntervalType& b) { auto& a = static_cast<IntervalType&>(*this); a = subtract_intervals(a, b); return a; }
        IntervalType& operator-=(const T& b) { auto& a = static_cast<IntervalType&>(*this); a = subtract_intervals(a, b); return a; }
        IntervalType& operator*=(const IntervalType& b) { auto& a = static_cast<IntervalType&>(*this); a = multiply_intervals(a, b); return a; }
        IntervalType& operator*=(const T& b) { auto& a = static_cast<IntervalType&>(*this); a = multiply_intervals(a, b); return a; }
        IntervalType& operator/=(const IntervalType& b) { auto& a = static_cast<IntervalType&>(*this); a = divide_intervals(a, b); return a; }
        IntervalType& operator/=(const T& b) { auto& a = static_cast<IntervalType&>(*this); a = divide_intervals(a, b); return a; }

        friend IntervalType operator+(const IntervalType& a, const IntervalType& b) { return add_intervals(a, b); }
        friend IntervalType operator+(const IntervalType& a, const T& b) { return add_intervals(a, b); }
        friend IntervalType operator+(const T& a, const IntervalType& b) { return add_intervals(b, a); }
        friend IntervalType operator-(const IntervalType& a, const IntervalType& b) { return subtract_intervals(a, b); }
        friend IntervalType operator-(const IntervalType& a, const T& b) { return subtract_intervals(a, b); }
        friend IntervalType operator-(const T& a, const IntervalType& b) { return subtract_intervals(a, b); }
        friend IntervalType operator*(const IntervalType& a, const IntervalType& b) { return multiply_intervals(a, b); }
        friend IntervalType operator*(const IntervalType& a, const T& b) { return multiply_intervals(a, b); }
        friend IntervalType operator*(const T& a, const IntervalType& b) { return multiply_intervals(b, a); }
        friend IntervalType operator/(const IntervalType& a, const IntervalType& b) { return divide_intervals(a, b); }
        friend IntervalType operator/(const IntervalType& a, const T& b) { return divide_intervals(a, b); }
        friend IntervalType operator/(const T& a, const IntervalType& b) { return divide_intervals(a, b); }

    private:

        static IntervalType negative_interval(const IntervalType& a);
        static IntervalType add_intervals(const IntervalType& a, const IntervalType& b);
        static IntervalType subtract_intervals(const IntervalType& a, const IntervalType& b);
        static IntervalType multiply_intervals(const IntervalType& a, const IntervalType& b);
        static IntervalType divide_intervals(const IntervalType& a, const IntervalType& b);

    };

        template <typename IntervalType, typename T, IntervalCategory Cat>
        IntervalType IntervalArithmeticBase<IntervalType, T, Cat>::negative_interval(const IntervalType& a) {
            return IntervalType(- a.max(), - a.min(), a.right(), a.left());
        }

        template <typename IntervalType, typename T, IntervalCategory Cat>
        IntervalType IntervalArithmeticBase<IntervalType, T, Cat>::add_intervals(const IntervalType& a, const IntervalType& b) {
            auto l = left_boundary_of(a) + left_boundary_of(b);
            auto r = right_boundary_of(a) + right_boundary_of(b);
            return interval_from_boundaries(l, r);
        }

        template <typename IntervalType, typename T, IntervalCategory Cat>
        IntervalType IntervalArithmeticBase<IntervalType, T, Cat>::subtract_intervals(const IntervalType& a, const IntervalType& b) {
            return add_intervals(a, negative_interval(b));
        }

        template <typename IntervalType, typename T, IntervalCategory Cat>
        IntervalType IntervalArithmeticBase<IntervalType, T, Cat>::multiply_intervals(const IntervalType& a, const IntervalType& b) {

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

            IntervalType result = interval_from_boundaries(*i, *j);

            return result;

        }

        // template <typename IntervalType, typename T, IntervalCategory Cat>
        // IntervalType IntervalArithmeticBase<IntervalType, T, Cat>::divide_intervals(const IntervalType& a, const IntervalType& b) {
        //     // TODO
        //     (void)a;
        //     (void)b;
        //     return {};
        // }

    template <typename IntervalType, typename T>
    class IntervalArithmeticBase<IntervalType, T, IntervalCategory::ordered> {};

    template <typename IntervalType, typename T>
    class IntervalArithmeticBase<IntervalType, T, IntervalCategory::stepwise> {};

}
