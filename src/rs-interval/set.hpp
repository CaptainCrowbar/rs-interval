#pragma once

#include "rs-interval/interval.hpp"
#include "rs-interval/types.hpp"
#include <algorithm>
#include <compare>
#include <cstddef>
#include <format>
#include <initializer_list>
#include <iterator>
#include <set>
#include <vector>

namespace RS::Interval {

    // Interval set

    template <IntervalCompatible T>
    class IntervalSet {

    public:

        using iterator = typename std::set<Interval<T>>::const_iterator;
        using interval_type = Interval<T>;
        using value_type = T;

        static constexpr auto category = interval_category<T>;

        IntervalSet() = default;
        IntervalSet(const T& t): set_{{t}} {}
        IntervalSet(const interval_type& in) { insert(in); }
        IntervalSet(std::initializer_list<interval_type> list);

        bool operator[](const T& t) const { return contains(t); }

        auto begin() const noexcept { return set_.begin(); }
        auto end() const noexcept { return set_.end(); }
        bool empty() const noexcept { return set_.empty(); }
        std::size_t size() const noexcept { return set_.size(); }
        bool contains(const T& t) const;
        void clear() noexcept { set_.clear(); }
        void insert(const interval_type& in);
        void erase(const interval_type& in);
        void swap(IntervalSet& set) noexcept { set_.swap(set.set_); }

        IntervalSet complement() const;
        IntervalSet set_intersection(const IntervalSet& b) const;
        IntervalSet set_intersection(const interval_type& b) const;
        IntervalSet set_intersection(const T& b) const;
        IntervalSet set_union(const IntervalSet& b) const;
        IntervalSet set_union(const interval_type& b) const;
        IntervalSet set_union(const T& b) const;
        IntervalSet set_difference(const IntervalSet& b) const;
        IntervalSet set_difference(const interval_type& b) const;
        IntervalSet set_difference(const T& b) const;
        IntervalSet set_symmetric_difference(const IntervalSet& b) const;
        IntervalSet set_symmetric_difference(const interval_type& b) const;
        IntervalSet set_symmetric_difference(const T& b) const;

        IntervalSet& apply_complement();
        IntervalSet& apply_intersection(const IntervalSet& b);
        IntervalSet& apply_intersection(const interval_type& b);
        IntervalSet& apply_intersection(const T& b);
        IntervalSet& apply_union(const IntervalSet& b);
        IntervalSet& apply_union(const interval_type& b);
        IntervalSet& apply_union(const T& b);
        IntervalSet& apply_difference(const IntervalSet& b);
        IntervalSet& apply_difference(const interval_type& b);
        IntervalSet& apply_difference(const T& b);
        IntervalSet& apply_symmetric_difference(const IntervalSet& b);
        IntervalSet& apply_symmetric_difference(const interval_type& b);
        IntervalSet& apply_symmetric_difference(const T& b);

    private:

        std::set<Interval<T>> set_;

    };

        template <IntervalCompatible T>
        IntervalSet<T>::IntervalSet(std::initializer_list<interval_type> list) {
            for (const auto& in: list) {
                insert(in);
            }
        }

        template <IntervalCompatible T>
        bool IntervalSet<T>::contains(const T& t) const {

            interval_type in(t);
            auto i = set_.lower_bound(in);

            if (i != set_.begin()) {
                --i;
            }

            for (; i != set_.end(); ++i) {
                auto m = i->match(t);
                if (m == Match::ok) {
                    return true;
                } else if (m == Match::low) {
                    return false;
                }
            }

            return false;

        }

        template <IntervalCompatible T>
        void IntervalSet<T>::insert(const interval_type& in) {

            if (in.empty()) {
                return;
            }

            auto i = set_.lower_bound(in);

            if (i != set_.begin()) {
                --i;
            }

            auto add = in;

            while (i != set_.end()) {
                auto ord = in.order(*i);
                if (ord <= Order::a_below_b) {
                    break;
                }
                auto j = i++;
                if (ord <= Order::b_touches_a) {
                    add = add.envelope(*j);
                    set_.erase(j);
                }
            }

            set_.insert(add);

        }

        template <IntervalCompatible T>
        void IntervalSet<T>::erase(const interval_type& in) {

            if (empty() || in.empty()) {
                return;
            }

            auto i = set_.lower_bound(in);

            if (i != set_.begin()) {
                --i;
            }

            IntervalSet temp;
            std::vector<interval_type> vec;

            while (i != set_.end()) {
                auto ord = in.order(*i);
                if (ord <= Order::a_touches_b) {
                    break;
                }
                auto j = i++;
                if (ord <= Order::b_overlaps_a) {
                    temp = j->set_difference(in);
                    std::copy(temp.begin(), temp.end(), std::back_inserter(vec));
                    set_.erase(j);
                }
            }

            set_.insert(vec.begin(), vec.end());

        }

        template <IntervalCompatible T>
        IntervalSet<T> IntervalSet<T>::complement() const {

            if (empty()) {
                return interval_type::all();
            }

            IntervalSet result;
            auto i = set_.begin();

            if (i->is_left_bounded()) {
                result.set_.insert({{}, i->min(), Bound::unbound, ~ i->left()});
            }

            for (auto j = std::next(i), end = set_.end(); j != end; i = j++) {
                result.set_.insert({i->max(), j->min(), ~ i->right(), ~ j->left()});
            }

            if (i->is_right_bounded()) {
                result.set_.insert({i->max(), {}, ~ i->right(), Bound::unbound});
            }

            return result;

        }

        template <IntervalCompatible T>
        IntervalSet<T> IntervalSet<T>::set_intersection(const IntervalSet& b) const {
            return complement().set_union(b.complement()).complement();
        }

        template <IntervalCompatible T>
        IntervalSet<T> IntervalSet<T>::set_intersection(const interval_type& b) const {
            return set_intersection(IntervalSet{b});
        }

        template <IntervalCompatible T>
        IntervalSet<T> IntervalSet<T>::set_intersection(const T& b) const {
            return set_intersection(IntervalSet{b});
        }

        template <IntervalCompatible T>
        IntervalSet<T> IntervalSet<T>::set_union(const IntervalSet& b) const {
            auto result = *this;
            for (const auto& in: b) {
                result.insert(in);
            }
            return result;
        }

        template <IntervalCompatible T>
        IntervalSet<T> IntervalSet<T>::set_union(const interval_type& b) const {
            return set_union(IntervalSet{b});
        }

        template <IntervalCompatible T>
        IntervalSet<T> IntervalSet<T>::set_union(const T& b) const {
            return set_union(IntervalSet{b});
        }

        template <IntervalCompatible T>
        IntervalSet<T> IntervalSet<T>::set_difference(const IntervalSet& b) const {
            auto result = *this;
            for (const auto& in: b) {
                result.erase(in);
            }
            return result;
        }

        template <IntervalCompatible T>
        IntervalSet<T> IntervalSet<T>::set_difference(const interval_type& b) const {
            return set_difference(IntervalSet{b});
        }

        template <IntervalCompatible T>
        IntervalSet<T> IntervalSet<T>::set_difference(const T& b) const {
            return set_difference(IntervalSet{b});
        }

        template <IntervalCompatible T>
        IntervalSet<T> IntervalSet<T>::set_symmetric_difference(const IntervalSet& b) const {
            auto set1 = set_difference(b);
            auto set2 = b.set_difference(*this);
            return set1.set_union(set2);
        }

        template <IntervalCompatible T>
        IntervalSet<T> IntervalSet<T>::set_symmetric_difference(const interval_type& b) const {
            return set_symmetric_difference(IntervalSet{b});
        }

        template <IntervalCompatible T>
        IntervalSet<T> IntervalSet<T>::set_symmetric_difference(const T& b) const {
            return set_symmetric_difference(IntervalSet{b});
        }

        template <IntervalCompatible T>
        IntervalSet<T>& IntervalSet<T>::apply_complement() {
            auto set = complement();
            swap(set);
            return *this;
        }

        template <IntervalCompatible T>
        IntervalSet<T>& IntervalSet<T>::apply_intersection(const IntervalSet& b) {
            auto set = set_intersection(b);
            swap(set);
            return *this;
        }

        template <IntervalCompatible T>
        IntervalSet<T>& IntervalSet<T>::apply_intersection(const interval_type& b) {
            return apply_intersection(IntervalSet{b});
        }

        template <IntervalCompatible T>
        IntervalSet<T>& IntervalSet<T>::apply_intersection(const T& b) {
            return apply_intersection(IntervalSet{b});
        }

        template <IntervalCompatible T>
        IntervalSet<T>& IntervalSet<T>::apply_union(const IntervalSet& b) {
            for (const auto& in: b) {
                insert(in);
            }
            return *this;
        }

        template <IntervalCompatible T>
        IntervalSet<T>& IntervalSet<T>::apply_union(const interval_type& b) {
            return apply_union(IntervalSet{b});
        }

        template <IntervalCompatible T>
        IntervalSet<T>& IntervalSet<T>::apply_union(const T& b) {
            return apply_union(IntervalSet{b});
        }

        template <IntervalCompatible T>
        IntervalSet<T>& IntervalSet<T>::apply_difference(const IntervalSet& b) {
            for (const auto& in: b) {
                erase(in);
            }
            return *this;
        }

        template <IntervalCompatible T>
        IntervalSet<T>& IntervalSet<T>::apply_difference(const interval_type& b) {
            return apply_difference(IntervalSet{b});
        }

        template <IntervalCompatible T>
        IntervalSet<T>& IntervalSet<T>::apply_difference(const T& b) {
            return apply_difference(IntervalSet{b});
        }

        template <IntervalCompatible T>
        IntervalSet<T>& IntervalSet<T>::apply_symmetric_difference(const IntervalSet& b) {
            auto set1 = set_difference(b);
            auto set2 = b.set_difference(*this);
            auto set3 = set1.set_union(set2);
            swap(set3);
            return *this;
        }

        template <IntervalCompatible T>
        IntervalSet<T>& IntervalSet<T>::apply_symmetric_difference(const interval_type& b) {
            return apply_symmetric_difference(IntervalSet{b});
        }

        template <IntervalCompatible T>
        IntervalSet<T>& IntervalSet<T>::apply_symmetric_difference(const T& b) {
            return apply_symmetric_difference(IntervalSet{b});
        }

    template <IntervalCompatible T>
    bool operator==(const IntervalSet<T>& a, const IntervalSet<T>& b) noexcept {
        return std::equal(a.begin(), a.end(), b.begin(), b.end());
    }

    template <IntervalCompatible T>
    auto operator<=>(const IntervalSet<T>& a, const IntervalSet<T>& b) noexcept {

        auto i = a.begin();
        auto j = a.end();
        auto k = b.begin();
        auto l = b.end();

        for (; i != j && k != l; ++i, ++j) {
            auto c = *i <=> *k;
            if (c != 0) {
                return c;
            }
        }

        if (*i != *j) {
            return std::strong_ordering::greater;
        } else if (*k != *l) {
            return std::strong_ordering::less;
        } else {
            return std::strong_ordering::equal;
        }

    }

    template <IntervalCompatible T>
    void swap(IntervalSet<T>& a, IntervalSet<T>& b) noexcept {
        a.swap(b);
    }

    template <IntervalCompatible T> auto set_intersection(const IntervalSet<T>& a, const IntervalSet<T>& b) { return a.set_intersection(b); }
    template <IntervalCompatible T> auto set_intersection(const IntervalSet<T>& a, const Interval<T>& b) { return a.set_intersection(b); }
    template <IntervalCompatible T> auto set_intersection(const Interval<T>& a, const IntervalSet<T>& b) { return a.set_intersection(b); }
    template <IntervalCompatible T> auto set_intersection(const IntervalSet<T>& a, const T& b) { return IntervalSet<T>(a).set_intersection(b); }
    template <IntervalCompatible T> auto set_intersection(const T& a, const IntervalSet<T>& b) { return IntervalSet<T>(a).set_intersection(b); }
    template <IntervalCompatible T> auto set_intersection(const Interval<T>& a, const Interval<T>& b) { return IntervalSet<T>(a).set_intersection(b); }
    template <IntervalCompatible T> auto set_intersection(const Interval<T>& a, const T& b) { return IntervalSet<T>(a).set_intersection(b); }
    template <IntervalCompatible T> auto set_intersection(const T& a, const Interval<T>& b) { return IntervalSet<T>(a).set_intersection(b); }
    template <IntervalCompatible T> auto set_intersection(const T& a, const T& b) { return IntervalSet<T>(a).set_intersection(b); }

    template <IntervalCompatible T> auto set_union(const IntervalSet<T>& a, const IntervalSet<T>& b) { return a.set_union(b); }
    template <IntervalCompatible T> auto set_union(const IntervalSet<T>& a, const Interval<T>& b) { return a.set_union(b); }
    template <IntervalCompatible T> auto set_union(const Interval<T>& a, const IntervalSet<T>& b) { return a.set_union(b); }
    template <IntervalCompatible T> auto set_union(const IntervalSet<T>& a, const T& b) { return IntervalSet<T>(a).set_union(b); }
    template <IntervalCompatible T> auto set_union(const T& a, const IntervalSet<T>& b) { return IntervalSet<T>(a).set_union(b); }
    template <IntervalCompatible T> auto set_union(const Interval<T>& a, const Interval<T>& b) { return IntervalSet<T>(a).set_union(b); }
    template <IntervalCompatible T> auto set_union(const Interval<T>& a, const T& b) { return IntervalSet<T>(a).set_union(b); }
    template <IntervalCompatible T> auto set_union(const T& a, const Interval<T>& b) { return IntervalSet<T>(a).set_union(b); }
    template <IntervalCompatible T> auto set_union(const T& a, const T& b) { return IntervalSet<T>(a).set_union(b); }

    template <IntervalCompatible T> auto set_difference(const IntervalSet<T>& a, const IntervalSet<T>& b) { return a.set_difference(b); }
    template <IntervalCompatible T> auto set_difference(const IntervalSet<T>& a, const Interval<T>& b) { return a.set_difference(b); }
    template <IntervalCompatible T> auto set_difference(const Interval<T>& a, const IntervalSet<T>& b) { return a.set_difference(b); }
    template <IntervalCompatible T> auto set_difference(const IntervalSet<T>& a, const T& b) { return IntervalSet<T>(a).set_difference(b); }
    template <IntervalCompatible T> auto set_difference(const T& a, const IntervalSet<T>& b) { return IntervalSet<T>(a).set_difference(b); }
    template <IntervalCompatible T> auto set_difference(const Interval<T>& a, const Interval<T>& b) { return IntervalSet<T>(a).set_difference(b); }
    template <IntervalCompatible T> auto set_difference(const Interval<T>& a, const T& b) { return IntervalSet<T>(a).set_difference(b); }
    template <IntervalCompatible T> auto set_difference(const T& a, const Interval<T>& b) { return IntervalSet<T>(a).set_difference(b); }
    template <IntervalCompatible T> auto set_difference(const T& a, const T& b) { return IntervalSet<T>(a).set_difference(b); }

    template <IntervalCompatible T> auto set_symmetric_difference(const IntervalSet<T>& a, const IntervalSet<T>& b) { return a.set_symmetric_difference(b); }
    template <IntervalCompatible T> auto set_symmetric_difference(const IntervalSet<T>& a, const Interval<T>& b) { return a.set_symmetric_difference(b); }
    template <IntervalCompatible T> auto set_symmetric_difference(const Interval<T>& a, const IntervalSet<T>& b) { return a.set_symmetric_difference(b); }
    template <IntervalCompatible T> auto set_symmetric_difference(const IntervalSet<T>& a, const T& b) { return IntervalSet<T>(a).set_symmetric_difference(b); }
    template <IntervalCompatible T> auto set_symmetric_difference(const T& a, const IntervalSet<T>& b) { return IntervalSet<T>(a).set_symmetric_difference(b); }
    template <IntervalCompatible T> auto set_symmetric_difference(const Interval<T>& a, const Interval<T>& b) { return IntervalSet<T>(a).set_symmetric_difference(b); }
    template <IntervalCompatible T> auto set_symmetric_difference(const Interval<T>& a, const T& b) { return IntervalSet<T>(a).set_symmetric_difference(b); }
    template <IntervalCompatible T> auto set_symmetric_difference(const T& a, const Interval<T>& b) { return IntervalSet<T>(a).set_symmetric_difference(b); }
    template <IntervalCompatible T> auto set_symmetric_difference(const T& a, const T& b) { return IntervalSet<T>(a).set_symmetric_difference(b); }

}

template <RS::Interval::IntervalCompatible T>
requires (std::formattable<T, char>)
struct std::formatter<RS::Interval::IntervalSet<T>>:
std::formatter<RS::Interval::Interval<T>> {

    template <typename FormatContext>
    auto format(const RS::Interval::IntervalSet<T>& set, FormatContext& ctx) const {

        using base = std::formatter<RS::Interval::Interval<T>>;

        auto out = ctx.out();
        *out++ = '{';

        if (! set.empty()) {
            auto in = set.begin();
            auto end = set.end();
            out = base::format(*in++, ctx);
            while (in != end) {
                *out++ = ',';
                out = base::format(*in++, ctx);
            }
        }

        *out++ = '}';

        return out;

    }

};
