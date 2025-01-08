#pragma once

#include "rs-interval/interval.hpp"
#include "rs-interval/types.hpp"
#include <algorithm>
#include <compare>
#include <concepts>
#include <cstddef>
#include <format>
#include <initializer_list>
#include <map>
#include <utility>
#include <vector>

namespace RS::Interval {

    // Interval map

    template <IntervalCompatible K, std::regular T>
    class IntervalMap {

    public:

        using key_type = K;
        using mapped_type = T;
        using interval_type = Interval<K>;
        using iterator = typename std::map<Interval<K>, T>::const_iterator;
        using value_type = typename std::map<Interval<K>, T>::value_type;

        static constexpr auto category = interval_category<K>;

        IntervalMap() = default;
        explicit IntervalMap(const T& defval): map_(), def_(defval) {}
        IntervalMap(std::initializer_list<value_type> list) { for (const auto& v: list) insert(v); }

        const T& operator[](const K& key) const;

        auto begin() const noexcept { return map_.begin(); }
        auto end() const noexcept { return map_.end(); }
        bool empty() const noexcept { return map_.empty(); }
        std::size_t size() const noexcept { return map_.size(); }
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
        void swap(IntervalMap& map) noexcept { map_.swap(map.map_); std::swap(def_, map.def_); }

    private:

        std::map<Interval<K>, T> map_;
        T def_ {};

        std::pair<iterator, bool> do_find(const K& key) const;

    };

        template <IntervalCompatible K, std::regular T>
        const T& IntervalMap<K, T>::operator[](const K& key) const {
            auto [it,ok] = do_find(key);
            return ok ? it->second : def_;
        }

        template <IntervalCompatible K, std::regular T>
        typename IntervalMap<K, T>::iterator IntervalMap<K, T>::find(const K& key) const {
            auto [it,ok] = do_find(key);
            return ok ? it : end();
        }

        template <IntervalCompatible K, std::regular T>
        typename IntervalMap<K, T>::iterator IntervalMap<K, T>::upper_bound(const K& key) const {
            auto [it,ok] = do_find(key);
            if (ok) {
                ++it;
            }
            return it;
        }

        template <IntervalCompatible K, std::regular T>
        void IntervalMap<K, T>::insert(const interval_type& in, const T& t) {

            if (in.empty()) {
                return;
            }

            auto key = in;
            auto i = map_.upper_bound(key);

            if (i != map_.begin()) {
                --i;
            }

            std::vector<value_type> add;
            std::vector<iterator> del;

            for (; i != map_.end(); ++i) {
                auto ord = key.order(i->first);
                if (ord <= Order::a_below_b) {
                    break;
                } else if (ord <= Order::b_touches_a && i->second == t) {
                    key = key.envelope(i->first);
                    del.push_back(i);
                } else if (ord == Order::b_touches_a) {
                    break;
                } else if (ord <= Order::b_overlaps_a) {
                    auto diff = i->first.set_difference(in);
                    for (const auto& d: diff)
                        add.push_back({d, i->second});
                    del.push_back(i);
                }
            }

            for (const auto& d: del) {
                map_.erase(d);
            }

            for (const auto& a: add) {
                map_.insert(a);
            }

            map_.insert({key, t});

        }

        template <IntervalCompatible K, std::regular T>
        void IntervalMap<K, T>::erase(const interval_type& in) {

            if (empty() || in.empty()) {
                return;
            }

            auto i = map_.lower_bound(in);

            if (i != map_.begin()) {
                --i;
            }

            std::vector<value_type> vec;

            while (i != map_.end()) {
                auto ord = in.order(i->first);
                if (ord <= Order::a_touches_b) {
                    break;
                }
                auto j = i++;
                if (ord <= Order::b_overlaps_a) {
                    auto temp = j->first.set_difference(in);
                    for (const auto& t: temp) {
                        vec.push_back({t, j->second});
                    }
                    map_.erase(j);
                }
            }

            map_.insert(vec.begin(), vec.end());

        }

        template <IntervalCompatible K, std::regular T>
        std::pair<typename IntervalMap<K, T>::iterator, bool> IntervalMap<K, T>::do_find(const K& key) const {

            if (empty()) {
                return {end(), false};
            }

            interval_type in(key);
            auto it = map_.lower_bound(in);

            if (it != map_.begin()) {
                --it;
            }

            for (; it != map_.end(); ++it) {
                auto m = it->first.match(key);
                if (m != Match::high) {
                    return {it, m == Match::match};
                }
            }

            return {end(), false};

        }

    template <IntervalCompatible K, std::regular T>
    bool operator==(const IntervalMap<K, T>& a, const IntervalMap<K, T>& b) noexcept {
        return std::equal(a.begin(), a.end(), b.begin(), b.end());
    }

    template <IntervalCompatible K, std::regular T>
    auto operator<=>(const IntervalMap<K, T>& a, const IntervalMap<K, T>& b) noexcept {

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

    template <IntervalCompatible K, std::regular T>
    void swap(IntervalMap<K, T>& a, IntervalMap<K, T>& b) noexcept {
        a.swap(b);
    }

}

template <RS::Interval::IntervalCompatible K, std::regular T>
requires (std::formattable<K, char> && std::formattable<T, char>)
struct std::formatter<RS::Interval::IntervalMap<K, T>> {

    std::formatter<RS::Interval::Interval<K>> key_interval_formatter;
    std::formatter<T> value_formatter;

    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const RS::Interval::IntervalMap<K, T>& map, FormatContext& ctx) const {

        auto out = ctx.out();
        *out++ = '{';

        if (! map.empty()) {

            auto in = map.begin();
            auto end = map.end();
            out = key_interval_formatter.format(in->first, ctx);
            *out++ = ':';
            out = value_formatter.format(in->second, ctx);
            ++in;

            while (in != end) {
                *out++ = ',';
                out = key_interval_formatter.format(in->first, ctx);
                *out++ = ':';
                out = value_formatter.format(in->second, ctx);
                ++in;
            }

        }

        *out++ = '}';

        return out;

    }

};
