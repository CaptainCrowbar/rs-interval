#pragma once

#include "rs-intervals/arithmetic.hpp"
#include "rs-intervals/category-base-class.hpp"
#include "rs-intervals/interval-base-class.hpp"
#include "rs-intervals/types.hpp"
#include <algorithm>
#include <compare>
#include <concepts>
#include <cstddef>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace RS::Intervals {

    template <IntervalCompatible T> class Interval;
    template <IntervalCompatible T> class IntervalSet;
    template <IntervalCompatible K, std::regular T> class IntervalMap;

    // Interval class

    namespace Detail {

        inline std::pair<IntervalBound, IntervalBound> decode_interval_bounds(std::string_view mode) {

            using IB = IntervalBound;

            static const std::unordered_map<std::string_view, std::pair<IntervalBound, IntervalBound>> map = {
                { "()",  { IB::open,     IB::open     }},
                { "(]",  { IB::open,     IB::closed   }},
                { "[)",  { IB::closed,   IB::open     }},
                { "[]",  { IB::closed,   IB::closed   }},
                { ">",   { IB::open,     IB::unbound  }},
                { "<",   { IB::unbound,  IB::open     }},
                { ">=",  { IB::closed,   IB::unbound  }},
                { "<=",  { IB::unbound,  IB::closed   }},
                { "*",   { IB::unbound,  IB::unbound  }},
            };

            auto it = map.find(mode);
            if (it == map.end())
                throw std::invalid_argument("Invalid interval mode: " + std::string(mode));
            return it->second;

        }

    }

    template <IntervalCompatible T>
    class Interval:
    public IntervalCategoryBase<T> {

    public:

        using value_type = T;

        static constexpr auto category = interval_category<T>;

        Interval() = default;
        Interval(const T& t): Interval(t, t, IntervalBound::closed, IntervalBound::closed) {}
        Interval(const T& t, IntervalBound l, IntervalBound r): Interval(t, t, l, r) {}
        Interval(const T& min, const T& max, IntervalBound lr = IntervalBound::closed): Interval(min, max, lr, lr) {}
        Interval(const T& min, const T& max, IntervalBound l, IntervalBound r);
        Interval(const T& min, const T& max, std::string_view mode);

        explicit operator bool() const noexcept { return ! this->empty(); }
        bool operator()(const T& t) const { return contains(t); }

        bool contains(const T& t) const { return this->match(t) == IntervalMatch::match; }
        IntervalSet<T> complement() const;
        IntervalSet<T> operator~() const { return complement(); }
        IntervalOrder order(const Interval& b) const;
        bool includes(const Interval& b) const;      // True if b is a subset of this
        bool overlaps(const Interval& b) const;      // True if the intersection is not empty
        bool touches(const Interval& b) const;       // True if there is no gap between this and b
        Interval envelope(const Interval& b) const;  // Spanning interval, including any gap
        Interval set_intersection(const Interval& b) const;
        IntervalSet<T> set_union(const Interval& b) const;
        IntervalSet<T> set_difference(const Interval& b) const;
        IntervalSet<T> set_symmetric_difference(const Interval& b) const;
        template <Formatter<T> F> std::string str(const F& f) const;
        std::string str() const { return str(DefaultFormatter()); }
        void swap(Interval& in) noexcept { this->do_swap(in); }

        static Interval all() { return Interval(T(), IntervalBound::unbound, IntervalBound::unbound); }
        static Interval from_string(const std::string& str);

        friend bool operator==(const Interval& a, const Interval& b) noexcept { return a.compare(b) == 0; }
        friend std::strong_ordering operator<=>(const Interval& a, const Interval& b) noexcept { return a.compare(b); }

    private:

        std::strong_ordering compare(const Interval& b) const noexcept;

    };

        template <IntervalCompatible T>
        Interval<T>::Interval(const T& min, const T& max, IntervalBound l, IntervalBound r) {
            this->min_ = min;
            this->max_ = max;
            this->left_ = l;
            this->right_ = r;
            this->adjust_bounds();
        }

        template <IntervalCompatible T>
        Interval<T>::Interval(const T& min, const T& max, std::string_view mode) {
            auto bounds = Detail::decode_interval_bounds(mode);
            this->min_ = min;
            this->max_ = max;
            this->left_ = bounds.first;
            this->right_ = bounds.second;
            this->adjust_bounds();
        }

        template <IntervalCompatible T>
        IntervalSet<T> Interval<T>::complement() const {

            if (this->empty())
                return all();
            else if (this->is_universal())
                return {};

            IntervalSet<T> set;

            if (this->is_left_bounded())
                set.insert({this->min(), IntervalBound::unbound, ~ this->left()});

            if (this->is_right_bounded())
                set.insert({this->max(), ~ this->right(), IntervalBound::unbound});

            return set;

        }

        template <IntervalCompatible T>
        IntervalOrder Interval<T>::order(const Interval& b) const {

            using namespace Detail;

            using B = Boundary<T>;

            auto& a = *this;

            if (a.empty() && b.empty())
                return IntervalOrder::equal;
            else if (a.empty())
                return IntervalOrder::b_only;
            else if (b.empty())
                return IntervalOrder::a_only;

            B al = left_boundary_of(a);
            B ar = right_boundary_of(a);
            B bl = left_boundary_of(b);
            B br = right_boundary_of(b);

            if (ar.compare_rl(bl)) {
                if (ar.adjacent(bl))
                    return IntervalOrder::a_touches_b;
                else
                    return IntervalOrder::a_below_b;
            } else if (br.compare_rl(al)) {
                if (br.adjacent(al))
                    return IntervalOrder::b_touches_a;
                else
                    return IntervalOrder::b_below_a;
            } else if (al.compare_ll(bl)) {
                if (ar.compare_rr(br))
                    return IntervalOrder::a_overlaps_b;
                else if (br.compare_rr(ar))
                    return IntervalOrder::a_encloses_b;
                else
                    return IntervalOrder::a_extends_below_b;
            } else if (bl.compare_ll(al)) {
                if (ar.compare_rr(br))
                    return IntervalOrder::b_encloses_a;
                else if (br.compare_rr(ar))
                    return IntervalOrder::b_overlaps_a;
                else
                    return IntervalOrder::b_extends_below_a;
            } else {
                if (ar.compare_rr(br))
                    return IntervalOrder::b_extends_above_a;
                else if (br.compare_rr(ar))
                    return IntervalOrder::a_extends_above_b;
                else
                    return IntervalOrder::equal;
            }

        }

        template <IntervalCompatible T>
        bool Interval<T>::includes(const Interval& b) const {
            auto& a = *this;
            IntervalOrder ord = order(b);
            if (ord == IntervalOrder::equal)
                return ! a.empty();
            else
                return ord == IntervalOrder::a_extends_above_b || ord == IntervalOrder::a_extends_below_b
                    || ord == IntervalOrder::a_encloses_b;
        }

        template <IntervalCompatible T>
        bool Interval<T>::overlaps(const Interval& b) const {

            auto& a = *this;

            if (a.empty() || b.empty())
                return false;

            IntervalOrder ord = order(b);

            if (ord == IntervalOrder::equal)
                return true;
            else if (ord == IntervalOrder::a_below_b || ord == IntervalOrder::b_below_a
                    || ord == IntervalOrder::a_touches_b || ord == IntervalOrder::b_touches_a)
                return false;
            else
                return true;

        }

        template <IntervalCompatible T>
        bool Interval<T>::touches(const Interval& b) const {

            auto& a = *this;

            if (a.empty() || b.empty())
                return false;

            IntervalOrder ord = order(b);

            if (ord == IntervalOrder::equal)
                return true;
            else if (ord == IntervalOrder::a_below_b || ord == IntervalOrder::b_below_a)
                return false;
            else
                return true;

        }

        template <IntervalCompatible T>
        Interval<T> Interval<T>::envelope(const Interval& b) const {

            auto& a = *this;

            if (a.empty())
                return b;
            else if (b.empty())
                return a;

            switch (order(b)) {

                case IntervalOrder::a_below_b:
                case IntervalOrder::a_overlaps_b:
                case IntervalOrder::a_touches_b:
                    return {a.min(), b.max(), a.left(), b.right()};

                case IntervalOrder::b_below_a:
                case IntervalOrder::b_overlaps_a:
                case IntervalOrder::b_touches_a:
                    return {b.min(), a.max(), b.left(), a.right()};

                case IntervalOrder::b_encloses_a:
                case IntervalOrder::b_extends_above_a:
                case IntervalOrder::b_extends_below_a:
                    return b;

                default:
                    return a;

            }

        }

        template <IntervalCompatible T>
        Interval<T> Interval<T>::set_intersection(const Interval& b) const {

            auto& a = *this;

            if (a.empty() || b.empty())
                return {};

            switch (order(b)) {

                case IntervalOrder::b_overlaps_a:
                    return {a.min(), b.max(), a.left(), b.right()};

                case IntervalOrder::a_overlaps_b:
                    return {b.min(), a.max(), b.left(), a.right()};

                case IntervalOrder::b_encloses_a:
                case IntervalOrder::b_extends_above_a:
                case IntervalOrder::b_extends_below_a:
                case IntervalOrder::equal:
                    return a;

                case IntervalOrder::a_encloses_b:
                case IntervalOrder::a_extends_above_b:
                case IntervalOrder::a_extends_below_b:
                    return b;

                default:
                    return {};

            }

        }

        template <IntervalCompatible T>
        IntervalSet<T> Interval<T>::set_union(const Interval& b) const {

            auto& a = *this;

            if (a.empty() && b.empty())
                return {};

            switch (order(b)) {

                case IntervalOrder::a_below_b:
                    return {a,b};

                case IntervalOrder::a_overlaps_b:
                case IntervalOrder::a_touches_b:
                    return {{a.min(), b.max(), a.left(), b.right()}};

                case IntervalOrder::b_below_a:
                    return {b,a};

                case IntervalOrder::b_overlaps_a:
                case IntervalOrder::b_touches_a:
                    return {{b.min(), a.max(), b.left(), a.right()}};

                case IntervalOrder::b_encloses_a:
                case IntervalOrder::b_extends_above_a:
                case IntervalOrder::b_extends_below_a:
                case IntervalOrder::b_only:
                    return {b};

                default:
                    return {a};

            }

        }

        template <IntervalCompatible T>
        IntervalSet<T> Interval<T>::set_difference(const Interval& b) const {

            auto& a = *this;

            switch (order(b)) {

                case IntervalOrder::a_extends_above_b:
                case IntervalOrder::b_overlaps_a:
                    return {{b.max(), a.max(), ~ b.right(), a.right()}};

                case IntervalOrder::a_extends_below_b:
                case IntervalOrder::a_overlaps_b:
                    return {{a.min(), b.min(), a.left(), ~ b.left()}};

                case IntervalOrder::a_encloses_b:
                    return {{a.min(), b.min(), a.left(), ~ b.left()}, {b.max(), a.max(), ~ b.right(), a.right()}};

                case IntervalOrder::a_below_b:
                case IntervalOrder::a_only:
                case IntervalOrder::a_touches_b:
                case IntervalOrder::b_below_a:
                case IntervalOrder::b_touches_a:
                    return {a};

                default:
                    return {};

            }

        }

        template <IntervalCompatible T>
        IntervalSet<T> Interval<T>::set_symmetric_difference(const Interval& b) const {

            auto& a = *this;

            switch (order(b)) {
                case IntervalOrder::b_only:
                    return {b};
                case IntervalOrder::a_below_b:
                    return {a, b};
                case IntervalOrder::a_touches_b:
                    return {{a.min(), b.max(), a.left(), b.right()}};
                case IntervalOrder::a_overlaps_b:
                    return {{a.min(), b.min(), a.left(), ~ b.left()}, {a.max(), b.max(), ~ a.right(), b.right()}};
                case IntervalOrder::a_extends_below_b:
                    return {{a.min(), b.min(), a.left(), ~ b.left()}};
                case IntervalOrder::a_encloses_b:
                    return {{a.min(), b.min(), a.left(), ~ b.left()}, {b.max(), a.max(), ~ b.right(), a.right()}};
                case IntervalOrder::b_extends_above_a:
                    return {{a.max(), b.max(), ~ a.right(), b.right()}};
                case IntervalOrder::a_extends_above_b:
                    return {{b.max(), a.max(), ~ b.right(), a.right()}};
                case IntervalOrder::b_encloses_a:
                    return {{b.min(), a.min(), b.left(), ~ a.left()}, {a.max(), b.max(), ~ a.right(), b.right()}};
                case IntervalOrder::b_extends_below_a:
                    return {{b.min(), a.min(), b.left(), ~ a.left()}};
                case IntervalOrder::b_overlaps_a:
                    return {{b.min(), a.min(), b.left(), ~ a.left()}, {b.max(), a.max(), ~ b.right(), a.right()}};
                case IntervalOrder::b_touches_a:
                    return {{b.min(), a.max(), b.left(), a.right()}};
                case IntervalOrder::b_below_a:
                    return {b, a};
                case IntervalOrder::a_only:
                    return {a};
                default:
                    return {};
            }

        }

        template <IntervalCompatible T>
        template <Formatter<T> F>
        std::string Interval<T>::str(const F& f) const {
            if (this->empty())
                return "{}";
            else if (this->is_universal())
                return "*";
            else if (this->is_single())
                return f(this->min());
            else if (! this->is_left_bounded())
                return (this->is_right_closed() ? "<=" : "<") + f(this->max());
            else if (! this->is_right_bounded())
                return (this->is_left_closed() ? ">=" : ">") + f(this->min());
            else
                return (this->is_left_closed() ? '[' : '(') + f(this->min()) + ','
                    + f(this->max()) + (this->is_right_closed() ? ']' : ')');
        }

        template <IntervalCompatible T>
        Interval<T> Interval<T>::from_string(const std::string& str) {

            const auto fail = [&] { throw std::invalid_argument("Invalid interval: " + str); };

            if (str.empty() || str == "{}")
                return {};
            else if (str == "*")
                return Interval::all();

            char first = str.front();
            char last = str.back();
            T min, max;
            std::string mode;

            if ((first == '(' || first == '[') && (last == ')' || last == ']')) {

                std::size_t comma = str.find(',');

                if (comma == npos)
                    fail();

                min = Detail::from_string<T>(str.substr(1, comma - 1));
                max = Detail::from_string<T>(str.substr(comma + 1, str.size() - comma - 2));
                mode = {first, last};

            } else if (first == '<' || first == '>') {

                mode = str.substr(0, std::size_t(str[1] == '=') + 1);
                min = max = Detail::from_string<T>(str.substr(mode.size()));

            } else if (last == '+' || last == '-') {

                mode = last == '+' ? ">=" : "<=";
                min = max = Detail::from_string<T>(str.substr(0, str.size() - 1));

            } else {

                static const std::vector<std::string> delimiters = { "..<", "...", "..", "-" };

                mode = "[]";
                std::size_t i = str.find('-');
                std::size_t j = npos;

                if (i == npos) {
                    i = str.find("..");
                    if (i == npos)
                        return Interval(Detail::from_string<T>(str));
                    j = i + 2;
                    if (i >= 2 && str[i - 1] == '<')
                        --i;
                    if (j < str.size() && (str[j] == '.' || str[j] == '<'))
                        ++j;
                    if (i == 0 || j >= str.size())
                        return Interval(Detail::from_string<T>(str));
                    if (str[i] == '<')
                        mode[0] = '(';
                    if (str[j - 1] == '<')
                        mode[1] = ')';
                } else {
                    j = i + 1;
                }

                min = Detail::from_string<T>(str.substr(0, i));
                max = Detail::from_string<T>(str.substr(j));

            }

            return Interval(min, max, mode);

        }

        template <IntervalCompatible T>
        std::strong_ordering Interval<T>::compare(const Interval& b) const noexcept {

            using namespace Detail;

            auto& a = *this;

            if (a.empty() && b.empty())
                return SO::equal;
            else if (a.empty())
                return SO::less;
            else if (b.empty())
                return SO::greater;

            if (a.is_left_bounded() && b.is_left_bounded()) {
                if (a.min() > b.min())
                    return SO::greater;
                else if (a.min() < b.min())
                    return SO::less;
                else if (a.is_left_closed() != b.is_left_closed())
                    return a.is_left_closed() ? SO::less : SO::greater;
            } else if (a.is_left_bounded() || b.is_left_bounded()) {
                return a.is_left_bounded() ? SO::greater : SO::less;
            }

            if (a.is_right_bounded() && b.is_right_bounded()) {
                if (a.max() < b.max())
                    return SO::less;
                else if (a.max() > b.max())
                    return SO::greater;
                else if (a.is_right_closed() != b.is_right_closed())
                    return a.is_right_closed() ? SO::greater : SO::less;
            } else if (a.is_right_bounded() || b.is_right_bounded()) {
                return a.is_right_bounded() ? SO::less : SO::greater;
            }

            return SO::equal;

        }

    template <IntervalCompatible T>
    std::ostream& operator<<(std::ostream& out, const Interval<T>& in) {
        return out << in.str();
    }

    template <IntervalCompatible T>
    Interval<T> make_interval(const T& t) {
        return Interval<T>(t);
    }

    template <IntervalCompatible T>
    Interval<T> make_interval(const T& t, IntervalBound l, IntervalBound r) {
        return Interval<T>(t, l, r);
    }

    template <IntervalCompatible T>
    Interval<T> make_interval(const T& min, const T& max, IntervalBound lr = IntervalBound::closed) {
        return Interval<T>(min, max, lr);
    }

    template <IntervalCompatible T>
    Interval<T> make_interval(const T& min, const T& max, IntervalBound l, IntervalBound r) {
        return Interval<T>(min, max, l, r);
    }

    template <IntervalCompatible T>
    Interval<T> make_interval(const T& min, const T& max, std::string_view mode) {
        return Interval<T>(min, max, mode);
    }

    template <IntervalCompatible T>
    Interval<T> ordered_interval(const T& a, const T& b, IntervalBound lr = IntervalBound::closed) {
        if (a <= b)
            return Interval<T>(a, b, lr);
        else
            return Interval<T>(b, a, lr);
    }

    template <IntervalCompatible T>
    Interval<T> ordered_interval(const T& a, const T& b, IntervalBound l, IntervalBound r) {
        if (a <= b)
            return Interval<T>(a, b, l, r);
        else
            return Interval<T>(b, a, r, l);
    }

    template <IntervalCompatible T>
    void swap(Interval<T>& a, Interval<T>& b) noexcept {
        a.swap(b);
    }

}

namespace std {

    template <RS::Intervals::IntervalCompatible T>
    requires (RS::Intervals::Detail::Hashable<T>)
    struct hash<RS::Intervals::Interval<T>> {
        std::size_t operator()(const RS::Intervals::Interval<T>& in) const { return in.hash(); }
    };

}
