// This header is private to the implementation and should not be included by users

#pragma once

#include "rs-intervals/types.hpp"
#include <algorithm>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <type_traits>

namespace RS::Intervals {

    namespace Detail {

        constexpr std::size_t hash_mix(std::size_t h1, std::size_t h2) noexcept {
            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }

    }

    // Base class for intervals with the same value type

    template <IntervalCompatible T>
    class IntervalTypeBase {

    public:

        const T& min() const noexcept { return min_; }
        const T& max() const noexcept { return max_; }
        IntervalBound left() const noexcept { return left_; }
        IntervalBound right() const noexcept { return right_; }
        bool empty() const noexcept { return left_ == IntervalBound::empty; }
        bool is_single() const noexcept { return left_ == IntervalBound::closed && right_ == IntervalBound::closed && min_ == max_; }
        bool is_range() const noexcept { return left_ == IntervalBound::unbound || right_ == IntervalBound::unbound || min_ != max_; }
        bool is_finite() const noexcept { return is_left_bounded() && is_right_bounded(); }
        bool is_infinite() const noexcept { return left_ == IntervalBound::unbound || right_ == IntervalBound::unbound; }
        bool is_universal() const noexcept { return left_ == IntervalBound::unbound && right_ == IntervalBound::unbound; }
        bool is_left_bounded() const noexcept { return is_left_open() || is_left_closed(); }
        bool is_left_closed() const noexcept { return left_ == IntervalBound::closed; }
        bool is_left_open() const noexcept { return left_ == IntervalBound::open; }
        bool is_right_bounded() const noexcept { return is_right_open() || is_right_closed(); }
        bool is_right_closed() const noexcept { return right_ == IntervalBound::closed; }
        bool is_right_open() const noexcept { return right_ == IntervalBound::open; }
        std::enable_if_t<Detail::Hashable<T>, std::size_t> hash() const noexcept;
        IntervalMatch match(const T& t) const;

    protected:

        T min_ = T();
        T max_ = T();
        IntervalBound left_ = IntervalBound::empty;
        IntervalBound right_ = IntervalBound::empty;

        void adjust_bounds();
        void do_swap(IntervalTypeBase& in) noexcept;

    };

        template <IntervalCompatible T>
        std::enable_if_t<Detail::Hashable<T>, std::size_t>
        IntervalTypeBase<T>::hash() const noexcept {
            using namespace Detail;
            std::size_t h = std::hash<T>()(min_);
            h = hash_mix(h, std::hash<T>()(max_));
            h = hash_mix(h, std::hash<int>()(int(left_)));
            h = hash_mix(h, std::hash<int>()(int(right_)));
            return h;
        }

        template <IntervalCompatible T>
        IntervalMatch IntervalTypeBase<T>::match(const T& t) const {
            if (empty())                                           return IntervalMatch::empty;
            else if (is_universal())                               return IntervalMatch::match;
            else if (left_ == IntervalBound::closed && t < min_)   return IntervalMatch::low;
            else if (left_ == IntervalBound::open && t <= min_)    return IntervalMatch::low;
            else if (right_ == IntervalBound::closed && t > max_)  return IntervalMatch::high;
            else if (right_ == IntervalBound::open && t >= max_)   return IntervalMatch::high;
            else                                                   return IntervalMatch::match;
        }

        template <IntervalCompatible T>
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

        template <IntervalCompatible T>
        void IntervalTypeBase<T>::do_swap(IntervalTypeBase& in) noexcept {
            using std::swap;
            swap(min_, in.min_);
            swap(max_, in.max_);
            swap(left_, in.left_);
            swap(right_, in.right_);
        }

}
