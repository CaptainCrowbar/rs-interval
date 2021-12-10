#pragma once

#include "rs-interval/format.hpp"
#include <algorithm>
#include <array>
#include <functional>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace RS::Intervals {

    // Forward declarations

    template <typename T> class Interval;
    template <typename T> class IntervalSet;
    template <typename K, typename T> class IntervalMap;

    // Implementation details

    namespace Detail {

        constexpr size_t hash_mix(size_t h1, size_t h2) noexcept {
            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }

        template <typename T>
        size_t std_hash(const T& t) noexcept {
            return std::hash<T>()(t);
        }

        template <typename Range>
        size_t hash_range(const Range& range) noexcept {
            size_t h = 0;
            for (auto& x: range)
                h = hash_mix(h, std_hash(x));
            return h;
        }

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

        struct boundary_point:
        public Detail::LessThanComparable<boundary_point> {
            T value;
            Detail::BoundaryType flag;
            boundary_point() = default;
            boundary_point(T v, Detail::BoundaryType f): value(v), flag(f) {}
            bool operator==(const boundary_point& b) const noexcept {
                return flag == b.flag && (Detail::no_boundary(flag) || value == b.value);
            }
            bool operator<(const boundary_point& b) const noexcept {
                if (Detail::no_boundary(flag) || Detail::no_boundary(b.flag) || value == b.value)
                    return flag < b.flag;
                else
                    return value < b.value;
            }
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
            using namespace Detail;
            size_t h = std::hash<T>()(min_);
            h = hash_mix(h, std::hash<T>()(max_));
            h = hash_mix(h, std::hash<int>()(int(left_)));
            h = hash_mix(h, std::hash<int>()(int(right_)));
            return h;
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
            if (left_ == IntervalBound::open)          lbp = {min_, Detail::BoundaryType::open_left};
            else if (left_ == IntervalBound::closed)   lbp = {min_, Detail::BoundaryType::exact_value};
            else                                       lbp = {{}, Detail::BoundaryType::unbounded_left};
            if (right_ == IntervalBound::open)         rbp = {max_, Detail::BoundaryType::open_right};
            else if (right_ == IntervalBound::closed)  rbp = {max_, Detail::BoundaryType::exact_value};
            else                                       rbp = {{}, Detail::BoundaryType::unbounded_right};
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
            if (a.empty() || b.empty())
                return {};
            if (a.is_universal() || b.is_universal())
                return IntervalType::all();
            T x = a.min() + b.min();
            T y = a.max() + b.max();
            IntervalBound lb = std::max(a.left(), b.left());
            IntervalBound rb = std::max(a.right(), b.right());
            return IntervalType(x, y, lb, rb);
        }

        template <typename IntervalType, typename T, IntervalCategory Cat>
        IntervalType IntervalArithmeticBase<IntervalType, T, Cat>::subtract_intervals(const IntervalType& a, const IntervalType& b) {
            return add_intervals(a, negative_interval(b));
        }

        template <typename IntervalType, typename T, IntervalCategory Cat>
        IntervalType IntervalArithmeticBase<IntervalType, T, Cat>::multiply_intervals(const IntervalType& a, const IntervalType& b) {

            if (a.empty() || b.empty()) {

                // Both intervals are empty

                return {};

            } else if ((a.is_single() && a.min() == T()) || (b.is_single() && b.min() == T())) {

                // One of the intervals is singular zero

                return T();

            } else if (a.is_universal() || b.is_universal()) {

                // One of the intervals is universal

                return IntervalType::all();

            } else if (a.is_finite() && b.is_finite()) {

                // Both intervals are finite

                std::array<std::pair<T, bool>, 4> points; // (value,closed)
                points[0].first = a.min() * b.min();
                points[1].first = a.min() * b.max();
                points[2].first = a.max() * b.min();
                points[3].first = a.max() * b.max();
                points[0].second = a.is_left_closed() && b.is_left_closed();
                points[1].second = a.is_left_closed() && b.is_right_closed();
                points[2].second = a.is_right_closed() && b.is_left_closed();
                points[3].second = a.is_right_closed() && b.is_right_closed();
                T min_value = points[0].first;
                T max_value = min_value;
                T min_closed = points[0].second;
                T max_closed = min_closed;

                for (int i = 1; i < 4; ++i) {
                    if (points[i].first < min_value) {
                        min_value = points[i].first;
                        min_closed = points[i].second;
                    } else if (points[i].first == min_value) {
                        min_closed = min_closed || points[i].second;
                    }
                    if (points[i].first > max_value) {
                        max_value = points[i].first;
                        max_closed = points[i].second;
                    } else if (points[i].first == max_value) {
                        max_closed = max_closed || points[i].second;
                    }
                }

                auto lbound = min_closed ? IntervalBound::closed : IntervalBound::open;
                auto rbound = max_closed ? IntervalBound::closed : IntervalBound::open;

                return IntervalType(min_value, max_value, lbound, rbound);

            } else if (b.is_finite()) {

                // LHS is infinite, RHS is finite

                return multiply_intervals(b, a);

            } else if (b.is_right_bounded()) {

                // RHS is unbounded below

                return multiply_intervals(- a, - b);

            } else if (! a.is_left_bounded() && a.is_right_bounded()) {

                // LHS is unbounded below

                return - multiply_intervals(- a, b);

            } else if (a.is_finite()) {

                // LHS is finite, RHS is unbounded above
                // TODO

                return {};

            } else {

                // Both intervals are unbounded above
                // TODO

                return {};

            }

        }

        template <typename IntervalType, typename T, IntervalCategory Cat>
        IntervalType IntervalArithmeticBase<IntervalType, T, Cat>::divide_intervals(const IntervalType& a, const IntervalType& b) {
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

}

namespace std {

    template <typename T>
    class hash<RS::Intervals::Interval<T>> {
    public:
        size_t operator()(const RS::Intervals::Interval<T>& in) const noexcept { return in.hash(); }
    };

}
