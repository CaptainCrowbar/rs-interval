#pragma once

#include "rs-interval/format.hpp"
#include "rs-interval/interval.hpp"
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
    public Detail::LessThanComparable<IntervalSet<T>> {
    public:
        using value_type = T;
        using interval_type = Interval<T>;
    private:
        using container_type = std::set<interval_type>;
    public:
        using iterator = typename container_type::const_iterator;
        static constexpr auto category = interval_category<T>;
        IntervalSet() = default;
        IntervalSet(const T& t): con_{{t}} {}
        IntervalSet(const interval_type& in): con_{in} {}
        IntervalSet(std::initializer_list<interval_type> list): con_(list) {}
        bool operator()(const T& t) const { return contains(t); }
        IntervalSet operator~() const { return inverse(); }
        IntervalSet& operator&=(const IntervalSet<T>& b) { *this = set_intersection(b); return *this; }
        IntervalSet& operator|=(const IntervalSet<T>& b) { *this = set_union(b); return *this; }
        IntervalSet& operator-=(const IntervalSet<T>& b) { *this = set_difference(b); return *this; }
        IntervalSet& operator^=(const IntervalSet<T>& b) { *this = set_symmetric_difference(b); return *this; }
        auto begin() const noexcept { return con_.begin(); }
        auto end() const noexcept { return con_.end(); }
        bool empty() const noexcept { return con_.empty(); }
        size_t size() const noexcept { return con_.size(); }
        bool contains(const T& t) const;
        void clear() noexcept { con_.clear(); }
        void insert(const interval_type& in);
        void erase(const interval_type& in);
        IntervalSet inverse() const;
        IntervalSet set_intersection(const IntervalSet& b) const;
        IntervalSet set_union(const IntervalSet& b) const;
        IntervalSet set_difference(const IntervalSet& b) const;
        IntervalSet set_symmetric_difference(const IntervalSet& b) const;
        size_t hash() const noexcept;
        std::string str(const std::string& mode = {}) const;
        void swap(IntervalSet& set) noexcept { con_.swap(set.con_); }
    private:
        container_type con_;
    };

        template <typename T>
        bool IntervalSet<T>::contains(const T& t) const {
            interval_type in(t);
            auto i = con_.lower_bound(in);
            if (i != con_.begin())
                --i;
            for (; i != con_.end(); ++i) {
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
            auto i = con_.lower_bound(in);
            if (i != con_.begin())
                --i;
            auto add = in;
            while (i != con_.end()) {
                auto ord = in.order(*i);
                if (ord <= IntervalOrder::a_below_b)
                    break;
                auto j = i++;
                if (ord <= IntervalOrder::b_touches_a) {
                    add = add.envelope(*j);
                    con_.erase(j);
                }
            }
            con_.insert(add);
        }

        template <typename T>
        void IntervalSet<T>::erase(const interval_type& in) {
            if (empty() || in.empty())
                return;
            auto i = con_.lower_bound(in);
            if (i != con_.begin())
                --i;
            IntervalSet temp;
            std::vector<interval_type> vec;
            while (i != con_.end()) {
                auto ord = in.order(*i);
                if (ord <= IntervalOrder::a_touches_b)
                    break;
                auto j = i++;
                if (ord <= IntervalOrder::b_overlaps_a) {
                    temp = j->set_difference(in);
                    std::copy(temp.begin(), temp.end(), std::back_inserter(vec));
                    con_.erase(j);
                }
            }
            con_.insert(vec.begin(), vec.end());
        }

        template <typename T>
        IntervalSet<T> IntervalSet<T>::inverse() const {
            if (empty())
                return interval_type::all();
            IntervalSet result;
            auto i = con_.begin();
            if (i->is_left_bounded())
                result.con_.insert({{}, i->min(), IntervalBound::unbound, ~ i->left()});
            for (auto j = std::next(i), end = con_.end(); j != end; i = j++)
                result.con_.insert({i->max(), j->min(), ~ i->right(), ~ j->left()});
            if (i->is_right_bounded())
                result.con_.insert({i->max(), {}, ~ i->right(), IntervalBound::unbound});
            return result;
        }

        template <typename T>
        IntervalSet<T> IntervalSet<T>::set_intersection(const IntervalSet& b) const {
            return inverse().set_union(b.inverse()).inverse();
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
            return Detail::hash_range(con_);
        }

        template <typename T>
        std::string IntervalSet<T>::str(const std::string& mode) const {
            using namespace Detail;
            if (empty())
                return "{}";
            std::string s = "{";
            for (auto& t: *this)
                s += format_object(t, mode) + ',';
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
