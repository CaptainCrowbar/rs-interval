#pragma once

#include "rs-interval/interval-type-base.hpp"
#include "rs-interval/types.hpp"
#include "rs-format/format.hpp"
#include <iterator>
#include <string>
#include <type_traits>

namespace RS::Intervals {

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
        public Detail::TotalOrder<iterator> {
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
        public Detail::TotalOrder<iterator> {
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

}
