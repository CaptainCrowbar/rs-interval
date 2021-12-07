#include "rs-interval/interval.hpp"
#include "test/unit-test.hpp"
#include <string>

using namespace RS::Intervals;
using namespace RS::Intervals::Detail;

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
