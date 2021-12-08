#pragma once

#include "rs-interval/type-traits.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <type_traits>

namespace RS::Intervals {

    // Format spec is "<mode>[<options>][<prec>]"
    // Integer
    //      Default is "n1"
    //      Floating point specs can also be used
    //      Mode:
    //          n = Decimal
    //          x = Hexadecimal
    //      Options:
    //          s = Always show a sign
    // Floating point
    //      Default is "gz6"
    //      Mode:
    //          d = Decimal format (prec = significant digits)
    //          e = Exponential (scientific) format (prec = significant digits)
    //          f = Fixed point format (prec = decimal places)
    //          g = Use 'd' if 1e-3<=|value|<1e6 or value=0, otherwise 'e'
    //      Options:
    //          s = Always show a sign
    //          z = Strip trailing zeroes

    template <typename T> std::string format_object(const T& t, const std::string& spec = {});

    namespace Detail {

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

        inline std::string trim_exponent(const std::string& str) {
            size_t begin_cut = str.find('e');
            if (begin_cut == npos)
                return str;
            ++begin_cut;
            if (str[begin_cut] == '-')
                ++begin_cut;
            size_t end_cut = begin_cut;
            if (str[end_cut] == '+')
                ++end_cut;
            while (end_cut < str.size() && str[end_cut] == '0')
                ++end_cut;
            if (end_cut == str.size())
                --end_cut;
            return str.substr(0, begin_cut) + str.substr(end_cut);
        }

        inline std::string trim_zeros(const std::string& str) {
            size_t dec_point = str.find('.');
            if (dec_point == npos)
                return str;
            size_t end_sig = str.find_first_not_of("0123456789", dec_point + 1);
            if (end_sig == npos)
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

        inline std::string format_float_d(double x, int prec) {
            prec = std::max(prec, 1);
            auto full_str = native_format("%.*e", x, prec - 1);
            size_t sig_pos = size_t(full_str[0] == '-');
            size_t exp_pos = full_str.find('e', sig_pos);
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

        inline std::string format_float_e(double x, int prec) {
            prec = std::max(prec, 1);
            return trim_exponent(native_format("%.*e", x, prec - 1));
        }

        inline std::string format_float_f(double x, int prec) {
            prec = std::max(prec, 0);
            return native_format("%.*f", x, prec);
        }

        inline std::string format_float_g(double x, int prec) {
            auto y = std::abs(x);
            if (y == 0 || (y >= 1e-3 && y < 1e6))
                return format_float_d(x, prec);
            else
                return format_float_e(x, prec);
        }

    }

    inline std::string format_float(double x, const std::string& spec = {}) {

        if (spec.empty())
            return Detail::trim_zeros(Detail::format_float_g(x, 6));

        char mode = spec[0];
        bool opt_sign = spec.find('s') != npos;
        bool opt_ztrim = spec.find('z') != npos;
        size_t pos = spec.find_first_of("0123456789");
        int prec = pos == npos ? 6 : Detail::str_to_int(spec.substr(pos));
        std::string result;

        switch (mode) {
            case 'd':  result = Detail::format_float_d(x, prec); break;
            case 'e':  result = Detail::format_float_e(x, prec); break;
            case 'f':  result = Detail::format_float_f(x, prec); break;
            default:   result = Detail::format_float_g(x, prec); break;
        }

        if (opt_sign && result[0] != '-')
            result.insert(0, 1, '+');
        if (opt_ztrim)
            result = Detail::trim_zeros(result);

        return result;

    }

    inline std::string format_integer(int64_t x, const std::string& spec = {}) {

        char mode = spec[0];

        if (mode >= 'd' && mode <= 'f')
            return format_float(double(x), spec);

        int64_t base = mode == 'x' ? 16 : 10;
        bool opt_sign = spec.find('s') != npos;
        size_t pos = spec.find_first_of("0123456789");
        int prec = pos == npos ? 1 : Detail::str_to_int(spec.substr(pos));
        std::string result;
        int64_t quo = std::abs(x);

        while (quo != 0 || result.size() < size_t(prec)) {
            result += "0123456789abcdef"[quo % base];
            quo /= base;
        }

        if (x < 0)
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
