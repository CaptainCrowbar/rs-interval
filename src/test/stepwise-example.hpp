#pragma once

#include <compare>
#include <cstddef>
#include <format>
#include <functional>
#include <istream>

class StepwiseType {

public:

    constexpr StepwiseType(int n = 0) noexcept: value_(n) {}
    constexpr int get() const noexcept { return value_; }
    constexpr StepwiseType& operator++() noexcept { ++value_; return *this; }
    constexpr StepwiseType operator++(int) noexcept { auto old = *this; ++value_; return old; }
    constexpr StepwiseType& operator--() noexcept { --value_; return *this; }
    constexpr StepwiseType operator--(int) noexcept { auto old = *this; --value_; return old; }
    constexpr bool operator==(StepwiseType s) const noexcept { return value_ == s.value_; }
    constexpr bool operator!=(StepwiseType s) const noexcept { return value_ != s.value_; }
    constexpr bool operator<(StepwiseType s) const noexcept { return value_ < s.value_; }
    constexpr bool operator>(StepwiseType s) const noexcept { return value_ > s.value_; }
    constexpr bool operator<=(StepwiseType s) const noexcept { return value_ <= s.value_; }
    constexpr bool operator>=(StepwiseType s) const noexcept { return value_ >= s.value_; }

private:

    int value_;

};

constexpr StepwiseType operator""_sw(unsigned long long n) noexcept {
    return StepwiseType(int(n));
}

inline std::istream& operator>>(std::istream& in, StepwiseType& s) {
    int i;
    in >> i;
    s = i;
    return in;
}

template <>
struct std::formatter<StepwiseType>:
std::formatter<int> {
    using base = std::formatter<int>;
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }
    template <typename FormatContext>
    auto format(const StepwiseType& t, FormatContext& ctx) const {
        return base::format(t.get(), ctx);
    }
};

template <>
struct std::hash<StepwiseType> {
public:
    std::size_t operator()(StepwiseType in) const noexcept {
        return std::hash<int>()(in.get());
    }
};
