#pragma once

#include "rs-interval/interval.hpp"
#include "rs-format/format.hpp"
#include <algorithm>
#include <initializer_list>
#include <map>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace RS::Intervals {

    // Interval map

    template <typename K, typename T>
    class IntervalMap:
    public Detail::TotalOrder<IntervalMap<K, T>> {

    public:

        using key_type = K;
        using mapped_type = T;
        using interval_type = Interval<K>;
        using iterator = typename std::map<Interval<K>, T>::const_iterator;
        using value_type = typename std::map<Interval<K>, T>::value_type;

        static constexpr auto category = interval_category<K>;

        IntervalMap() = default;
        explicit IntervalMap(const T& defval): map_(), def_(defval) {}
        IntervalMap(std::initializer_list<value_type> list) { for (auto& v: list) insert(v); }

        const T& operator[](const K& key) const;

        auto begin() const noexcept { return map_.begin(); }
        auto end() const noexcept { return map_.end(); }
        bool empty() const noexcept { return map_.empty(); }
        size_t size() const noexcept { return map_.size(); }
        const T& default_value() const noexcept { return def_; }
        void default_value(const T& defval) { def_ = defval; }
        bool contains(const K& key) const { return do_find(key).second; }
        iterator find(const K& key) const;
        iterator lower_bound(const K& key) const { return do_find(key).first; }
        iterator upper_bound(const K& key) const;
        void clear() noexcept { map_.clear(); }
        void reset(const T& defval = {}) { def_ = defval; clear(); }
        void insert(const interval_type& in, const T& t);
        void insert(const value_type& v) { insert(v.first, v.second); }
        void erase(const interval_type& in);
        size_t hash() const noexcept;
        std::string str() const { return str({}, {}); }
        std::string str(const FormatSpec& kspec, const FormatSpec& vspec) const;
        void swap(IntervalMap& map) noexcept { map_.swap(map.map_); std::swap(def_, map.def_); }

    private:

        std::map<Interval<K>, T> map_;
        T def_ = T();

        std::pair<iterator, bool> do_find(const K& key) const;

    };

        template <typename K, typename T>
        const T& IntervalMap<K, T>::operator[](const K& key) const {
            auto [it,ok] = do_find(key);
            return ok ? it->second : def_;
        }

        template <typename K, typename T>
        typename IntervalMap<K, T>::iterator IntervalMap<K, T>::find(const K& key) const {
            auto [it,ok] = do_find(key);
            return ok ? it : end();
        }

        template <typename K, typename T>
        typename IntervalMap<K, T>::iterator IntervalMap<K, T>::upper_bound(const K& key) const {
            auto [it,ok] = do_find(key);
            if (ok)
                ++it;
            return it;
        }

        template <typename K, typename T>
        void IntervalMap<K, T>::insert(const interval_type& in, const T& t) {
            if (in.empty())
                return;
            auto key = in;
            auto i = map_.upper_bound(key);
            if (i != map_.begin())
                --i;
            std::vector<value_type> add;
            std::vector<iterator> del;
            for (; i != map_.end(); ++i) {
                auto ord = key.order(i->first);
                if (ord <= IntervalOrder::a_below_b) {
                    break;
                } else if (ord <= IntervalOrder::b_touches_a && i->second == t) {
                    key = key.envelope(i->first);
                    del.push_back(i);
                } else if (ord == IntervalOrder::b_touches_a) {
                    break;
                } else if (ord <= IntervalOrder::b_overlaps_a) {
                    auto diff = i->first.set_difference(in);
                    for (auto& d: diff)
                        add.push_back({d, i->second});
                    del.push_back(i);
                }
            }
            for (auto& d: del)
                map_.erase(d);
            for (auto& a: add)
                map_.insert(a);
            map_.insert({key, t});
        }

        template <typename K, typename T>
        void IntervalMap<K, T>::erase(const interval_type& in) {
            if (empty() || in.empty())
                return;
            auto i = map_.lower_bound(in);
            if (i != map_.begin())
                --i;
            std::vector<value_type> vec;
            while (i != map_.end()) {
                auto ord = in.order(i->first);
                if (ord <= IntervalOrder::a_touches_b)
                    break;
                auto j = i++;
                if (ord <= IntervalOrder::b_overlaps_a) {
                    auto temp = j->first.set_difference(in);
                    for (auto& t: temp)
                        vec.push_back({t, j->second});
                    map_.erase(j);
                }
            }
            map_.insert(vec.begin(), vec.end());
        }

        template <typename K, typename T>
        size_t IntervalMap<K, T>::hash() const noexcept {
            return Detail::hash_range(map_);
        }

        template <typename K, typename T>
        std::string IntervalMap<K, T>::str(const FormatSpec& kspec, const FormatSpec& vspec) const {
            using namespace RS::Format;
            if (empty())
                return "{}";
            std::string s = "{";
            for (auto& [k,v]: *this) {
                s += format_object(k, kspec);
                s += ':';
                s += format_object(v, vspec);
                s += ',';
            }
            s.back() = '}';
            return s;
        }

        template <typename K, typename T>
        std::pair<typename IntervalMap<K, T>::iterator, bool> IntervalMap<K, T>::do_find(const K& key) const {
            if (empty())
                return {end(), false};
            interval_type in(key);
            auto it = map_.lower_bound(in);
            if (it != map_.begin())
                --it;
            for (; it != map_.end(); ++it) {
                auto m = it->first.match(key);
                if (m != IntervalMatch::high)
                    return {it, m == IntervalMatch::match};
            }
            return {end(), false};
        }

    template <typename K, typename T> bool operator==(const IntervalMap<K, T>& a, const IntervalMap<K, T>& b) noexcept
        { return std::equal(a.begin(), a.end(), b.begin(), b.end()); }
    template <typename K, typename T> bool operator<(const IntervalMap<K, T>& a, const IntervalMap<K, T>& b) noexcept
        { return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end()); }
    template <typename K, typename T> std::ostream& operator<<(std::ostream& out, const IntervalMap<K, T>& set) { return out << set.str(); }
    template <typename K, typename T> void swap(IntervalMap<K, T>& a, IntervalMap<K, T>& b) { a.swap(b); }

}

namespace std {

    template <typename K, typename T>
    class hash<RS::Intervals::IntervalMap<K, T>> {
    public:
        size_t operator()(const RS::Intervals::IntervalMap<K, T>& map) const noexcept { return map.hash(); }
    };

}
