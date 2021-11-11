#include "rs-interval/interval.hpp"
#include "test/unit-test.hpp"
#include <chrono>
#include <complex>
#include <string>
#include <vector>

using namespace RS::Intervals;

void test_rs_interval_basic_type_traits() {

    TEST_EQUAL(interval_category<bool>, IntervalCategory::none);
    TEST_EQUAL(interval_category<const bool>, IntervalCategory::none);
    TEST_EQUAL(interval_category<volatile bool>, IntervalCategory::none);
    TEST_EQUAL(interval_category<const volatile bool>, IntervalCategory::none);
    TEST_EQUAL(interval_category<void>, IntervalCategory::none);
    TEST_EQUAL(interval_category<std::complex<float>>, IntervalCategory::none);

    TEST_EQUAL(interval_category<std::string>, IntervalCategory::ordered);
    TEST_EQUAL(interval_category<const std::string>, IntervalCategory::ordered);
    TEST_EQUAL(interval_category<volatile std::string>, IntervalCategory::ordered);
    TEST_EQUAL(interval_category<const volatile std::string>, IntervalCategory::ordered);
    TEST_EQUAL(interval_category<std::vector<int>>, IntervalCategory::ordered);
    TEST_EQUAL(interval_category<void*>, IntervalCategory::ordered);
    TEST_EQUAL(interval_category<const void*>, IntervalCategory::ordered);

    TEST_EQUAL(interval_category<char*>, IntervalCategory::stepwise);
    TEST_EQUAL(interval_category<const char*>, IntervalCategory::stepwise);
    TEST_EQUAL(interval_category<std::string::const_iterator>, IntervalCategory::stepwise);
    TEST_EQUAL(interval_category<std::string::iterator>, IntervalCategory::stepwise);
    TEST_EQUAL(interval_category<std::vector<int>::const_iterator>, IntervalCategory::stepwise);
    TEST_EQUAL(interval_category<std::vector<int>::iterator>, IntervalCategory::stepwise);
    TEST_EQUAL(interval_category<std::chrono::microseconds>, IntervalCategory::stepwise);

    TEST_EQUAL(interval_category<int>, IntervalCategory::integral);
    TEST_EQUAL(interval_category<const int>, IntervalCategory::integral);
    TEST_EQUAL(interval_category<volatile int>, IntervalCategory::integral);
    TEST_EQUAL(interval_category<const volatile int>, IntervalCategory::integral);
    TEST_EQUAL(interval_category<uint8_t>, IntervalCategory::integral);
    TEST_EQUAL(interval_category<int64_t>, IntervalCategory::integral);

    TEST_EQUAL(interval_category<float>, IntervalCategory::continuous);
    TEST_EQUAL(interval_category<const float>, IntervalCategory::continuous);
    TEST_EQUAL(interval_category<volatile float>, IntervalCategory::continuous);
    TEST_EQUAL(interval_category<const volatile float>, IntervalCategory::continuous);
    TEST_EQUAL(interval_category<double>, IntervalCategory::continuous);

}

void test_rs_interval_format_floating_point() {

    using namespace RS::Intervals::Detail;

    std::string s;
    double x = 0;

    x = 0;               TRY(s = format_value(x, 'e'));     TEST_EQUAL(s, "0.000000e0");
    x = 0;               TRY(s = format_value(x, 'e', 0));  TEST_EQUAL(s, "0e0");
    x = 0;               TRY(s = format_value(x, 'e', 2));  TEST_EQUAL(s, "0.00e0");
    x = 123.456;         TRY(s = format_value(x, 'e'));     TEST_EQUAL(s, "1.234560e2");
    x = 123.456;         TRY(s = format_value(x, 'e', 0));  TEST_EQUAL(s, "1e2");
    x = 123.456;         TRY(s = format_value(x, 'e', 2));  TEST_EQUAL(s, "1.23e2");
    x = 123.456;         TRY(s = format_value(x, 'e', 8));  TEST_EQUAL(s, "1.23456000e2");
    x = -123.456;        TRY(s = format_value(x, 'e'));     TEST_EQUAL(s, "-1.234560e2");
    x = -123.456;        TRY(s = format_value(x, 'e', 0));  TEST_EQUAL(s, "-1e2");
    x = -123.456;        TRY(s = format_value(x, 'e', 2));  TEST_EQUAL(s, "-1.23e2");
    x = -123.456;        TRY(s = format_value(x, 'e', 8));  TEST_EQUAL(s, "-1.23456000e2");
    x = 123456000.0;     TRY(s = format_value(x, 'e'));     TEST_EQUAL(s, "1.234560e8");
    x = 123456000.0;     TRY(s = format_value(x, 'e', 0));  TEST_EQUAL(s, "1e8");
    x = 123456000.0;     TRY(s = format_value(x, 'e', 2));  TEST_EQUAL(s, "1.23e8");
    x = 123456000.0;     TRY(s = format_value(x, 'e', 8));  TEST_EQUAL(s, "1.23456000e8");
    x = 0.000000123456;  TRY(s = format_value(x, 'e'));     TEST_EQUAL(s, "1.234560e-7");
    x = 0.000000123456;  TRY(s = format_value(x, 'e', 0));  TEST_EQUAL(s, "1e-7");
    x = 0.000000123456;  TRY(s = format_value(x, 'e', 2));  TEST_EQUAL(s, "1.23e-7");
    x = 0.000000123456;  TRY(s = format_value(x, 'e', 8));  TEST_EQUAL(s, "1.23456000e-7");

    x = 0;         TRY(s = format_value(x, 'f'));     TEST_EQUAL(s, "0.000000");
    x = 0;         TRY(s = format_value(x, 'f', 0));  TEST_EQUAL(s, "0");
    x = 0;         TRY(s = format_value(x, 'f', 2));  TEST_EQUAL(s, "0.00");
    x = 123.456;   TRY(s = format_value(x, 'f'));     TEST_EQUAL(s, "123.456000");
    x = 123.456;   TRY(s = format_value(x, 'f', 0));  TEST_EQUAL(s, "123");
    x = 123.456;   TRY(s = format_value(x, 'f', 2));  TEST_EQUAL(s, "123.46");
    x = 123.456;   TRY(s = format_value(x, 'f', 4));  TEST_EQUAL(s, "123.4560");
    x = -123.456;  TRY(s = format_value(x, 'f'));     TEST_EQUAL(s, "-123.456000");
    x = -123.456;  TRY(s = format_value(x, 'f', 0));  TEST_EQUAL(s, "-123");
    x = -123.456;  TRY(s = format_value(x, 'f', 2));  TEST_EQUAL(s, "-123.46");
    x = -123.456;  TRY(s = format_value(x, 'f', 4));  TEST_EQUAL(s, "-123.4560");

    x = 0;               TRY(s = format_value(x, 'g'));     TEST_EQUAL(s, "0");
    x = 0;               TRY(s = format_value(x, 'g', 0));  TEST_EQUAL(s, "0");
    x = 0;               TRY(s = format_value(x, 'g', 4));  TEST_EQUAL(s, "0");
    x = 123.456;         TRY(s = format_value(x, 'g'));     TEST_EQUAL(s, "123.456");
    x = 123.456;         TRY(s = format_value(x, 'g', 0));  TEST_EQUAL(s, "1e2");
    x = 123.456;         TRY(s = format_value(x, 'g', 4));  TEST_EQUAL(s, "123.5");
    x = 123.456;         TRY(s = format_value(x, 'g', 8));  TEST_EQUAL(s, "123.456");
    x = -123.456;        TRY(s = format_value(x, 'g'));     TEST_EQUAL(s, "-123.456");
    x = -123.456;        TRY(s = format_value(x, 'g', 0));  TEST_EQUAL(s, "-1e2");
    x = -123.456;        TRY(s = format_value(x, 'g', 4));  TEST_EQUAL(s, "-123.5");
    x = -123.456;        TRY(s = format_value(x, 'g', 8));  TEST_EQUAL(s, "-123.456");
    x = 123456000.0;     TRY(s = format_value(x, 'g'));     TEST_EQUAL(s, "1.23456e8");
    x = 123456000.0;     TRY(s = format_value(x, 'g', 0));  TEST_EQUAL(s, "1e8");
    x = 123456000.0;     TRY(s = format_value(x, 'g', 4));  TEST_EQUAL(s, "1.235e8");
    x = 123456000.0;     TRY(s = format_value(x, 'g', 8));  TEST_EQUAL(s, "1.23456e8");
    x = 0.000000123456;  TRY(s = format_value(x, 'g'));     TEST_EQUAL(s, "1.23456e-7");
    x = 0.000000123456;  TRY(s = format_value(x, 'g', 0));  TEST_EQUAL(s, "1e-7");
    x = 0.000000123456;  TRY(s = format_value(x, 'g', 4));  TEST_EQUAL(s, "1.235e-7");
    x = 0.000000123456;  TRY(s = format_value(x, 'g', 8));  TEST_EQUAL(s, "1.23456e-7");

    x = 123456000.0;     TRY(s = format_value(x, 'E', 8));  TEST_EQUAL(s, "1.23456000E8");
    x = 123456000.0;     TRY(s = format_value(x, 'G', 8));  TEST_EQUAL(s, "1.23456E8");

}
