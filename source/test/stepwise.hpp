#pragma once

#include <functional>
#include <ostream>

class Stepwise {
public:
    constexpr Stepwise(int n = 0) noexcept: value_(n) {}
    constexpr int get() const noexcept { return value_; }
    constexpr Stepwise& operator++() noexcept { ++value_; return *this; }
    constexpr Stepwise operator++(int) noexcept { auto old = *this; ++value_; return old; }
    constexpr Stepwise& operator--() noexcept { --value_; return *this; }
    constexpr Stepwise operator--(int) noexcept { auto old = *this; --value_; return old; }
    constexpr bool operator==(Stepwise s) const noexcept { return value_ == s.value_; }
    constexpr bool operator!=(Stepwise s) const noexcept { return value_ != s.value_; }
    constexpr bool operator<(Stepwise s) const noexcept { return value_ < s.value_; }
    constexpr bool operator>(Stepwise s) const noexcept { return value_ > s.value_; }
    constexpr bool operator<=(Stepwise s) const noexcept { return value_ <= s.value_; }
    constexpr bool operator>=(Stepwise s) const noexcept { return value_ >= s.value_; }
private:
    int value_;
};

constexpr Stepwise operator""_sw(unsigned long long n) noexcept { return Stepwise(int(n)); }
inline std::ostream& operator<<(std::ostream& out, Stepwise s) { return out << s.get(); }

namespace std {

    template <>
    class hash<Stepwise> {
    public:
        size_t operator()(Stepwise in) const noexcept { return std::hash<int>()(in.get()); }
    };

}
