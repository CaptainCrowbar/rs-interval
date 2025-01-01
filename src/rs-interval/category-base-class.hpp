// This header is private to the implementation and should not be included by users

#pragma once

#include "rs-interval/interval-base-class.hpp"
#include "rs-interval/types.hpp"
#include <compare>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <limits>
#include <type_traits>

namespace RS::Interval {

    namespace Detail {

        template <typename T>
        concept SelfArithmeticIntervalType = requires (T t) {
            { t + t } -> std::convertible_to<T>;
            { t - t } -> std::convertible_to<T>;
        };

        template <typename T>
        concept IntArithmeticIntervalType = requires (T t) {
            { t + 1 } -> std::convertible_to<T>;
            { t - 1 } -> std::convertible_to<T>;
        };

        template <typename T>
        concept PtrdiffArithmeticIntervalType = IntArithmeticIntervalType<T>
            && (sizeof(T) > sizeof(int));

        template <typename T>
        concept RandomAccessIntervalType = SelfArithmeticIntervalType<T>
            || IntArithmeticIntervalType<T>;

        template <typename T>
        concept DifferenceIntervalType = requires (T t) {
            { t - t } -> std::convertible_to<std::size_t>;
        };

    }

    // Base class for intervals in the same category

    template <IntervalCompatible T, Category Cat = interval_category<T>>
    class IntervalCategoryBase;

    template <IntervalCompatible T>
    class IntervalCategoryBase<T, Category::ordered>:
    public IntervalTypeBase<T> {};

    template <IntervalCompatible T>
    class IntervalCategoryBase<T, Category::stepwise>:
    public IntervalTypeBase<T> {

    private:

        using delta_type = std::conditional_t<Detail::SelfArithmeticIntervalType<T>, T,
            std::conditional_t<Detail::PtrdiffArithmeticIntervalType<T>, std::ptrdiff_t, int>>;

    public:

        class iterator {

        public:

            using difference_type = delta_type;
            using iterator_category = std::conditional_t<Detail::RandomAccessIntervalType<T>,
                std::random_access_iterator_tag, std::bidirectional_iterator_tag>;
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
            auto operator<=>(const iterator& rhs) const noexcept = default;

        private:

            T value_;

        };

        iterator begin() const { return this->empty() ? iterator() : iterator(this->min_); }
        iterator end() const { return this->empty() ? iterator() : std::next(iterator(this->max_)); }
        std::size_t size() const;

    protected:

        void adjust_bounds();

    };

        template <IntervalCompatible T>
        std::size_t IntervalCategoryBase<T, Category::stepwise>::size() const {
            if (this->empty()) {
                return 0;
            } else if (this->is_infinite()) {
                return npos;
            } else if constexpr (Detail::DifferenceIntervalType<T>) {
                return std::size_t(this->max_ - this->min_) + 1;
            } else {
                auto n = 1uz;
                for (T t = this->min_; t != this->max_; ++t, ++n) {}
                return n;
            }
        }

        template <IntervalCompatible T>
        void IntervalCategoryBase<T, Category::stepwise>::adjust_bounds() {
            if (this->left_ == Bound::open) {
                ++this->min_;
                this->left_ = Bound::closed;
            }
            if (this->right_ == Bound::open) {
                --this->max_;
                this->right_ = Bound::closed;
            }
            IntervalTypeBase<T>::adjust_bounds();
        }

    template <IntervalCompatible T>
    class IntervalCategoryBase<T, Category::integral>:
    public IntervalTypeBase<T> {

    private:

        using delta_type = std::conditional_t<
            Detail::SelfArithmeticIntervalType<T>, T,
            std::conditional_t<Detail::PtrdiffArithmeticIntervalType<T>, std::ptrdiff_t, int>
        >;

    public:

        class iterator {

        public:

            using difference_type = delta_type;
            using iterator_category = std::conditional_t<Detail::RandomAccessIntervalType<T>,
                std::random_access_iterator_tag, std::bidirectional_iterator_tag>;
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
            auto operator<=>(const iterator& rhs) const noexcept = default;

        private:

            T value_;

        };

        iterator begin() const { return this->empty() ? iterator() : iterator(this->min_); }
        iterator end() const { return this->empty() ? iterator() : std::next(iterator(this->max_)); }
        std::size_t size() const;

    protected:

        void adjust_bounds();

    };

        template <IntervalCompatible T>
        std::size_t IntervalCategoryBase<T, Category::integral>::size() const {
            if (this->empty()) {
                return 0;
            } else if (this->is_infinite()) {
                return npos;
            } else if constexpr (Detail::DifferenceIntervalType<T>) {
                return std::size_t(this->max_ - this->min_) + 1;
            } else {
                std::size_t n = 1;
                for (T t = this->min_; t != this->max_; ++t, ++n) {}
                return n;
            }
        }

        template <IntervalCompatible T>
        void IntervalCategoryBase<T, Category::integral>::adjust_bounds() {
            if (this->left_ == Bound::open) {
                ++this->min_;
                this->left_ = Bound::closed;
            }
            if (this->right_ == Bound::open) {
                --this->max_;
                this->right_ = Bound::closed;
            }
            IntervalTypeBase<T>::adjust_bounds();
        }

    template <IntervalCompatible T>
    class IntervalCategoryBase<T, Category::continuous>:
    public IntervalTypeBase<T> {
    public:
        T size() const {
            if (this->is_infinite()) {
                return std::numeric_limits<T>::infinity();
            } else {
                return this->max_ - this->min_;
            }
        }
    };

}
