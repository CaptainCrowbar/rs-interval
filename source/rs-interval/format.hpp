#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream> // TEST
#include <iterator>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

namespace RS::Intervals {

    // Format spec is "<mode>[<options>][<prec>]"

    //  Integer
    //      Default is "n1"
    //      Floating point specs can also be used
    //      Mode:
    //          N,n = Decimal
    //          X,x = Hexadecimal
    //      Options:
    //          s = Always show a sign

    //  Floating point
    //      Default is "gz6"
    //      Mode:
    //          D,d = Decimal format (prec = significant digits)
    //          E,e = Exponential (scientific) format (prec = significant digits)
    //          F,f = Fixed point format (prec = decimal places)
    //          G,g = Use D/d if 1e-3<=|value|<1e6 or value=0, otherwise E/e
    //      Options:
    //          s = Always show a sign
    //          S = Always show a sign on exponent
    //          z = Strip trailing zeroes

    template <typename T> std::string format_object(const T& t, const std::string& spec = {});

    namespace Detail {

        template <typename T, typename = void> struct HasStrMethod: std::false_type {};
        template <typename T> struct HasStrMethod<T,
            std::void_t<decltype(std::declval<std::string&>() = std::declval<const T&>().str())>>: std::true_type {};
        template <typename T> constexpr bool has_str_method = HasStrMethod<T>::value;

        template <typename T, typename = void> struct HasAdlToStringFunction: std::false_type {};
        template <typename T> struct HasAdlToStringFunction<T,
            std::void_t<decltype(std::declval<std::string&>() = to_string(std::declval<const T&>()))>>: std::true_type {};
        template <typename T> constexpr bool has_adl_to_string_function = HasAdlToStringFunction<T>::value;

        template <typename T, typename = void> struct HasStdToStringFunction: std::false_type {};
        template <typename T> struct HasStdToStringFunction<T,
            std::void_t<decltype(std::declval<std::string&>() = std::to_string(std::declval<const T&>()))>>: std::true_type {};
        template <typename T> constexpr bool has_std_to_string_function = HasStdToStringFunction<T>::value;

        template <typename T, typename = void> struct HasAdlBeginFunction: std::false_type {};
        template <typename T> struct HasAdlBeginFunction<T, std::void_t<decltype(begin(std::declval<const T&>()))>>: std::true_type {};
        template <typename T, typename = void> struct HasAdlEndFunction: std::false_type {};
        template <typename T> struct HasAdlEndFunction<T, std::void_t<decltype(end(std::declval<const T&>()))>>: std::true_type {};
        template <typename T, typename = void> struct HasStdBeginFunction: std::false_type {};
        template <typename T> struct HasStdBeginFunction<T, std::void_t<decltype(std::begin(std::declval<const T&>()))>>: std::true_type {};
        template <typename T, typename = void> struct HasStdEndFunction: std::false_type {};
        template <typename T> struct HasStdEndFunction<T, std::void_t<decltype(std::end(std::declval<const T&>()))>>: std::true_type {};

        template <typename T> constexpr bool is_range = (HasAdlBeginFunction<T>::value && HasAdlEndFunction<T>::value)
            || (HasStdBeginFunction<T>::value && HasStdEndFunction<T>::value);

        template <typename T, bool UseAdl = HasAdlBeginFunction<T>::value, bool UseStd = HasStdBeginFunction<T>::value> struct RangeValueType
            { using type = void; };
        template <typename T, bool UseStd> struct RangeValueType<T, true, UseStd>
            { using type = std::decay_t<decltype(*begin(std::declval<T>()))>; };
        template <typename T> struct RangeValueType<T, false, true>
            { using type = std::decay_t<decltype(*std::begin(std::declval<T>()))>; };

        template <typename T, typename = void> struct HasFirstMember: std::false_type {};
        template <typename T> struct HasFirstMember<T,
            std::void_t<decltype(std::declval<T>().first)>>: std::true_type {};
        template <typename T, typename = void> struct HasSecondMember: std::false_type {};
        template <typename T> struct HasSecondMember<T,
            std::void_t<decltype(std::declval<T>().second)>>: std::true_type {};
        template <typename T> constexpr bool is_pairlike = HasFirstMember<T>::value && HasSecondMember<T>::value;

        template <typename T> constexpr bool is_maplike = is_range<T> && is_pairlike<typename RangeValueType<T>::type>;

        inline int str_to_int(const std::string& str) {
            return int(std::strtol(str.data(), nullptr, 10));
        }

        inline std::string native_format(const char* pattern, double x, int prec) {
            std::string result(15, '\0');
            int rc = 0;
            for (;;) {
                rc = std::snprintf(result.data(), result.size(), pattern, prec, x);
                if (rc < int(result.size()))
                    break;
                result.resize(rc + 1);
            }
            result.resize(rc);
            return result;
        }

        inline std::string trim_exponent(const std::string& str, bool xsign) {
            size_t begin_cut = str.find_first_of("Ee");
            if (begin_cut == std::string::npos)
                return str;
            ++begin_cut;
            bool neg = str[begin_cut] == '-';
            if (neg)
                ++begin_cut;
            size_t end_cut = begin_cut;
            if (str[end_cut] == '+')
                ++end_cut;
            while (end_cut < str.size() && str[end_cut] == '0')
                ++end_cut;
            if (end_cut == str.size())
                --end_cut;
            auto result = str.substr(0, begin_cut);
            if (xsign && ! neg)
                result += '+';
            result += str.substr(end_cut);
            return result;
        }

        inline std::string trim_zeros(const std::string& str) {
            size_t dec_point = str.find('.');
            if (dec_point == std::string::npos)
                return str;
            size_t end_sig = str.find_first_not_of("0123456789", dec_point + 1);
            if (end_sig == std::string::npos)
                end_sig = str.size();
            size_t last_digit = end_sig - 1;
            while (str[last_digit] == '0')
                --last_digit;
            if (str[last_digit] != '.')
                ++last_digit;
            if (last_digit < end_sig)
                return str.substr(0, last_digit) + str.substr(end_sig);
            else
                return str;
        }

        template <typename T>
        std::string format_float_d(T x, int prec) {
            static constexpr const char* pattern = sizeof(T) > sizeof(double) ? "%.*Le" : "%.*e";
            prec = std::max(prec, 1);
            std::string full_str = native_format(pattern, x, prec - 1);
            size_t sig_pos = size_t(full_str[0] == '-');
            size_t exp_pos = full_str.find_first_of("Ee", sig_pos);
            auto result = full_str.substr(sig_pos, exp_pos - sig_pos);
            if (result[1] == '.')
                result.erase(1, 1);
            int exp = str_to_int(full_str.substr(exp_pos + 1));
            if (exp < 0) {
                result.insert(0, "0.");
                result.insert(2, - exp - 1, '0');
            } else if (exp < prec - 1) {
                result.insert(exp + 1, 1, '.');
            } else if (exp > prec - 1) {
                result.append(exp + 1 - prec, '0');
            }
            if (sig_pos > 0)
                result.insert(0, 1, '-');
            return result;
        }

        template <typename T>
        std::string format_float_e(T x, int prec, bool cap, bool xsign) {
            static constexpr const char* lcase = sizeof(T) > sizeof(double) ? "%.*Le" : "%.*e";
            static constexpr const char* ucase = sizeof(T) > sizeof(double) ? "%.*LE" : "%.*E";
            prec = std::max(prec, 1);
            return trim_exponent(native_format(cap ? ucase : lcase, x, prec - 1), xsign);
        }

        template <typename T>
        std::string format_float_f(T x, int prec) {
            static constexpr const char* pattern = sizeof(T) > sizeof(double) ? "%.*Lf" : "%.*f";
            prec = std::max(prec, 0);
            return native_format(pattern, x, prec);
        }

        template <typename T>
        std::string format_float_g(T x, int prec, bool cap, bool xsign) {
            auto y = std::abs(x);
            if (y == 0 || (y >= 1e-3 && y < 1e6))
                return format_float_d(x, prec);
            else
                return format_float_e(x, prec, cap, xsign);
        }

    }

    template <typename T>
    std::string format_float(T x, const std::string& spec = {}) {

        if (spec.empty())
            return Detail::trim_zeros(Detail::format_float_g(x, 6, false, false));

        char mode = spec[0];
        bool opt_ucase = mode <= 'Z';
        bool opt_sign = spec.find('s') != std::string::npos;
        bool opt_xsign = spec.find('S') != std::string::npos;
        bool opt_ztrim = spec.find('z') != std::string::npos;
        size_t pos = spec.find_first_of("0123456789");
        int prec = pos == std::string::npos ? 6 : Detail::str_to_int(spec.substr(pos));
        std::string result;

        switch (mode) {
            case 'D': case 'd':  result = Detail::format_float_d(x, prec); break;
            case 'E': case 'e':  result = Detail::format_float_e(x, prec, opt_ucase, opt_xsign); break;
            case 'F': case 'f':  result = Detail::format_float_f(x, prec); break;
            default:             result = Detail::format_float_g(x, prec, opt_ucase, opt_xsign); break;
        }

        if (opt_sign && result[0] != '-')
            result.insert(0, 1, '+');
        if (opt_ztrim)
            result = Detail::trim_zeros(result);

        return result;

    }

    template <typename T>
    std::string format_integer(T x, const std::string& spec = {}) {

        static_assert(std::numeric_limits<T>::is_integer);

        using floating_type = std::conditional_t<(sizeof(T) >= sizeof(double)), long double, double>;

        static constexpr bool is_signed = std::numeric_limits<T>::is_signed;

        char mode = spec[0];

        if (mode >= 'd' && mode <= 'f')
            return format_float(floating_type(x), spec);

        bool opt_sign = spec.find('s') != std::string::npos;
        T base = mode == 'X' || mode == 'x' ? 16 : 10;
        size_t pos = spec.find_first_of("0123456789");
        int prec = pos == std::string::npos ? 1 : Detail::str_to_int(spec.substr(pos));
        std::string result;
        bool neg = false;
        if constexpr (is_signed) {
            neg = x < 0;
            if (neg)
                x = - x;
        }

        while (x != 0 || result.size() < size_t(prec)) {
            int rem = int(x % base);
            if (rem < 10)
                result += char('0' + rem);
            else if (mode == 'X')
                result += char('A' + rem - 10);
            else
                result += char('a' + rem - 10);
            x /= base;
        }

        if (neg)
            result += '-';
        else if (opt_sign)
            result += '+';

        std::reverse(result.begin(), result.end());

        return result;

    }

    template <typename Range>
    std::string format_range(const Range& r, const std::string& spec = {}) {
        std::string s = "[";
        for (auto& x: r)
            s += format_object(x, spec) + ',';
        if (s.size() > 1)
            s.pop_back();
        s += ']';
        return s;
    }

    template <typename Range>
    std::string format_map(const Range& r, const std::string& spec = {}) {
        std::string s = "{";
        for (auto& [k,v]: r) {
            s += format_object(k, spec) + ':';
            s += format_object(v, spec) + ',';
        }
        if (s.size() > 1)
            s.pop_back();
        s += '}';
        return s;
    }

    template <typename T>
    std::string format_via_stream(const T& t) {
        std::ostringstream out;
        out << t;
        return out.str();
    }

    template <typename T>
    std::string format_object(const T& t, const std::string& spec) {
        if constexpr (std::is_same_v<T, std::nullptr_t>)
            return "null";
        else if constexpr (std::is_same_v<T, bool>)
            return spec[0] == 'y' ? (t ? "yes" : "no") : (t ? "true" : "false");
        else if constexpr (std::is_same_v<T, char>)
            return std::string{t};
        else if constexpr (std::is_integral_v<T>)
            return format_integer(t, spec);
        else if constexpr (std::is_floating_point_v<T>)
            return format_float(t, spec);
        else if constexpr (Detail::has_str_method<T>)
            return t.str();
        else if constexpr (Detail::has_adl_to_string_function<T>)
            return to_string(t);
        else if constexpr (Detail::has_std_to_string_function<T>)
            return std::to_string(t);
        else if constexpr (std::is_constructible_v<std::string, T> && std::is_pointer_v<T>)
            return t ? std::string(t) : "null";
        else if constexpr (std::is_constructible_v<std::string, T>)
            return std::string(t);
        else if constexpr (Detail::is_maplike<T>)
            return format_map(t, spec);
        else if constexpr (Detail::is_range<T>)
            return format_range(t, spec);
        else
            return format_via_stream<T>(t);
    }

}
