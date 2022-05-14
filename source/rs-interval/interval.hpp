#pragma once

#include "rs-interval/interval-arithmetic.hpp"
#include "rs-interval/interval-category-base.hpp"
#include "rs-interval/interval-type-base.hpp"
#include "rs-interval/types.hpp"
#include "rs-format/format.hpp"
#include "rs-tl/types.hpp"
#include <algorithm>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace RS::Intervals {

    template <typename T> class IntervalSet;

    // Interval class

    template <typename T>
    class Interval:
    public IntervalCategoryBase<T>,
    public TL::TotalOrder<Interval<T>> {

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
        int compare(const Interval& b) const noexcept;
        bool includes(const Interval& b) const;      // True if b is a subset of this
        bool overlaps(const Interval& b) const;      // True if the intersection is not empty
        bool touches(const Interval& b) const;       // True if there is no gap between this and b
        Interval envelope(const Interval& b) const;  // Spanning interval, including any gap
        Interval set_intersection(const Interval& b) const;
        IntervalSet<T> set_union(const Interval& b) const;
        IntervalSet<T> set_difference(const Interval& b) const;
        IntervalSet<T> set_symmetric_difference(const Interval& b) const;
        std::string str(const FormatSpec& spec = {}) const;
        void swap(Interval& in) noexcept { this->do_swap(in); }

        static Interval all() noexcept { return Interval(T(), IntervalBound::unbound, IntervalBound::unbound); }
        static Interval from_string(const std::string& str);

    };

        template <typename T>
        Interval<T>::Interval(const T& min, const T& max, IntervalBound l, IntervalBound r) {
            this->min_ = min;
            this->max_ = max;
            this->left_ = l;
            this->right_ = r;
            this->adjust_bounds();
        }

        template <typename T>
        Interval<T>::Interval(const T& min, const T& max, std::string_view mode) {

            using namespace std::literals;

            static const std::unordered_map<std::string_view, std::pair<IntervalBound, IntervalBound>> map = {
                { "()"sv,  { IntervalBound::open,     IntervalBound::open     }},
                { "(]"sv,  { IntervalBound::open,     IntervalBound::closed   }},
                { "[)"sv,  { IntervalBound::closed,   IntervalBound::open     }},
                { "[]"sv,  { IntervalBound::closed,   IntervalBound::closed   }},
                { ">"sv,   { IntervalBound::open,     IntervalBound::unbound  }},
                { "<"sv,   { IntervalBound::unbound,  IntervalBound::open     }},
                { ">="sv,  { IntervalBound::closed,   IntervalBound::unbound  }},
                { "<="sv,  { IntervalBound::unbound,  IntervalBound::closed   }},
                { "*"sv,   { IntervalBound::unbound,  IntervalBound::unbound  }},
            };

            auto it = map.find(mode);
            if (it == map.end())
                throw std::invalid_argument("Invalid interval mode: " + std::string(mode));

            this->min_ = min;
            this->max_ = max;
            this->left_ = it->second.first;
            this->right_ = it->second.second;
            this->adjust_bounds();

        }

        template <typename T>
        IntervalSet<T> Interval<T>::complement() const {
            if (this->empty())
                return all();
            if (this->is_universal())
                return {};
            IntervalSet<T> set;
            if (this->is_left_bounded())
                set.insert({this->min(), IntervalBound::unbound, ~ this->left()});
            if (this->is_right_bounded())
                set.insert({this->max(), ~ this->right(), IntervalBound::unbound});
            return set;
        }

        template <typename T>
        IntervalOrder Interval<T>::order(const Interval& b) const {

            using namespace Detail;

            using B = Boundary<T>;

            auto& a = *this;

            if (a.empty() && b.empty())
                return IntervalOrder::both_empty;
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

        template <typename T>
        int Interval<T>::compare(const Interval& b) const noexcept {

            auto& a = *this;

            if (a.empty() || b.empty())
                return int(b.empty()) - int(a.empty());

            if (a.is_left_bounded() && b.is_left_bounded()) {
                if (a.min() > b.min())
                    return 1;
                else if (a.min() < b.min())
                    return -1;
                else if (a.is_left_closed() != b.is_left_closed())
                    return a.is_left_closed() ? -1 : 1;
            } else if (a.is_left_bounded() || b.is_left_bounded()) {
                return a.is_left_bounded() ? 1 : -1;
            }

            if (a.is_right_bounded() && b.is_right_bounded()) {
                if (a.max() < b.max())
                    return -1;
                else if (a.max() > b.max())
                    return 1;
                else if (a.is_right_closed() != b.is_right_closed())
                    return a.is_right_closed() ? 1 : -1;
            } else if (a.is_right_bounded() || b.is_right_bounded()) {
                return a.is_right_bounded() ? -1 : 1;
            }

            return 0;

        }

        template <typename T>
        bool Interval<T>::includes(const Interval& b) const {
            IntervalOrder ord = order(b);
            if (ord == IntervalOrder::equal)
                return ! this->empty();
            else
                return ord == IntervalOrder::a_extends_above_b || ord == IntervalOrder::a_extends_below_b
                    || ord == IntervalOrder::a_encloses_b;
        }

        template <typename T>
        bool Interval<T>::overlaps(const Interval& b) const {
            IntervalOrder ord = order(b);
            if (ord == IntervalOrder::both_empty)
                return false;
            else if (ord == IntervalOrder::equal)
                return true;
            else if (ord == IntervalOrder::a_only || ord == IntervalOrder::b_only
                    || ord == IntervalOrder::a_below_b || ord == IntervalOrder::b_below_a
                    || ord == IntervalOrder::a_touches_b || ord == IntervalOrder::b_touches_a)
                return false;
            else
                return true;
        }

        template <typename T>
        bool Interval<T>::touches(const Interval& b) const {
            IntervalOrder ord = order(b);
            if (ord == IntervalOrder::both_empty)
                return false;
            else if (ord == IntervalOrder::equal)
                return true;
            else if (ord == IntervalOrder::a_only || ord == IntervalOrder::b_only
                    || ord == IntervalOrder::a_below_b || ord == IntervalOrder::b_below_a)
                return false;
            else
                return true;
        }

        template <typename T>
        Interval<T> Interval<T>::envelope(const Interval& b) const {
            auto& a = *this;
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
                case IntervalOrder::b_only:
                    return b;
                default:
                    return a;
            }
        }

        template <typename T>
        Interval<T> Interval<T>::set_intersection(const Interval& b) const {
            auto& a = *this;
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

        template <typename T>
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

        template <typename T>
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

        template <typename T>
        IntervalSet<T> Interval<T>::set_symmetric_difference(const Interval& b) const {
            auto& a = *this;
            switch (order(b)) {
                case IntervalOrder::b_only:             return {b};
                case IntervalOrder::a_below_b:          return {a, b};
                case IntervalOrder::a_touches_b:        return {{a.min(), b.max(), a.left(), b.right()}};
                case IntervalOrder::a_overlaps_b:       return {{a.min(), b.min(), a.left(), ~ b.left()}, {a.max(), b.max(), ~ a.right(), b.right()}};
                case IntervalOrder::a_extends_below_b:  return {{a.min(), b.min(), a.left(), ~ b.left()}};
                case IntervalOrder::a_encloses_b:       return {{a.min(), b.min(), a.left(), ~ b.left()}, {b.max(), a.max(), ~ b.right(), a.right()}};
                case IntervalOrder::b_extends_above_a:  return {{a.max(), b.max(), ~ a.right(), b.right()}};
                case IntervalOrder::a_extends_above_b:  return {{b.max(), a.max(), ~ b.right(), a.right()}};
                case IntervalOrder::b_encloses_a:       return {{b.min(), a.min(), b.left(), ~ a.left()}, {a.max(), b.max(), ~ a.right(), b.right()}};
                case IntervalOrder::b_extends_below_a:  return {{b.min(), a.min(), b.left(), ~ a.left()}};
                case IntervalOrder::b_overlaps_a:       return {{b.min(), a.min(), b.left(), ~ a.left()}, {b.max(), a.max(), ~ b.right(), a.right()}};
                case IntervalOrder::b_touches_a:        return {{b.min(), a.max(), b.left(), a.right()}};
                case IntervalOrder::b_below_a:          return {b, a};
                case IntervalOrder::a_only:             return {a};
                default:                                return {};
            }
        }

        template <typename T>
        std::string Interval<T>::str(const FormatSpec& spec) const {
            using namespace RS::Format;
            if (this->empty())
                return "{}";
            else if (this->is_universal())
                return "*";
            else if (this->is_single())
                return format_object(this->min(), spec);
            else if (! this->is_left_bounded())
                return (this->is_right_closed() ? "<=" : "<") + format_object(this->max(), spec);
            else if (! this->is_right_bounded())
                return (this->is_left_closed() ? ">=" : ">") + format_object(this->min(), spec);
            else
                return (this->is_left_closed() ? '[' : '(') + format_object(this->min(), spec) + ','
                    + format_object(this->max(), spec) + (this->is_right_closed() ? ']' : ')');
        }

        template <typename T>
        Interval<T> Interval<T>::from_string(const std::string& str) {

            const auto fail = [&] { throw std::invalid_argument("Invalid interval: " + Format::quote(str)); };

            if (str.empty() || str == "{}")
                return {};
            else if (str == "*")
                return Interval::all();

            char first = str.front();
            char last = str.back();
            T min, max;
            std::string mode;

            if ((first == '(' || first == '[') && (last == ')' || last == ']')) {

                size_t comma = str.find(',');

                if (comma == TL::npos)
                    fail();

                min = Detail::from_string<T>(str.substr(1, comma - 1));
                max = Detail::from_string<T>(str.substr(comma + 1, str.size() - comma - 2));
                mode = {first, last};

            } else if (first == '<' || first == '>') {

                mode = str.substr(0, size_t(str[1] == '=') + 1);
                min = max = Detail::from_string<T>(str.substr(mode.size()));

            } else if (last == '+' || last == '-') {

                mode = last == '+' ? ">=" : "<=";
                min = max = Detail::from_string<T>(str.substr(0, str.size() - 1));

            } else {

                static const std::vector<std::string> delimiters = { "..<", "...", "..", "-" };

                mode = "[]";
                size_t i = i = str.find('-');
                size_t j = TL::npos;

                if (i == TL::npos) {
                    i = str.find("..");
                    if (i == TL::npos)
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

    template <typename T> bool operator==(const Interval<T>& a, const Interval<T>& b) noexcept { return a.compare(b) == 0; }
    template <typename T> bool operator<(const Interval<T>& a, const Interval<T>& b) noexcept { return a.compare(b) < 0; }
    template <typename T> std::ostream& operator<<(std::ostream& out, const Interval<T>& in) { return out << in.str(); }

    template <typename T> Interval<T> make_interval(const T& t) { return Interval<T>(t); }
    template <typename T> Interval<T> make_interval(const T& t, IntervalBound l, IntervalBound r)
        { return Interval<T>(t, l, r); }
    template <typename T> Interval<T> make_interval(const T& min, const T& max, IntervalBound lr = IntervalBound::closed)
        { return Interval<T>(min, max, lr); }
    template <typename T> Interval<T> make_interval(const T& min, const T& max, IntervalBound l, IntervalBound r)
        { return Interval<T>(min, max, l, r); }
    template <typename T> Interval<T> make_interval(const T& min, const T& max, std::string_view mode)
        { return Interval<T>(min, max, mode); }
    template <typename T> void swap(Interval<T>& a, Interval<T>& b) { a.swap(b); }

    template <typename T>
    Interval<T> ordered_interval(T a, T b, IntervalBound lr = IntervalBound::closed) {
        if (b < a)
            std::swap(a, b);
        return Interval<T>(a, b, lr);
    }

    template <typename T>
    Interval<T> ordered_interval(T a, T b, IntervalBound l, IntervalBound r) {
        if (b < a) {
            std::swap(a, b);
            std::swap(l, r);
        }
        return Interval<T>(a, b, l, r);
    }

}

namespace std {

    template <typename T>
    class hash<RS::Intervals::Interval<T>> {
    public:
        size_t operator()(const RS::Intervals::Interval<T>& in) const noexcept { return in.hash(); }
    };

}
