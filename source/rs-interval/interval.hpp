#pragma once

#include "rs-interval/format.hpp"
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <map>
#include <ostream>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace RS::Intervals {

    // Implementation details

    namespace Detail {

        inline std::pair<IntervalBound, IntervalBound> parse_interval_mode(std::string_view mode) {
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
            return it->second;
        }

    }

    // Forward declarations

    template <typename T> class Interval;
    template <typename T> class IntervalSet;
    template <typename K, typename T> class IntervalMap;

    // Base class for intervals with the same value type

    template <typename T>
    class IntervalTypeBase {
    public:
        const T& min() const noexcept { return min_; }
        const T& max() const noexcept { return max_; }
        IntervalBound left() const noexcept { return left_; }
        IntervalBound right() const noexcept { return right_; }
        bool empty() const noexcept { return left_ == IntervalBound::empty; }
        bool is_single() const noexcept { return left_ == IntervalBound::closed && right_ == IntervalBound::closed && min_ == max_; }
        bool is_finite() const noexcept { return is_left_bounded() && is_right_bounded(); }
        bool is_infinite() const noexcept { return left_ == IntervalBound::unbound || right_ == IntervalBound::unbound; }
        bool is_universal() const noexcept { return left_ == IntervalBound::unbound && right_ == IntervalBound::unbound; }
        bool is_left_bounded() const noexcept { return is_left_open() || is_left_closed(); }
        bool is_left_closed() const noexcept { return left_ == IntervalBound::closed; }
        bool is_left_open() const noexcept { return left_ == IntervalBound::open; }
        bool is_right_bounded() const noexcept { return is_right_open() || is_right_closed(); }
        bool is_right_closed() const noexcept { return right_ == IntervalBound::closed; }
        bool is_right_open() const noexcept { return right_ == IntervalBound::open; }
        size_t hash() const noexcept;
        IntervalMatch match(const T& t) const;
    protected:
        struct boundary_point {
            Detail::BoundaryType flag;
            T value;
            bool operator<(const boundary_point& b) const noexcept
                { return Detail::no_boundary(flag) || Detail::no_boundary(b.flag) || value == b.value ? flag < b.flag : value < b.value; }
        };
        using boundary_points = std::pair<boundary_point, boundary_point>;
        T min_ = T();
        T max_ = T();
        IntervalBound left_ = IntervalBound::empty;
        IntervalBound right_ = IntervalBound::empty;
        void adjust_bounds();
        void do_swap(IntervalTypeBase& in) noexcept;
        boundary_points find_interval_bounds() const noexcept;
        static bool adjacent_intervals(const boundary_point& left, const boundary_point& right) noexcept;
    };

        template <typename T>
        size_t IntervalTypeBase<T>::hash() const noexcept {
            return hash_value(min_, max_, int(left_), int(right_));
        }

        template <typename T>
        IntervalMatch IntervalTypeBase<T>::match(const T& t) const {
            if (empty())                                           return IntervalMatch::empty;
            else if (is_universal())                               return IntervalMatch::match;
            else if (left_ == IntervalBound::closed && t < min_)   return IntervalMatch::low;
            else if (left_ == IntervalBound::open && t <= min_)    return IntervalMatch::low;
            else if (right_ == IntervalBound::closed && t > max_)  return IntervalMatch::high;
            else if (right_ == IntervalBound::open && t >= max_)   return IntervalMatch::high;
            else                                                   return IntervalMatch::match;
        }

        template <typename T>
        void IntervalTypeBase<T>::adjust_bounds() {
            if ((left_ == IntervalBound::empty) != (right_ == IntervalBound::empty)) {
                throw std::invalid_argument("Inconsistent interval bounds");
            } else if (left_ == IntervalBound::empty) {
                min_ = max_ = T();
            } else {
                if (is_finite()) {
                    if (min_ > max_)
                        left_ = right_ = IntervalBound::empty;
                    else if (min_ == max_ && (left_ == IntervalBound::open || right_ == IntervalBound::open))
                        left_ = right_ = IntervalBound::empty;
                }
                if (! is_left_bounded())
                    min_ = T();
                if (! is_right_bounded())
                    max_ = T();
            }
        }

        template <typename T>
        void IntervalTypeBase<T>::do_swap(IntervalTypeBase& in) noexcept {
            using std::swap;
            swap(min_, in.min_);
            swap(max_, in.max_);
            swap(left_, in.left_);
            swap(right_, in.right_);
        }

        template <typename T>
        typename IntervalTypeBase<T>::boundary_points IntervalTypeBase<T>::find_interval_bounds() const noexcept {
            boundary_point lbp, rbp;
            if (left_ == IntervalBound::open)          lbp = {Detail::BoundaryType::value_plus_epsilon, min_};
            else if (left_ == IntervalBound::closed)   lbp = {Detail::BoundaryType::exact_value, min_};
            else                                       lbp = {Detail::BoundaryType::minus_infinity, {}};
            if (right_ == IntervalBound::open)         rbp = {Detail::BoundaryType::value_minus_epsilon, max_};
            else if (right_ == IntervalBound::closed)  rbp = {Detail::BoundaryType::exact_value, max_};
            else                                       rbp = {Detail::BoundaryType::plus_infinity, {}};
            return {lbp, rbp};
        }

        template <typename T>
        bool IntervalTypeBase<T>::adjacent_intervals(const boundary_point& left, const boundary_point& right) noexcept {
            if (Detail::no_boundary(left.flag) || Detail::no_boundary(right.flag))
                return false;
            if (int(right.flag) - int(left.flag) == 1 && left.value == right.value)
                return true;
            if constexpr (interval_category<T> == IntervalCategory::stepwise || interval_category<T> == IntervalCategory::integral) {
                if (left.flag == Detail::BoundaryType::exact_value && right.flag == Detail::BoundaryType::exact_value && left.value < right.value) {
                    T t = left.value;
                    return ++t == right.value;
                }
            }
            return false;
        }

    // Base class for intervals in the same category

    template <typename T, IntervalCategory Cat = interval_category<T>>
    class IntervalCategoryBase;

    template <typename T>
    class IntervalCategoryBase<T, IntervalCategory::ordered>:
    public IntervalTypeBase<T> {};

    // TODO
    template <typename T>
    class IntervalCategoryBase<T, IntervalCategory::stepwise>:
    public IntervalTypeBase<T> {
    private:
        static constexpr bool has_t_arithmetic = Detail::has_plus_operator<T> && Detail::has_minus_operator<T>;
        static constexpr bool has_int_arithmetic = Detail::has_plus_operator<T, int> && Detail::has_minus_operator<T, int>;
        static constexpr bool has_random_access = has_t_arithmetic || has_int_arithmetic;
        static constexpr bool prefer_ptrdiff_arithmetic = has_int_arithmetic && sizeof(T) > sizeof(int);
        using delta_type = std::conditional_t<has_t_arithmetic, T, std::conditional_t<prefer_ptrdiff_arithmetic, ptrdiff_t, int>>;
    public:
        class iterator:
        public Detail::LessThanComparable<iterator> {
        public:
            using difference_type = delta_type;
            using iterator_category = std::conditional_t<has_random_access, std::random_access_iterator_tag, std::bidirectional_iterator_tag>;
            using pointer = const T*;
            using reference = const T&;
            using value_type = T;
            iterator() = default;
            explicit iterator(T t): value_(t) {}
            const T& operator*() const noexcept { return value_; }
            const T* operator->() const noexcept { return &value_; }
            iterator& operator++() { ++value_; return *this; }
            iterator operator++(int) { auto old = *this; ++value_; return old; }
            iterator& operator--() { --value_; return *this; }
            iterator operator--(int) { auto old = *this; --value_; return old; }
            iterator& operator+=(delta_type rhs) { value_ = value_ + delta_type(rhs); return *this; }
            iterator& operator-=(delta_type rhs) { value_ = value_ - delta_type(rhs); return *this; }
            iterator operator+(delta_type rhs) const { auto it = *this; it += rhs; return it; }
            friend iterator operator+(delta_type lhs, const iterator& rhs) { return rhs + lhs; }
            iterator operator-(delta_type rhs) const { auto it = *this; it -= rhs; return it; }
            delta_type operator-(const iterator& rhs) { return delta_type(value_ - rhs.value_); }
            bool operator==(const iterator& rhs) const noexcept { return value_ == rhs.value_; }
            bool operator<(const iterator& rhs) const noexcept { return value_ < rhs.value_; }
        private:
            T value_;
        };
        iterator begin() const { return this->empty() ? iterator() : iterator(this->min_); }
        iterator end() const { return this->empty() ? iterator() : std::next(iterator(this->max_)); }
        size_t size() const;
    protected:
        void adjust_bounds();
    };

        template <typename T>
        size_t IntervalCategoryBase<T, IntervalCategory::stepwise>::size() const {
            if (this->empty())
                return 0;
            if (this->is_infinite())
                return std::string::npos;
            if constexpr (Detail::has_minus_operator<T> && std::is_constructible_v<size_t, T>) {
                return static_cast<size_t>(this->max_ - this->min_) + 1;
            } else {
                size_t n = 1;
                for (T t = this->min_; t != this->max_; ++t, ++n) {}
                return n;
            }
        }

        template <typename T>
        void IntervalCategoryBase<T, IntervalCategory::stepwise>::adjust_bounds() {
            if (this->left_ == IntervalBound::open) {
                ++this->min_;
                this->left_ = IntervalBound::closed;
            }
            if (this->right_ == IntervalBound::open) {
                --this->max_;
                this->right_ = IntervalBound::closed;
            }
            IntervalTypeBase<T>::adjust_bounds();
        }

    // TODO
    template <typename T>
    class IntervalCategoryBase<T, IntervalCategory::integral>:
    public IntervalTypeBase<T> {
    private:
        static constexpr bool has_t_arithmetic = Detail::has_plus_operator<T> && Detail::has_minus_operator<T>;
        static constexpr bool has_int_arithmetic = Detail::has_plus_operator<T, int> && Detail::has_minus_operator<T, int>;
        static constexpr bool has_random_access = has_t_arithmetic || has_int_arithmetic;
        static constexpr bool prefer_ptrdiff_arithmetic = has_int_arithmetic && sizeof(T) > sizeof(int);
        using delta_type = std::conditional_t<has_t_arithmetic, T, std::conditional_t<prefer_ptrdiff_arithmetic, ptrdiff_t, int>>;
    public:
        class iterator:
        public Detail::LessThanComparable<iterator> {
        public:
            using difference_type = delta_type;
            using iterator_category = std::conditional_t<has_random_access, std::random_access_iterator_tag, std::bidirectional_iterator_tag>;
            using pointer = const T*;
            using reference = const T&;
            using value_type = T;
            iterator() = default;
            explicit iterator(T t): value_(t) {}
            const T& operator*() const noexcept { return value_; }
            const T* operator->() const noexcept { return &value_; }
            iterator& operator++() { ++value_; return *this; }
            iterator operator++(int) { auto old = *this; ++value_; return old; }
            iterator& operator--() { --value_; return *this; }
            iterator operator--(int) { auto old = *this; --value_; return old; }
            iterator& operator+=(delta_type rhs) { value_ = value_ + delta_type(rhs); return *this; }
            iterator& operator-=(delta_type rhs) { value_ = value_ - delta_type(rhs); return *this; }
            iterator operator+(delta_type rhs) const { auto it = *this; it += rhs; return it; }
            friend iterator operator+(delta_type lhs, const iterator& rhs) { return rhs + lhs; }
            iterator operator-(delta_type rhs) const { auto it = *this; it -= rhs; return it; }
            delta_type operator-(const iterator& rhs) { return delta_type(value_ - rhs.value_); }
            bool operator==(const iterator& rhs) const noexcept { return value_ == rhs.value_; }
            bool operator<(const iterator& rhs) const noexcept { return value_ < rhs.value_; }
        private:
            T value_;
        };
        iterator begin() const { return this->empty() ? iterator() : iterator(this->min_); }
        iterator end() const { return this->empty() ? iterator() : std::next(iterator(this->max_)); }
        size_t size() const;
    protected:
        void adjust_bounds();
    };

        template <typename T>
        size_t IntervalCategoryBase<T, IntervalCategory::integral>::size() const {
            if (this->empty())
                return 0;
            if (this->is_infinite())
                return std::string::npos;
            if constexpr (Detail::has_minus_operator<T> && std::is_constructible_v<size_t, T>) {
                return static_cast<size_t>(this->max_ - this->min_) + 1;
            } else {
                size_t n = 1;
                for (T t = this->min_; t != this->max_; ++t, ++n) {}
                return n;
            }
        }

        template <typename T>
        void IntervalCategoryBase<T, IntervalCategory::integral>::adjust_bounds() {
            if (this->left_ == IntervalBound::open) {
                ++this->min_;
                this->left_ = IntervalBound::closed;
            }
            if (this->right_ == IntervalBound::open) {
                --this->max_;
                this->right_ = IntervalBound::closed;
            }
            IntervalTypeBase<T>::adjust_bounds();
        }

    template <typename T>
    class IntervalCategoryBase<T, IntervalCategory::continuous>:
    public IntervalTypeBase<T> {
    public:
        T size() const { return this->max_ - this->min_; }
    };

    // Base class for interval arithmetic

    template <typename IntervalType, typename T, IntervalCategory Cat = interval_category<T>>
    class IntervalArithmeticBase {
    public:
        IntervalType operator+() const { auto& a = static_cast<const IntervalType&>(*this); return a; }
        IntervalType operator-() const { auto& a = static_cast<const IntervalType&>(*this); return negative(a); }
        IntervalType& operator+=(const IntervalType& b) { auto& a = static_cast<IntervalType&>(*this); a = add(a, b); return a; }
        IntervalType& operator+=(const T& b) { auto& a = static_cast<IntervalType&>(*this); a = add(a, b); return a; }
        IntervalType& operator-=(const IntervalType& b) { auto& a = static_cast<IntervalType&>(*this); a = subtract(a, b); return a; }
        IntervalType& operator-=(const T& b) { auto& a = static_cast<IntervalType&>(*this); a = subtract(a, b); return a; }
        IntervalType& operator*=(const IntervalType& b) { auto& a = static_cast<IntervalType&>(*this); a = multiply(a, b); return a; }
        IntervalType& operator*=(const T& b) { auto& a = static_cast<IntervalType&>(*this); a = multiply(a, b); return a; }
        IntervalType& operator/=(const IntervalType& b) { auto& a = static_cast<IntervalType&>(*this); a = divide(a, b); return a; }
        IntervalType& operator/=(const T& b) { auto& a = static_cast<IntervalType&>(*this); a = divide(a, b); return a; }
        friend IntervalType operator+(const IntervalType& a, const IntervalType& b) { return add(a, b); }
        friend IntervalType operator+(const IntervalType& a, const T& b) { return add(a, b); }
        friend IntervalType operator+(const T& a, const IntervalType& b) { return add(b, a); }
        friend IntervalType operator-(const IntervalType& a, const IntervalType& b) { return subtract(a, b); }
        friend IntervalType operator-(const IntervalType& a, const T& b) { return subtract(a, b); }
        friend IntervalType operator-(const T& a, const IntervalType& b) { return subtract(a, b); }
        friend IntervalType operator*(const IntervalType& a, const IntervalType& b) { return multiply(a, b); }
        friend IntervalType operator*(const IntervalType& a, const T& b) { return multiply(a, b); }
        friend IntervalType operator*(const T& a, const IntervalType& b) { return multiply(b, a); }
        friend IntervalType operator/(const IntervalType& a, const IntervalType& b) { return divide(a, b); }
        friend IntervalType operator/(const IntervalType& a, const T& b) { return divide(a, b); }
        friend IntervalType operator/(const T& a, const IntervalType& b) { return divide(a, b); }
    private:
        static IntervalType negative(const IntervalType& a);
        static IntervalType add(const IntervalType& a, const IntervalType& b);
        static IntervalType subtract(const IntervalType& a, const IntervalType& b);
        static IntervalType multiply(const IntervalType& a, const IntervalType& b);
        static IntervalType divide(const IntervalType& a, const IntervalType& b);
    };

        template <typename IntervalType, typename T, IntervalCategory Cat>
        IntervalType IntervalArithmeticBase<IntervalType, T, Cat>::negative(const IntervalType& a) {
            return IntervalType(- a.max(), - a.min(), a.right(), a.left());
        }

        template <typename IntervalType, typename T, IntervalCategory Cat>
        IntervalType IntervalArithmeticBase<IntervalType, T, Cat>::add(const IntervalType& a, const IntervalType& b) {
            if (a.empty() || b.empty())
                return {};
            else if (a.is_universal() || b.is_universal())
                return IntervalType::all();
            IntervalBound l = std::max(a.left(), b.left());
            IntervalBound r = std::max(a.right(), b.right());
            T t = l == IntervalBound::unbound ? T() : a.min() + b.min();
            T u = r == IntervalBound::unbound ? T() : a.max() + b.max();
            return IntervalType(t, u, l, r);
        }

        template <typename IntervalType, typename T, IntervalCategory Cat>
        IntervalType IntervalArithmeticBase<IntervalType, T, Cat>::subtract(const IntervalType& a, const IntervalType& b) {
            if (a.empty() || b.empty())
                return {};
            else if (a.is_universal() || b.is_universal())
                return IntervalType::all();
            IntervalBound l = std::max(a.left(), b.right());
            IntervalBound r = std::max(a.right(), b.left());
            T t = l == IntervalBound::unbound ? T() : a.min() - b.max();
            T u = r == IntervalBound::unbound ? T() : a.max() - b.min();
            return IntervalType(t, u, l, r);
        }

        template <typename IntervalType, typename T, IntervalCategory Cat>
        IntervalType IntervalArithmeticBase<IntervalType, T, Cat>::multiply(const IntervalType& a, const IntervalType& b) {
            // if (a.empty() || b.empty())
            //     return {};
            // else if ((a.is_single() && a.min() == T()) || (b.is_single() && b.min() == T()))
            //     return T();
            // else if (a.is_universal() || b.is_universal())
            //     return IntervalType::all();
            // TODO
            (void)a;
            (void)b;
            return {};
        }

        template <typename IntervalType, typename T, IntervalCategory Cat>
        IntervalType IntervalArithmeticBase<IntervalType, T, Cat>::divide(const IntervalType& a, const IntervalType& b) {
            // TODO
            (void)a;
            (void)b;
            return {};
        }

    template <typename IntervalType, typename T>
    class IntervalArithmeticBase<IntervalType, T, IntervalCategory::ordered> {};

    template <typename IntervalType, typename T>
    class IntervalArithmeticBase<IntervalType, T, IntervalCategory::stepwise> {};

    // Interval class

    template <typename T>
    class Interval:
    public IntervalCategoryBase<T>,
    public IntervalArithmeticBase<Interval<T>, T>,
    public Detail::LessThanComparable<Interval<T>> {
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
        IntervalSet<T> inverse() const;
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
        std::string str(const std::string& mode = {}) const;
        void swap(Interval& in) noexcept { this->do_swap(in); }
        static Interval all() noexcept { return Interval(T(), IntervalBound::unbound, IntervalBound::unbound); }
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
            this->min_ = min;
            this->max_ = max;
            std::tie(this->left_, this->right_) = Detail::parse_interval_mode(mode);
            this->adjust_bounds();
        }

        template <typename T>
        IntervalSet<T> Interval<T>::inverse() const {
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
            auto& a = *this;
            if (a.empty() && b.empty())  return IntervalOrder::equal;
            if (a.empty())               return IntervalOrder::b_only;
            if (b.empty())               return IntervalOrder::a_only;
            auto [al, ar] = a.find_interval_bounds();
            auto [bl, br] = b.find_interval_bounds();
            if (ar < bl) {
                if (this->adjacent_intervals(ar, bl))  return IntervalOrder::a_touches_b;
                else                                   return IntervalOrder::a_below_b;
            } else if (br < al) {
                if (this->adjacent_intervals(br, al))  return IntervalOrder::b_touches_a;
                else                                   return IntervalOrder::b_below_a;
            } else if (al < bl) {
                if (ar < br)       return IntervalOrder::a_overlaps_b;
                else if (br < ar)  return IntervalOrder::a_encloses_b;
                else               return IntervalOrder::a_extends_below_b;
            } else if (bl < al) {
                if (ar < br)       return IntervalOrder::b_encloses_a;
                else if (br < ar)  return IntervalOrder::b_overlaps_a;
                else               return IntervalOrder::b_extends_below_a;
            } else {
                if (ar < br)       return IntervalOrder::b_extends_above_a;
                else if (br < ar)  return IntervalOrder::a_extends_above_b;
                else               return IntervalOrder::equal;
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
                return ord == IntervalOrder::a_extends_above_b || ord == IntervalOrder::a_extends_below_b || ord == IntervalOrder::a_encloses_b;
        }

        template <typename T>
        bool Interval<T>::overlaps(const Interval& b) const {
            IntervalOrder ord = order(b);
            if (ord == IntervalOrder::equal)
                return ! this->empty();
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
            if (ord == IntervalOrder::equal)
                return ! this->empty();
            else if (ord == IntervalOrder::a_only || ord == IntervalOrder::b_only || ord == IntervalOrder::a_below_b || ord == IntervalOrder::b_below_a)
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
        std::string Interval<T>::str(const std::string& mode) const {
            using namespace Detail;
            if (this->empty())
                return "{}";
            else if (this->is_universal())
                return "*";
            else if (this->is_single())
                return format_object(this->min(), mode);
            else if (! this->is_left_bounded())
                return (this->is_right_closed() ? "<=" : "<") + format_object(this->max(), mode);
            else if (! this->is_right_bounded())
                return (this->is_left_closed() ? ">=" : ">") + format_object(this->min(), mode);
            else
                return (this->is_left_closed() ? '[' : '(') + format_object(this->min(), mode) + ','
                    + format_object(this->max(), mode) + (this->is_right_closed() ? ']' : ')');
        }

    template <typename T> bool operator==(const Interval<T>& a, const Interval<T>& b) noexcept { return a.compare(b) == 0; }
    template <typename T> bool operator<(const Interval<T>& a, const Interval<T>& b) noexcept { return a.compare(b) < 0; }
    template <typename T> std::ostream& operator<<(std::ostream& out, const Interval<T>& in) { return out << in.str(); }
    template <typename T> Interval<T> make_interval(const T& t) { return Interval<T>(t); }
    template <typename T> Interval<T> make_interval(const T& t, IntervalBound l, IntervalBound r) { return Interval<T>(t, l, r); }
    template <typename T> Interval<T> make_interval(const T& min, const T& max, IntervalBound lr = IntervalBound::closed) { return Interval<T>(min, max, lr); }
    template <typename T> Interval<T> make_interval(const T& min, const T& max, IntervalBound l, IntervalBound r) { return Interval<T>(min, max, l, r); }
    template <typename T> Interval<T> make_interval(const T& min, const T& max, std::string_view mode) { return Interval<T>(min, max, mode); }
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
            // TODO
            return hash_range(con_);
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

    // Interval map

    template <typename K, typename T>
    class IntervalMap:
    public Detail::LessThanComparable<IntervalMap<K, T>> {
    public:
        using key_type = K;
        using mapped_type = T;
        using interval_type = Interval<K>;
    private:
        using container_type = std::map<interval_type, T>;
    public:
        using iterator = typename container_type::const_iterator;
        using value_type = typename container_type::value_type;
        static constexpr auto category = interval_category<K>;
        IntervalMap() = default;
        explicit IntervalMap(const T& defval): con_(), def_(defval) {}
        IntervalMap(std::initializer_list<value_type> list) { for (auto& v: list) insert(v); }
        const T& operator[](const K& key) const;
        auto begin() const noexcept { return con_.begin(); }
        auto end() const noexcept { return con_.end(); }
        bool empty() const noexcept { return con_.empty(); }
        size_t size() const noexcept { return con_.size(); }
        const T& default_value() const noexcept { return def_; }
        void default_value(const T& defval) { def_ = defval; }
        bool contains(const K& key) const { return do_find(key).second; }
        iterator find(const K& key) const;
        iterator lower_bound(const K& key) const { return do_find(key).first; }
        iterator upper_bound(const K& key) const;
        void clear() noexcept { con_.clear(); }
        void reset(const T& defval = {}) { def_ = defval; clear(); }
        void insert(const interval_type& in, const T& t);
        void insert(const value_type& v) { insert(v.first, v.second); }
        void erase(const interval_type& in);
        size_t hash() const noexcept;
        std::string str() const { return str({}, {}); }
        std::string str(const std::string& kmode, const std::string& vmode) const;
        void swap(IntervalMap& map) noexcept { con_.swap(map.con_); std::swap(def_, map.def_); }
    private:
        container_type con_;
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
            auto i = con_.upper_bound(key);
            if (i != con_.begin())
                --i;
            std::vector<value_type> add;
            std::vector<iterator> del;
            for (; i != con_.end(); ++i) {
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
                con_.erase(d);
            for (auto& a: add)
                con_.insert(a);
            con_.insert({key, t});
        }

        template <typename K, typename T>
        void IntervalMap<K, T>::erase(const interval_type& in) {
            if (empty() || in.empty())
                return;
            auto i = con_.lower_bound(in);
            if (i != con_.begin())
                --i;
            std::vector<value_type> vec;
            while (i != con_.end()) {
                auto ord = in.order(i->first);
                if (ord <= IntervalOrder::a_touches_b)
                    break;
                auto j = i++;
                if (ord <= IntervalOrder::b_overlaps_a) {
                    auto temp = j->first.set_difference(in);
                    for (auto& t: temp)
                        vec.push_back({t, j->second});
                    con_.erase(j);
                }
            }
            con_.insert(vec.begin(), vec.end());
        }

        template <typename K, typename T>
        size_t IntervalMap<K, T>::hash() const noexcept {
            size_t h = std::hash<T>()(def_);
            for (auto& [k,t]: con_)
                hash_combine(h, k, t);
            return h;
        }

        template <typename K, typename T>
        std::string IntervalMap<K, T>::str(const std::string& kmode, const std::string& vmode) const {
            using namespace Detail;
            if (empty())
                return "{}";
            std::string s = "{";
            for (auto& [k,v]: *this)
                s += format_object(k, kmode) + ':'+ format_object(v, vmode) + ',';
            s.back() = '}';
            return s;
        }

        template <typename K, typename T>
        std::pair<typename IntervalMap<K, T>::iterator, bool> IntervalMap<K, T>::do_find(const K& key) const {
            if (empty())
                return {end(), false};
            interval_type in(key);
            auto it = con_.lower_bound(in);
            if (it != con_.begin())
                --it;
            for (; it != con_.end(); ++it) {
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

    template <typename T>
    class hash<RS::Intervals::Interval<T>> {
    public:
        size_t operator()(const RS::Intervals::Interval<T>& in) const noexcept { return in.hash(); }
    };

    template <typename T>
    class hash<RS::Intervals::IntervalSet<T>> {
    public:
        size_t operator()(const RS::Intervals::IntervalSet<T>& set) const noexcept { return set.hash(); }
    };

    template <typename K, typename T>
    class hash<RS::Intervals::IntervalMap<K, T>> {
    public:
        size_t operator()(const RS::Intervals::IntervalMap<K, T>& map) const noexcept { return map.hash(); }
    };

}
