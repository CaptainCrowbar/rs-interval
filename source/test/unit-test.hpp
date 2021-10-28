#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <new>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#ifdef _WIN32
    #include <io.h>
#else
    #include <unistd.h>
#endif

#ifdef __GNUC__
    #include <cxxabi.h>
#endif

namespace RS::UnitTest {

    // Utility functions

    constexpr int max_failures = 100;

    inline bool isatty_function(int fd) noexcept {
        #ifdef _WIN32
            return _isatty(fd) != 0;
        #else
            return isatty(fd) != 0;
        #endif
    }

    inline std::string text_colour(int r, int g, int b) {
        if (isatty_function(1))
            return "\x1b[38;5;" + std::to_string(36 * r + 6 * g + b + 16) + "m";
        else
            return {};
    }

    const std::string reset_style = isatty_function(1) ? "\x1b[0m" : "";
    const std::string info_style = text_colour(5,5,3);
    const std::string pass_style = text_colour(2,5,2);
    const std::string fail_style = text_colour(5,3,0);
    const std::string delimiter_line = info_style + std::string(30, '=') + reset_style;

    inline std::atomic<int> failure_count(0);
    inline std::chrono::system_clock::time_point start_time;
    inline std::regex test_pattern(".*");

    // Utility functions

    // Format a range as "[a,b,c,...]".

    template <typename Range>
    std::string format_range(const Range& r) {
        std::ostringstream out;
        out << '[';
        for (auto& x: r)
            out << x << ',';
        auto str = out.str();
        if (str.size() > 1)
            str.pop_back();
        str += ']';
        return str;
    }

    // Format a range of pairs as "{a:1,b:2,c:3,...}".

    template <typename Range>
    std::string format_map(const Range& r) {
        std::ostringstream out;
        out << '{';
        for (auto& p: r)
            out << p.first << ':' << p.second << ',';
        auto str = out.str();
        if (str.size() > 1)
            str.pop_back();
        str += '}';
        return str;
    }

    // Human readable type name.

    template <typename T>
    std::string type_name() {
        std::string name = typeid(T).name();
        #ifdef __GNUC__
            auto mangled = name;
            std::shared_ptr<char> demangled;
            int status = 0;
            for (;;) {
                if (mangled.empty())
                    break;
                demangled.reset(abi::__cxa_demangle(mangled.data(), nullptr, nullptr, &status), std::free);
                if (status == -1)
                    throw std::bad_alloc();
                if (status == 0 && demangled)
                    return demangled.get();
                if (mangled[0] != '_')
                    break;
                mangled.erase(0, 1);
            }
        #endif
        return name;
    }

    // Strip leading whitespace from a multiline string, and remove
    // empty leading and trailing lines. Leading whitespace common to
    // all lines is removed. Indentation must be all spaces or all
    // tabs; behaviour is undefined if these are mixed.

    inline std::string operator""_doc(const char* ptr, std::size_t len) {
        static constexpr std::size_t npos = std::string::npos;
        static constexpr const char* ws = "\t ";
        std::string text(ptr, len);
        // Normalize line endings
        text.erase(std::remove(text.begin(), text.end(), '\r'), text.end());
        if (text.empty())
            return {};
        // Check for leading and trailing blank lines
        std::size_t margin = npos;
        std::size_t not_space = text.find_first_not_of(ws);
        if (text[not_space] == '\n')
            text.erase(0, not_space + 1);
        not_space = text.find_last_not_of(ws);
        if (not_space != npos && text[not_space] == '\n') {
            margin = text.size() - not_space - 1;
            text.erase(text.size() - margin, npos);
        }
        if (text.empty())
            return {};
        if (text.back() != '\n')
            text += '\n';
        // Find the amount of leading space common to all lines
        std::size_t line_begin = 0;
        do {
            not_space = text.find_first_not_of(ws, line_begin);
            margin = std::min(margin, not_space);
            line_begin = text.find('\n', line_begin) + 1;
        } while (line_begin != 0);
        // Erase the leading space from each line
        if (margin != npos) {
            line_begin = 0;
            do {
                text.erase(line_begin, margin);
                line_begin = text.find('\n', line_begin) + 1;
            } while (line_begin != 0);
        }
        return text;
    }

    // Type wrapper with population tracking.

    template <typename T>
    class Counted {
    public:
        Counted(): value_() { ++number(); }
        Counted(const T& t): value_(t) { ++number(); }
        Counted(const Counted& c): value_(c.value_) { ++number(); }
        Counted(Counted&& c) noexcept: value_(std::exchange(c.value_, T())) { ++number(); }
        ~Counted() noexcept { --number(); }
        Counted& operator=(const Counted& c) = default;
        Counted& operator=(Counted&& c) noexcept { if (&c != this) value_ = std::exchange(c.value_, T()); return *this; }
        Counted& operator=(const T& t) { value_ = t; return *this; }
        T& operator*() noexcept { return value_; }
        const T& operator*() const noexcept { return value_; }
        T* operator->() noexcept { return &value_; }
        const T* operator->() const noexcept { return &value_; }
        const T& get() const noexcept { return value_; }
        void set(const T& t) { value_ = t; }
        static int count() noexcept { return number(); }
        static void reset() noexcept { number() = 0; }
        friend bool operator==(const Counted& a, const Counted& b) { return a.value_ == b.value_; }
        friend bool operator!=(const Counted& a, const Counted& b) { return ! (a == b); }
        friend bool operator<(const Counted& a, const Counted& b) { return a.value_ < b.value_; }
        friend bool operator>(const Counted& a, const Counted& b) { return b < a; }
        friend bool operator<=(const Counted& a, const Counted& b) { return ! (b < a); }
        friend bool operator>=(const Counted& a, const Counted& b) { return ! (a < b); }
        friend std::ostream& operator<<(std::ostream& out, const Counted& c) { return out << c.value_; }
    private:
        T value_;
        static std::atomic<int>& number() noexcept { static std::atomic<int> n(0); return n; }
    };

    // Call these at the beginning and end of main()

    inline void begin_tests(int argc, char** argv) {
        using namespace std::chrono;
        if (argc > 1) {
            std::string pattern_str;
            for (int i = 1; i < argc; ++i)
                pattern_str += "(" + std::string(argv[i]) + ")|";
            pattern_str.pop_back();
            test_pattern = std::regex(pattern_str, std::regex::icase | std::regex::nosubs);
        }
        std::cout << "\n"
            << info_style << "Running unit tests" << reset_style << "\n"
            << delimiter_line << std::endl;
        start_time = system_clock::now();
    }

    inline int end_tests() {
        using namespace std::chrono;
        auto time = system_clock::now() - start_time;
        std::cout << delimiter_line << "\n";
        double seconds = 1e-9 * duration_cast<nanoseconds>(time).count();
        if (failure_count == 0)
            std::cout << pass_style << "OK: all tests passed" << reset_style << "\n";
        else
            std::cout << fail_style << "*** Tests failed: " << failure_count << reset_style << "\n";
        std::cout << info_style << "Time: " << seconds << " s" << reset_style << "\n\n";
        return int(failure_count > 0);
    }

}

namespace std {

    template <typename T>
    struct hash<::RS::UnitTest::Counted<T>> {
        std::size_t operator()(const ::RS::UnitTest::Counted<T>& ct) const {
            return ::std::hash<T>()(*ct);
        }
    };

}

// Call this from main() for each test

#define UNIT_TEST(name) \
    if (std::regex_search(# name, RS::UnitTest::test_pattern)) { \
        extern void test_##name(); \
        try { \
            std::string title(#name); \
            std::replace(title.begin(), title.end(), '_', ' '); \
            std::cout << RS::UnitTest::info_style << "Test " << title << RS::UnitTest::reset_style << std::endl; \
            test_##name(); \
        } \
        catch (const std::exception& ex) { \
            std::cout << RS::UnitTest::fail_style << "*** " << ex.what() << std::endl; \
        } \
    }

// Fail a test (normally only used internally)

#define FAIL_TEST(message) { \
    std::cout << RS::UnitTest::fail_style \
        << "*** [" << __FILE__ << ":" << __LINE__ << "] " \
        << message << RS::UnitTest::reset_style << std::endl; \
    if (++RS::UnitTest::failure_count >= RS::UnitTest::max_failures) \
        return; \
}

// Fail if the expression throws an exception

#define TRY(expr) \
    try { \
        expr; \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception from " << # expr << ": " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception from " << # expr); \
    }

// Fail if the expression is false or throws an exception

#define TEST(expr) \
    try { \
        if (! (expr)) \
            FAIL_TEST("Expression is false: " << # expr); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception from " << # expr << ": " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception from " << # expr); \
    }

// Fail and cut testing short if the expression is false or throws an exception

#define REQUIRE(expr) \
    try { \
        if (! (expr)) { \
            FAIL_TEST("Expression is false: " << # expr); \
            return; \
        } \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception from " << # expr << ": " << ex.what()); \
        return; \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception from " << # expr); \
        return; \
    }

// Fail if the two expressions are not equal, or either of them throws an exception

#define TEST_EQUAL(lhs, rhs) \
    try { \
        auto rs_unit_test_lhs = (lhs); \
        auto rs_unit_test_rhs = (rhs); \
        if (rs_unit_test_lhs != rs_unit_test_rhs) \
            FAIL_TEST("Expressions are not equal: " \
                << # lhs << " = " << rs_unit_test_lhs << ", " \
                << # rhs << " = " << rs_unit_test_rhs); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception: " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception"); \
    }

#define TEST_EQUAL_RANGES(lhs, rhs) \
    try { \
        using std::begin; \
        using std::end; \
        auto rs_unit_test_lhs_it = begin(lhs); \
        auto rs_unit_test_lhs_end = end(lhs); \
        auto rs_unit_test_rhs_it = begin(rhs); \
        auto rs_unit_test_rhs_end = end(rhs); \
        auto rs_unit_test_lhs_size = std::distance(rs_unit_test_lhs_it, rs_unit_test_lhs_end); \
        auto rs_unit_test_rhs_size = std::distance(rs_unit_test_rhs_it, rs_unit_test_rhs_end); \
        if (rs_unit_test_lhs_size != rs_unit_test_rhs_size) \
            FAIL_TEST("Ranges have different lengths: " \
                << # lhs << " [" << rs_unit_test_lhs_size << "], " \
                << # rhs << " [" << rs_unit_test_rhs_size << "]"); \
        int rs_unit_test_index = 0; \
        for (; rs_unit_test_lhs_it != rs_unit_test_lhs_end; ++rs_unit_test_lhs_it, ++rs_unit_test_rhs_it, ++rs_unit_test_index) \
            if (*rs_unit_test_lhs_it != *rs_unit_test_rhs_it) \
                FAIL_TEST("Ranges are not equal: " \
                    << # lhs << " [" << rs_unit_test_index << "] = " << *rs_unit_test_lhs_it << ", " \
                    << # rhs << " [" << rs_unit_test_index << "] = " << *rs_unit_test_rhs_it); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception: " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception"); \
    }

// Fail if two floating point values are not within epsilon of each other

#define TEST_NEAR(lhs, rhs, epsilon) \
    try { \
        auto rs_unit_test_lhs = static_cast<long double>(lhs); \
        auto rs_unit_test_rhs = static_cast<long double>(rhs); \
        auto rs_unit_test_epsilon = static_cast<long double>(epsilon); \
        if (std::abs(rs_unit_test_rhs - rs_unit_test_lhs) > rs_unit_test_epsilon) \
            FAIL_TEST("Expressions are not close enough: " \
                << # lhs << " = " << rs_unit_test_lhs << ", " \
                << # rhs << " = " << rs_unit_test_rhs << ", " \
                << "epsilon = " << rs_unit_test_epsilon); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception: " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception"); \
    }

// Fail if the first expression does not match the regex pattern, or throws an exception

#define TEST_MATCH(expr, pattern) \
    try { \
        auto rs_unit_test_expr = (expr); \
        std::regex rs_unit_test_pattern(pattern); \
        if (! std::regex_search(rs_unit_test_expr, rs_unit_test_pattern)) \
            FAIL_TEST("Expression does not match regex: " \
                << # expr << " = " << rs_unit_test_expr \
                << ", pattern = " << # pattern); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception from " << # expr << ": " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception from " << # expr); \
    }

#define TEST_MATCH_ICASE(expr, pattern) \
    try { \
        auto rs_unit_test_expr = (expr); \
        std::regex rs_unit_test_pattern(pattern, std::regex::icase); \
        if (! std::regex_search(rs_unit_test_expr, rs_unit_test_pattern)) \
            FAIL_TEST("Expression does not match regex: " \
                << # expr << " = " << rs_unit_test_expr \
                << ", pattern = " << # pattern); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception from " << # expr << ": " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception from " << # expr); \
    }

// Fail if the expression does not throw an exception, or throws one of the wrong type

#define TEST_THROW(expr, extype) \
    try { \
        expr; \
        FAIL_TEST("Expression failed to throw: " << # expr); \
    } \
    catch (const extype&) {} \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception from " << # expr << ": " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception from " << # expr); \
    }

// Also check that the error message matches

#define TEST_THROW_MESSAGE(expr, extype, message) \
    try { \
        expr; \
        FAIL_TEST("Expression failed to throw: " << # expr); \
    } \
    catch (const extype& ex) { \
        std::string rs_unit_test_what(ex.what()); \
        std::string rs_unit_test_message(message); \
        if (rs_unit_test_what != rs_unit_test_message) \
            FAIL_TEST("Exception message does not match: " \
                << "what = " << rs_unit_test_what \
                << ", expected = " << rs_unit_test_message); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception from " << # expr << ": " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception from " << # expr); \
    }

#define TEST_THROW_MATCH(expr, extype, pattern) \
    try { \
        expr; \
        FAIL_TEST("Expression failed to throw: " << # expr); \
    } \
    catch (const extype& ex) { \
        std::string rs_unit_test_what(ex.what()); \
        std::regex rs_unit_test_pattern(pattern, std::regex::icase); \
        if (! std::regex_search(rs_unit_test_what, rs_unit_test_pattern)) \
            FAIL_TEST("Exception message does not match regex: " \
                << "what = " << rs_unit_test_what \
                << ", pattern = " << # pattern); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception from " << # expr << ": " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception from " << # expr); \
    }

// Fail if the two types are not the same

#define TEST_TYPE(lhs, rhs) { \
    if (! std::is_same_v<lhs, rhs>) \
        FAIL_TEST("Types are not the same: " \
            << # lhs << " => " << RS::UnitTest::type_name<lhs>() << ", " \
            << # rhs << " => " << RS::UnitTest::type_name<rhs>()); \
}
