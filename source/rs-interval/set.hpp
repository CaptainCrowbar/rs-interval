#pragma once

#include "rs-interval/interval.hpp"
#include "rs-format/format.hpp"
#include "rs-tl/types.hpp"
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <ostream>
#include <set>
#include <string>
#include <vector>

namespace RS::Intervals {

    // Interval set

    template <typename T>
    class IntervalSet:
    public TL::TotalOrder<IntervalSet<T>> {

    public:

        using iterator = typename std::set<Interval<T>>::const_iterator;
        using interval_type = Interval<T>;
        using value_type = T;

        static constexpr auto category = interval_category<T>;

        IntervalSet() = default;
        IntervalSet(const T& t): set_{{t}} {}
        IntervalSet(const interval_type& in) { insert(in); }
        IntervalSet(std::initializer_list<interval_type> list);

        bool operator()(const T& t) const { return contains(t); }
        IntervalSet operator~() const { return complement(); }
        IntervalSet& operator&=(const IntervalSet<T>& b) { *this = set_intersection(b); return *this; }
        IntervalSet& operator|=(const IntervalSet<T>& b) { *this = set_union(b); return *this; }
        IntervalSet& operator-=(const IntervalSet<T>& b) { *this = set_difference(b); return *this; }
        IntervalSet& operator^=(const IntervalSet<T>& b) { *this = set_symmetric_difference(b); return *this; }

        auto begin() const noexcept { return set_.begin(); }
        auto end() const noexcept { return set_.end(); }
        bool empty() const noexcept { return set_.empty(); }
        size_t size() const noexcept { return set_.size(); }
        bool contains(const T& t) const;
        void clear() noexcept { set_.clear(); }
        void insert(const interval_type& in);
        void erase(const interval_type& in);
        IntervalSet complement() const;
        IntervalSet set_intersection(const IntervalSet& b) const;
        IntervalSet set_union(const IntervalSet& b) const;
        IntervalSet set_difference(const IntervalSet& b) const;
        IntervalSet set_symmetric_difference(const IntervalSet& b) const;
        size_t hash() const noexcept;
        std::string str(const FormatSpec& spec = {}) const;
        void swap(IntervalSet& set) noexcept { set_.swap(set.set_); }

    private:

        std::set<Interval<T>> set_;

    };

        template <typename T>
        IntervalSet<T>::IntervalSet(std::initializer_list<interval_type> list) {
            for (auto& in: list)
                insert(in);
        }

        template <typename T>
        bool IntervalSet<T>::contains(const T& t) const {
            interval_type in(t);
            auto i = set_.lower_bound(in);
            if (i != set_.begin())
                --i;
            for (; i != set_.end(); ++i) {
                auto m = i->match(t);
                if (m == IntervalMatch::match)
                    return true;
                else if (m == IntervalMatch::low)
                    return false;
            }
            return false;
        }

        template <typename T>
        void IntervalSet<T>::insert(const interval_type& in) {
            if (in.empty())
                return;
            auto i = set_.lower_bound(in);
            if (i != set_.begin())
                --i;
            auto add = in;
            while (i != set_.end()) {
                auto ord = in.order(*i);
                if (ord <= IntervalOrder::a_below_b)
                    break;
                auto j = i++;
                if (ord <= IntervalOrder::b_touches_a) {
                    add = add.envelope(*j);
                    set_.erase(j);
                }
            }
            set_.insert(add);
        }

        template <typename T>
        void IntervalSet<T>::erase(const interval_type& in) {
            if (empty() || in.empty())
                return;
            auto i = set_.lower_bound(in);
            if (i != set_.begin())
                --i;
            IntervalSet temp;
            std::vector<interval_type> vec;
            while (i != set_.end()) {
                auto ord = in.order(*i);
                if (ord <= IntervalOrder::a_touches_b)
                    break;
                auto j = i++;
                if (ord <= IntervalOrder::b_overlaps_a) {
                    temp = j->set_difference(in);
                    std::copy(temp.begin(), temp.end(), std::back_inserter(vec));
                    set_.erase(j);
                }
            }
            set_.insert(vec.begin(), vec.end());
        }

        template <typename T>
        IntervalSet<T> IntervalSet<T>::complement() const {
            if (empty())
                return interval_type::all();
            IntervalSet result;
            auto i = set_.begin();
            if (i->is_left_bounded())
                result.set_.insert({{}, i->min(), IntervalBound::unbound, ~ i->left()});
            for (auto j = std::next(i), end = set_.end(); j != end; i = j++)
                result.set_.insert({i->max(), j->min(), ~ i->right(), ~ j->left()});
            if (i->is_right_bounded())
                result.set_.insert({i->max(), {}, ~ i->right(), IntervalBound::unbound});
            return result;
        }

        template <typename T>
        IntervalSet<T> IntervalSet<T>::set_intersection(const IntervalSet& b) const {
            return complement().set_union(b.complement()).complement();
        }

        template <typename T>
        IntervalSet<T> IntervalSet<T>::set_union(const IntervalSet& b) const {
            auto result = *this;
            for (auto& in: b)
                result.insert(in);
            return result;
        }

        template <typename T>
        IntervalSet<T> IntervalSet<T>::set_difference(const IntervalSet& b) const {
            auto result = *this;
            for (auto& in: b)
                result.erase(in);
            return result;
        }

        template <typename T>
        IntervalSet<T> IntervalSet<T>::set_symmetric_difference(const IntervalSet& b) const {
            auto set1 = set_difference(b);
            auto set2 = b.set_difference(*this);
            return set1.set_union(set2);
        }

        template <typename T>
        size_t IntervalSet<T>::hash() const noexcept {
            return Detail::hash_range(set_);
        }

        template <typename T>
        std::string IntervalSet<T>::str(const FormatSpec& spec) const {
            using namespace RS::Format;
            if (empty())
                return "{}";
            std::string s = "{";
            for (auto& t: *this) {
                s += format_object(t, spec);
                s += ',';
            }
            s.back() = '}';
            return s;
        }

    template <typename T> IntervalSet<T> operator&(const IntervalSet<T>& a, const IntervalSet<T>& b) { return a.set_intersection(b); }
    template <typename T> IntervalSet<T> operator&(const IntervalSet<T>& a, const Interval<T>& b) { return a.set_intersection(b); }
    template <typename T> IntervalSet<T> operator&(const Interval<T>& a, const IntervalSet<T>& b) { return a.set_intersection(b); }
    template <typename T> IntervalSet<T> operator&(const IntervalSet<T>& a, const T& b) { return IntervalSet<T>(a).set_intersection(b); }
    template <typename T> IntervalSet<T> operator&(const T& a, const IntervalSet<T>& b) { return IntervalSet<T>(a).set_intersection(b); }
    template <typename T> IntervalSet<T> operator|(const IntervalSet<T>& a, const IntervalSet<T>& b) { return a.set_union(b); }
    template <typename T> IntervalSet<T> operator|(const IntervalSet<T>& a, const Interval<T>& b) { return a.set_union(b); }
    template <typename T> IntervalSet<T> operator|(const Interval<T>& a, const IntervalSet<T>& b) { return a.set_union(b); }
    template <typename T> IntervalSet<T> operator|(const IntervalSet<T>& a, const T& b) { return IntervalSet<T>(a).set_union(b); }
    template <typename T> IntervalSet<T> operator|(const T& a, const IntervalSet<T>& b) { return IntervalSet<T>(a).set_union(b); }
    template <typename T> IntervalSet<T> operator-(const IntervalSet<T>& a, const IntervalSet<T>& b) { return a.set_difference(b); }
    template <typename T> IntervalSet<T> operator-(const IntervalSet<T>& a, const Interval<T>& b) { return a.set_difference(b); }
    template <typename T> IntervalSet<T> operator-(const Interval<T>& a, const IntervalSet<T>& b) { return a.set_difference(b); }
    template <typename T> IntervalSet<T> operator-(const IntervalSet<T>& a, const T& b) { return IntervalSet<T>(a).set_difference(b); }
    template <typename T> IntervalSet<T> operator-(const T& a, const IntervalSet<T>& b) { return IntervalSet<T>(a).set_difference(b); }
    template <typename T> IntervalSet<T> operator^(const IntervalSet<T>& a, const IntervalSet<T>& b) { return a.set_symmetric_difference(b); }
    template <typename T> IntervalSet<T> operator^(const IntervalSet<T>& a, const Interval<T>& b) { return a.set_symmetric_difference(b); }
    template <typename T> IntervalSet<T> operator^(const Interval<T>& a, const IntervalSet<T>& b) { return a.set_symmetric_difference(b); }
    template <typename T> IntervalSet<T> operator^(const IntervalSet<T>& a, const T& b) { return IntervalSet<T>(a).set_symmetric_difference(b); }
    template <typename T> IntervalSet<T> operator^(const T& a, const IntervalSet<T>& b) { return IntervalSet<T>(a).set_symmetric_difference(b); }
    template <typename T> bool operator==(const IntervalSet<T>& a, const IntervalSet<T>& b) noexcept
        { return std::equal(a.begin(), a.end(), b.begin(), b.end()); }
    template <typename T> bool operator<(const IntervalSet<T>& a, const IntervalSet<T>& b) noexcept
        { return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end()); }
    template <typename T> std::ostream& operator<<(std::ostream& out, const IntervalSet<T>& set) { return out << set.str(); }
    template <typename T> void swap(IntervalSet<T>& a, IntervalSet<T>& b) { a.swap(b); }

}

namespace std {

    template <typename T>
    class hash<RS::Intervals::IntervalSet<T>> {
    public:
        size_t operator()(const RS::Intervals::IntervalSet<T>& set) const noexcept { return set.hash(); }
    };

}
