#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <format>
#include <print>
#include <ranges>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

namespace RS::UnitTest {

    const inline auto tty =
        #ifdef _WIN32
            [] {
                DWORD mode = 0;
                return GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &mode) != 0;
            }();
        #else
            isatty(1) != 0;
        #endif

    // black    = esc [30m
    // red      = esc [31m
    // green    = esc [32m
    // yellow   = esc [33m
    // blue     = esc [34m
    // magenta  = esc [35m
    // cyan     = esc [36m
    // white    = esc [37m

    const inline std::string_view xreset  = tty ? "\x1b[0m" : "";   // reset colours
    const inline std::string_view xhead   = tty ? "\x1b[33m" : "";  // yellow
    const inline std::string_view xrule   = tty ? "\x1b[36m" : "";  // cyan
    const inline std::string_view xtest   = tty ? "\x1b[33m" : "";  // yellow
    const inline std::string_view xpass   = tty ? "\x1b[32m" : "";  // green
    const inline std::string_view xfail   = tty ? "\x1b[31m" : "";  // red

    const inline std::string rule(30, '=');

    inline auto failures = 0;
    inline std::vector<std::string> main_args;

    struct TextMatch {

        std::string text;

        bool operator()(std::string pattern) const {
            std::regex regex(pattern);
            return std::regex_search(text, regex);
        }

    };

    inline void call_me_maybe(void (*test)(), std::string name) {
        if (main_args.empty() || std::ranges::any_of(main_args, TextMatch{name})) {
            std::println("{}{}{}", xtest, name, xreset);
            test();
        }
    }

    inline bool read_file_contents(const std::filesystem::path& path, std::string& out) {

        auto file_ptr = std::fopen(path.c_str(), "rb");

        if (file_ptr == nullptr) {
            return false;
        }

        std::fseek(file_ptr, 0, SEEK_END);
        auto size = static_cast<std::size_t>(std::ftell(file_ptr));
        std::fseek(file_ptr, 0, SEEK_SET);
        out.resize(size);
        std::fread(out.data(), 1, size, file_ptr);

        return true;

    }

}

// Explicitly trigger a failure.

#define FAIL(...) do { \
    std::println("{}... Test failed [{}:{}]{}\n\t{}", \
        ::RS::UnitTest::xfail, __FILE__, __LINE__, ::RS::UnitTest::xreset, std::format(__VA_ARGS__)); \
    ++::RS::UnitTest::failures; \
} while (false)

// Evaluate an expression. Fails if any exception is thrown.

#define TRY(expr) do { \
    try { \
        (expr); \
    } \
    catch (const std::exception& _test_except) { \
        FAIL("Unexpected exception: {}", _test_except.what()); \
    } \
    catch (...) { \
        FAIL("Unexpected exception"); \
    } \
} while (false)

// Evaluate a boolean expression. Fails if the expression is false, or if any
// exception is thrown.

#define TEST(expr) do { \
    try { \
        auto _test_flag = static_cast<bool>(expr); \
        if (! _test_flag) { \
            FAIL("Expression is false: {}", # expr); \
        } \
    } \
    catch (const std::exception& _test_except) { \
        FAIL("Unexpected exception: {}", _test_except.what()); \
    } \
    catch (...) { \
        FAIL("Unexpected exception"); \
    } \
} while (false)

// Same as TEST(), but quit the current test function on failure. This is
// intended for situations where a failure at this point would render the
// rest of the tests meaningless or impossible.

#define REQUIRE(expr) do { \
    try { \
        auto _test_flag = static_cast<bool>(expr); \
        if (! _test_flag) { \
            FAIL("Expression is false: {}", # expr); \
            return; \
        } \
    } \
    catch (const std::exception& _test_except) { \
        FAIL("Unexpected exception: {}", _test_except.what()); \
        return; \
    } \
    catch (...) { \
        FAIL("Unexpected exception"); \
        return; \
    } \
} while (false)

// Compare two expressions for equality. Fails if the expressions are not
// equal, or if any exception is thrown.

#define TEST_EQUAL(lhs, rhs) do { \
    try { \
        auto _test_lhs {lhs}; \
        auto _test_rhs {rhs}; \
        if (_test_lhs != _test_rhs) { \
            FAIL("Expressions are not equal\n\t{} = {}\n\t{} = {}", \
                # lhs, _test_lhs, # rhs, _test_rhs); \
        } \
    } \
    catch (const std::exception& _test_except) { \
        FAIL("Unexpected exception: {}", _test_except.what()); \
    } \
    catch (...) { \
        FAIL("Unexpected exception"); \
    } \
} while (false)

// Compare two floating point expressions. Fails if the expressions differ by
// more than the tolerance in either direction, or if any exception is
// thrown.

#define TEST_NEAR(lhs, rhs, tolerance) do { \
    try { \
        auto _test_lhs = static_cast<double>(lhs); \
        auto _test_rhs = static_cast<double>(rhs); \
        auto _test_tolerance = static_cast<double>(tolerance); \
        auto _test_delta = std::abs(_test_lhs - _test_rhs); \
        if (_test_delta > _test_tolerance) { \
            FAIL("Difference between expressions is too great\n" \
                "\t{} = {}\n\t{} = {}\n\ttolerance = {}", \
                # lhs, _test_lhs, # rhs, _test_rhs, _test_tolerance); \
        } \
    } \
    catch (const std::exception& ex) { \
        FAIL("Unexpected exception: {}", ex.what()); \
    } \
    catch (...) { \
        FAIL("Unexpected exception"); \
    } \
} while (false)

// Check an expression against a range. Fails if the expression is outside the
// range, or if any exception is thrown.

#define TEST_IN_RANGE(expr, min, max) do { \
    try { \
        auto _test_expr {expr}; \
        auto _test_min {min}; \
        auto _test_max {max}; \
        if (_test_expr < _test_min || _test_expr > _test_max) \
            FAIL("Expression is out of range\n" \
                "\t{} = {}\n\tmin = {}, max = {}", \
                # expr, _test_expr, _test_min, _test_max); \
    } \
    catch (const std::exception& _test_except) { \
        FAIL("Unexpected exception: {}", _test_except.what()); \
    } \
    catch (...) { \
        FAIL("Unexpected exception"); \
    } \
} while (false)

// Compare an expression (expected to return a string) for a substring match
// against a regular expression. Fails if the result does not match or an
// exception is thrown. The regex pattern is passed as a string literal, and
// can be made case insensitive by appending "/i".

#define TEST_MATCH(expr, pattern) do { \
    try { \
        auto _test_string = static_cast<std::string>(expr); \
        if (! ::RS::UnitTest::TextMatch{_test_string}(pattern)) { \
            FAIL("Expression does not match pattern: {:?}", _test_string); \
        } \
    } \
    catch (const std::exception& _test_except) { \
        FAIL("Unexpected exception: {}", _test_except.what()); \
    } \
    catch (...) { \
        FAIL("Unexpected exception"); \
    } \
} while (false)

// Evaluate an expression that is expected to throw an exception of the given
// type, or a type derived from it. Fails if no exception is thrown, if an
// exception of the wrong type is thrown, or if the exception's type is
// correct but its what() message does not match the regular expression. The
// regex pattern is passed as a string literal, and can be made case
// insensitive by appending "/i".

#define TEST_THROW(expr, exception_type, pattern) do { \
    try { \
        (expr); \
        FAIL("No exception thrown: {}", # expr); \
    } \
    catch (const exception_type& _test_except) { \
        if (! ::RS::UnitTest::TextMatch{_test_except.what()}(pattern)) { \
            FAIL("Unexpected error message: {}", _test_except.what()); \
        } \
    } \
    catch (const std::exception& _test_except) { \
        FAIL("Unexpected exception: {}", _test_except.what()); \
    } \
    catch (...) { \
        FAIL("Unexpected exception"); \
    } \
} while (false)
