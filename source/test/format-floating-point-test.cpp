#include "rs-interval/format.hpp"
#include "test/unit-test.hpp"
#include <string>

using namespace RS::Intervals;

void test_rs_interval_format_floating_point_with_significant_digits() {

    double x;
    std::string s;

    x = 0;               TRY(s = format_float(x, "d"));    TEST_EQUAL(s, "0.00000");
    x = 0;               TRY(s = format_float(x, "d0"));   TEST_EQUAL(s, "0");
    x = 0;               TRY(s = format_float(x, "d4"));   TEST_EQUAL(s, "0.000");
    x = 100;             TRY(s = format_float(x, "d"));    TEST_EQUAL(s, "100.000");
    x = 100;             TRY(s = format_float(x, "d0"));   TEST_EQUAL(s, "100");
    x = 100;             TRY(s = format_float(x, "d4"));   TEST_EQUAL(s, "100.0");
    x = 100;             TRY(s = format_float(x, "d8"));   TEST_EQUAL(s, "100.00000");
    x = 123.456;         TRY(s = format_float(x, "d"));    TEST_EQUAL(s, "123.456");
    x = 123.456;         TRY(s = format_float(x, "d0"));   TEST_EQUAL(s, "100");
    x = 123.456;         TRY(s = format_float(x, "d4"));   TEST_EQUAL(s, "123.5");
    x = 123.456;         TRY(s = format_float(x, "d8"));   TEST_EQUAL(s, "123.45600");
    x = -123.456;        TRY(s = format_float(x, "d"));    TEST_EQUAL(s, "-123.456");
    x = -123.456;        TRY(s = format_float(x, "d0"));   TEST_EQUAL(s, "-100");
    x = -123.456;        TRY(s = format_float(x, "d4"));   TEST_EQUAL(s, "-123.5");
    x = -123.456;        TRY(s = format_float(x, "d8"));   TEST_EQUAL(s, "-123.45600");
    x = 123456000.0;     TRY(s = format_float(x, "d"));    TEST_EQUAL(s, "123456000");
    x = 123456000.0;     TRY(s = format_float(x, "d0"));   TEST_EQUAL(s, "100000000");
    x = 123456000.0;     TRY(s = format_float(x, "d4"));   TEST_EQUAL(s, "123500000");
    x = 123456000.0;     TRY(s = format_float(x, "d8"));   TEST_EQUAL(s, "123456000");
    x = 0.000000123456;  TRY(s = format_float(x, "d"));    TEST_EQUAL(s, "0.000000123456");
    x = 0.000000123456;  TRY(s = format_float(x, "d0"));   TEST_EQUAL(s, "0.0000001");
    x = 0.000000123456;  TRY(s = format_float(x, "d4"));   TEST_EQUAL(s, "0.0000001235");
    x = 0.000000123456;  TRY(s = format_float(x, "d8"));   TEST_EQUAL(s, "0.00000012345600");
    x = 0;               TRY(s = format_float(x, "ds"));   TEST_EQUAL(s, "+0.00000");
    x = 123.456;         TRY(s = format_float(x, "ds"));   TEST_EQUAL(s, "+123.456");
    x = -123.456;        TRY(s = format_float(x, "ds"));   TEST_EQUAL(s, "-123.456");
    x = 0;               TRY(s = format_float(x, "dz"));   TEST_EQUAL(s, "0");
    x = 0;               TRY(s = format_float(x, "dz0"));  TEST_EQUAL(s, "0");
    x = 0;               TRY(s = format_float(x, "dz4"));  TEST_EQUAL(s, "0");
    x = 100;             TRY(s = format_float(x, "dz"));   TEST_EQUAL(s, "100");
    x = 100;             TRY(s = format_float(x, "dz0"));  TEST_EQUAL(s, "100");
    x = 100;             TRY(s = format_float(x, "dz4"));  TEST_EQUAL(s, "100");
    x = 100;             TRY(s = format_float(x, "dz8"));  TEST_EQUAL(s, "100");
    x = 123.456;         TRY(s = format_float(x, "dz"));   TEST_EQUAL(s, "123.456");
    x = 123.456;         TRY(s = format_float(x, "dz0"));  TEST_EQUAL(s, "100");
    x = 123.456;         TRY(s = format_float(x, "dz4"));  TEST_EQUAL(s, "123.5");
    x = 123.456;         TRY(s = format_float(x, "dz8"));  TEST_EQUAL(s, "123.456");
    x = -123.456;        TRY(s = format_float(x, "dz"));   TEST_EQUAL(s, "-123.456");
    x = -123.456;        TRY(s = format_float(x, "dz0"));  TEST_EQUAL(s, "-100");
    x = -123.456;        TRY(s = format_float(x, "dz4"));  TEST_EQUAL(s, "-123.5");
    x = -123.456;        TRY(s = format_float(x, "dz8"));  TEST_EQUAL(s, "-123.456");
    x = 123456000.0;     TRY(s = format_float(x, "dz"));   TEST_EQUAL(s, "123456000");
    x = 123456000.0;     TRY(s = format_float(x, "dz0"));  TEST_EQUAL(s, "100000000");
    x = 123456000.0;     TRY(s = format_float(x, "dz4"));  TEST_EQUAL(s, "123500000");
    x = 123456000.0;     TRY(s = format_float(x, "dz8"));  TEST_EQUAL(s, "123456000");
    x = 0.000000123456;  TRY(s = format_float(x, "dz"));   TEST_EQUAL(s, "0.000000123456");
    x = 0.000000123456;  TRY(s = format_float(x, "dz0"));  TEST_EQUAL(s, "0.0000001");
    x = 0.000000123456;  TRY(s = format_float(x, "dz4"));  TEST_EQUAL(s, "0.0000001235");
    x = 0.000000123456;  TRY(s = format_float(x, "dz8"));  TEST_EQUAL(s, "0.000000123456");
    x = 0;               TRY(s = format_float(x, "dsz"));  TEST_EQUAL(s, "+0");
    x = 123.456;         TRY(s = format_float(x, "dsz"));  TEST_EQUAL(s, "+123.456");
    x = -123.456;        TRY(s = format_float(x, "dsz"));  TEST_EQUAL(s, "-123.456");

}

void test_rs_interval_format_floating_point_with_scientific_notation() {

    double x;
    std::string s;

    x = 0;               TRY(s = format_float(x, "e"));    TEST_EQUAL(s, "0.00000e0");
    x = 0;               TRY(s = format_float(x, "e0"));   TEST_EQUAL(s, "0e0");
    x = 0;               TRY(s = format_float(x, "e2"));   TEST_EQUAL(s, "0.0e0");
    x = 100;             TRY(s = format_float(x, "e"));    TEST_EQUAL(s, "1.00000e2");
    x = 100;             TRY(s = format_float(x, "e0"));   TEST_EQUAL(s, "1e2");
    x = 100;             TRY(s = format_float(x, "e2"));   TEST_EQUAL(s, "1.0e2");
    x = 100;             TRY(s = format_float(x, "e8"));   TEST_EQUAL(s, "1.0000000e2");
    x = 123.456;         TRY(s = format_float(x, "e"));    TEST_EQUAL(s, "1.23456e2");
    x = 123.456;         TRY(s = format_float(x, "e0"));   TEST_EQUAL(s, "1e2");
    x = 123.456;         TRY(s = format_float(x, "e2"));   TEST_EQUAL(s, "1.2e2");
    x = 123.456;         TRY(s = format_float(x, "e8"));   TEST_EQUAL(s, "1.2345600e2");
    x = -123.456;        TRY(s = format_float(x, "e"));    TEST_EQUAL(s, "-1.23456e2");
    x = -123.456;        TRY(s = format_float(x, "e0"));   TEST_EQUAL(s, "-1e2");
    x = -123.456;        TRY(s = format_float(x, "e2"));   TEST_EQUAL(s, "-1.2e2");
    x = -123.456;        TRY(s = format_float(x, "e8"));   TEST_EQUAL(s, "-1.2345600e2");
    x = 123456000.0;     TRY(s = format_float(x, "e"));    TEST_EQUAL(s, "1.23456e8");
    x = 123456000.0;     TRY(s = format_float(x, "e0"));   TEST_EQUAL(s, "1e8");
    x = 123456000.0;     TRY(s = format_float(x, "e2"));   TEST_EQUAL(s, "1.2e8");
    x = 123456000.0;     TRY(s = format_float(x, "e8"));   TEST_EQUAL(s, "1.2345600e8");
    x = 0.000000123456;  TRY(s = format_float(x, "e"));    TEST_EQUAL(s, "1.23456e-7");
    x = 0.000000123456;  TRY(s = format_float(x, "e0"));   TEST_EQUAL(s, "1e-7");
    x = 0.000000123456;  TRY(s = format_float(x, "e2"));   TEST_EQUAL(s, "1.2e-7");
    x = 0.000000123456;  TRY(s = format_float(x, "e8"));   TEST_EQUAL(s, "1.2345600e-7");
    x = 0;               TRY(s = format_float(x, "es"));   TEST_EQUAL(s, "+0.00000e0");
    x = 123.456;         TRY(s = format_float(x, "es"));   TEST_EQUAL(s, "+1.23456e2");
    x = -123.456;        TRY(s = format_float(x, "es"));   TEST_EQUAL(s, "-1.23456e2");
    x = 100;             TRY(s = format_float(x, "ez"));   TEST_EQUAL(s, "1e2");
    x = 100;             TRY(s = format_float(x, "ez0"));  TEST_EQUAL(s, "1e2");
    x = 100;             TRY(s = format_float(x, "ez2"));  TEST_EQUAL(s, "1e2");
    x = 100;             TRY(s = format_float(x, "ez8"));  TEST_EQUAL(s, "1e2");
    x = 123.456;         TRY(s = format_float(x, "ez"));   TEST_EQUAL(s, "1.23456e2");
    x = 123.456;         TRY(s = format_float(x, "ez0"));  TEST_EQUAL(s, "1e2");
    x = 123.456;         TRY(s = format_float(x, "ez2"));  TEST_EQUAL(s, "1.2e2");
    x = 123.456;         TRY(s = format_float(x, "ez8"));  TEST_EQUAL(s, "1.23456e2");

}

void test_rs_interval_format_floating_point_with_fixed_point() {

    double x;
    std::string s;

    x = 0;               TRY(s = format_float(x, "f"));    TEST_EQUAL(s, "0.000000");
    x = 0;               TRY(s = format_float(x, "f0"));   TEST_EQUAL(s, "0");
    x = 0;               TRY(s = format_float(x, "f2"));   TEST_EQUAL(s, "0.00");
    x = 100;             TRY(s = format_float(x, "f"));    TEST_EQUAL(s, "100.000000");
    x = 100;             TRY(s = format_float(x, "f0"));   TEST_EQUAL(s, "100");
    x = 100;             TRY(s = format_float(x, "f2"));   TEST_EQUAL(s, "100.00");
    x = 100;             TRY(s = format_float(x, "f4"));   TEST_EQUAL(s, "100.0000");
    x = 123.456;         TRY(s = format_float(x, "f"));    TEST_EQUAL(s, "123.456000");
    x = 123.456;         TRY(s = format_float(x, "f0"));   TEST_EQUAL(s, "123");
    x = 123.456;         TRY(s = format_float(x, "f2"));   TEST_EQUAL(s, "123.46");
    x = 123.456;         TRY(s = format_float(x, "f4"));   TEST_EQUAL(s, "123.4560");
    x = -123.456;        TRY(s = format_float(x, "f"));    TEST_EQUAL(s, "-123.456000");
    x = -123.456;        TRY(s = format_float(x, "f0"));   TEST_EQUAL(s, "-123");
    x = -123.456;        TRY(s = format_float(x, "f2"));   TEST_EQUAL(s, "-123.46");
    x = -123.456;        TRY(s = format_float(x, "f4"));   TEST_EQUAL(s, "-123.4560");
    x = 0;               TRY(s = format_float(x, "fs"));   TEST_EQUAL(s, "+0.000000");
    x = 123.456;         TRY(s = format_float(x, "fs"));   TEST_EQUAL(s, "+123.456000");
    x = -123.456;        TRY(s = format_float(x, "fs"));   TEST_EQUAL(s, "-123.456000");
    x = 100;             TRY(s = format_float(x, "fz"));   TEST_EQUAL(s, "100");
    x = 100;             TRY(s = format_float(x, "fz0"));  TEST_EQUAL(s, "100");
    x = 100;             TRY(s = format_float(x, "fz2"));  TEST_EQUAL(s, "100");
    x = 100;             TRY(s = format_float(x, "fz4"));  TEST_EQUAL(s, "100");
    x = 123.456;         TRY(s = format_float(x, "fz"));   TEST_EQUAL(s, "123.456");
    x = 123.456;         TRY(s = format_float(x, "fz0"));  TEST_EQUAL(s, "123");
    x = 123.456;         TRY(s = format_float(x, "fz2"));  TEST_EQUAL(s, "123.46");
    x = 123.456;         TRY(s = format_float(x, "fz4"));  TEST_EQUAL(s, "123.456");

}

void test_rs_interval_format_floating_point_with_general_format() {

    double x;
    std::string s;

    x = 0;               TRY(s = format_float(x, "g"));    TEST_EQUAL(s, "0.00000");
    x = 0;               TRY(s = format_float(x, "g0"));   TEST_EQUAL(s, "0");
    x = 0;               TRY(s = format_float(x, "g4"));   TEST_EQUAL(s, "0.000");
    x = 100;             TRY(s = format_float(x, "g"));    TEST_EQUAL(s, "100.000");
    x = 100;             TRY(s = format_float(x, "g0"));   TEST_EQUAL(s, "100");
    x = 100;             TRY(s = format_float(x, "g4"));   TEST_EQUAL(s, "100.0");
    x = 100;             TRY(s = format_float(x, "g8"));   TEST_EQUAL(s, "100.00000");
    x = 123.456;         TRY(s = format_float(x, "g"));    TEST_EQUAL(s, "123.456");
    x = 123.456;         TRY(s = format_float(x, "g0"));   TEST_EQUAL(s, "100");
    x = 123.456;         TRY(s = format_float(x, "g4"));   TEST_EQUAL(s, "123.5");
    x = 123.456;         TRY(s = format_float(x, "g8"));   TEST_EQUAL(s, "123.45600");
    x = -123.456;        TRY(s = format_float(x, "g"));    TEST_EQUAL(s, "-123.456");
    x = -123.456;        TRY(s = format_float(x, "g0"));   TEST_EQUAL(s, "-100");
    x = -123.456;        TRY(s = format_float(x, "g4"));   TEST_EQUAL(s, "-123.5");
    x = -123.456;        TRY(s = format_float(x, "g8"));   TEST_EQUAL(s, "-123.45600");
    x = 123456000.0;     TRY(s = format_float(x, "g"));    TEST_EQUAL(s, "1.23456e8");
    x = 123456000.0;     TRY(s = format_float(x, "g0"));   TEST_EQUAL(s, "1e8");
    x = 123456000.0;     TRY(s = format_float(x, "g4"));   TEST_EQUAL(s, "1.235e8");
    x = 123456000.0;     TRY(s = format_float(x, "g8"));   TEST_EQUAL(s, "1.2345600e8");
    x = 0.000000123456;  TRY(s = format_float(x, "g"));    TEST_EQUAL(s, "1.23456e-7");
    x = 0.000000123456;  TRY(s = format_float(x, "g0"));   TEST_EQUAL(s, "1e-7");
    x = 0.000000123456;  TRY(s = format_float(x, "g4"));   TEST_EQUAL(s, "1.235e-7");
    x = 0.000000123456;  TRY(s = format_float(x, "g8"));   TEST_EQUAL(s, "1.2345600e-7");
    x = 0;               TRY(s = format_float(x, "gs"));   TEST_EQUAL(s, "+0.00000");
    x = 123.456;         TRY(s = format_float(x, "gs"));   TEST_EQUAL(s, "+123.456");
    x = -123.456;        TRY(s = format_float(x, "gs"));   TEST_EQUAL(s, "-123.456");
    x = 123456000.0;     TRY(s = format_float(x, "gs"));   TEST_EQUAL(s, "+1.23456e8");
    x = 0.000000123456;  TRY(s = format_float(x, "gs"));   TEST_EQUAL(s, "+1.23456e-7");
    x = 0;               TRY(s = format_float(x, "gz"));   TEST_EQUAL(s, "0");
    x = 0;               TRY(s = format_float(x, "gz0"));  TEST_EQUAL(s, "0");
    x = 0;               TRY(s = format_float(x, "gz4"));  TEST_EQUAL(s, "0");
    x = 100;             TRY(s = format_float(x, "gz"));   TEST_EQUAL(s, "100");
    x = 100;             TRY(s = format_float(x, "gz0"));  TEST_EQUAL(s, "100");
    x = 100;             TRY(s = format_float(x, "gz4"));  TEST_EQUAL(s, "100");
    x = 100;             TRY(s = format_float(x, "gz8"));  TEST_EQUAL(s, "100");
    x = 123.456;         TRY(s = format_float(x, "gz"));   TEST_EQUAL(s, "123.456");
    x = 123.456;         TRY(s = format_float(x, "gz0"));  TEST_EQUAL(s, "100");
    x = 123.456;         TRY(s = format_float(x, "gz4"));  TEST_EQUAL(s, "123.5");
    x = 123.456;         TRY(s = format_float(x, "gz8"));  TEST_EQUAL(s, "123.456");
    x = -123.456;        TRY(s = format_float(x, "gz"));   TEST_EQUAL(s, "-123.456");
    x = -123.456;        TRY(s = format_float(x, "gz0"));  TEST_EQUAL(s, "-100");
    x = -123.456;        TRY(s = format_float(x, "gz4"));  TEST_EQUAL(s, "-123.5");
    x = -123.456;        TRY(s = format_float(x, "gz8"));  TEST_EQUAL(s, "-123.456");
    x = 123456000.0;     TRY(s = format_float(x, "gz"));   TEST_EQUAL(s, "1.23456e8");
    x = 123456000.0;     TRY(s = format_float(x, "gz0"));  TEST_EQUAL(s, "1e8");
    x = 123456000.0;     TRY(s = format_float(x, "gz4"));  TEST_EQUAL(s, "1.235e8");
    x = 123456000.0;     TRY(s = format_float(x, "gz8"));  TEST_EQUAL(s, "1.23456e8");
    x = 0.000000123456;  TRY(s = format_float(x, "gz"));   TEST_EQUAL(s, "1.23456e-7");
    x = 0.000000123456;  TRY(s = format_float(x, "gz0"));  TEST_EQUAL(s, "1e-7");
    x = 0.000000123456;  TRY(s = format_float(x, "gz4"));  TEST_EQUAL(s, "1.235e-7");
    x = 0.000000123456;  TRY(s = format_float(x, "gz8"));  TEST_EQUAL(s, "1.23456e-7");

}
