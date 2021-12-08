#include "rs-interval/format.hpp"
#include "test/unit-test.hpp"
#include <map>
#include <string>
#include <vector>

using namespace RS::Intervals;
using namespace RS::Intervals::Detail;

void test_rs_interval_format_integer() {

    int n;
    unsigned u;
    std::string s;

    n = 0;           TRY(s = format_integer(n));        TEST_EQUAL(s, "0");
    n = 42;          TRY(s = format_integer(n));        TEST_EQUAL(s, "42");
    n = -42;         TRY(s = format_integer(n));        TEST_EQUAL(s, "-42");
    n = 123456789;   TRY(s = format_integer(n));        TEST_EQUAL(s, "123456789");
    n = -123456789;  TRY(s = format_integer(n));        TEST_EQUAL(s, "-123456789");
    n = 0;           TRY(s = format_integer(n, "n"));   TEST_EQUAL(s, "0");
    n = 42;          TRY(s = format_integer(n, "n"));   TEST_EQUAL(s, "42");
    n = -42;         TRY(s = format_integer(n, "n"));   TEST_EQUAL(s, "-42");
    n = 123456789;   TRY(s = format_integer(n, "n"));   TEST_EQUAL(s, "123456789");
    n = -123456789;  TRY(s = format_integer(n, "n"));   TEST_EQUAL(s, "-123456789");
    n = 0;           TRY(s = format_integer(n, "ns"));  TEST_EQUAL(s, "+0");
    n = 42;          TRY(s = format_integer(n, "ns"));  TEST_EQUAL(s, "+42");
    n = -42;         TRY(s = format_integer(n, "ns"));  TEST_EQUAL(s, "-42");
    n = 123456789;   TRY(s = format_integer(n, "ns"));  TEST_EQUAL(s, "+123456789");
    n = -123456789;  TRY(s = format_integer(n, "ns"));  TEST_EQUAL(s, "-123456789");
    n = 0;           TRY(s = format_integer(n, "n4"));  TEST_EQUAL(s, "0000");
    n = 42;          TRY(s = format_integer(n, "n4"));  TEST_EQUAL(s, "0042");
    n = -42;         TRY(s = format_integer(n, "n4"));  TEST_EQUAL(s, "-0042");
    n = 123456789;   TRY(s = format_integer(n, "n4"));  TEST_EQUAL(s, "123456789");
    n = -123456789;  TRY(s = format_integer(n, "n4"));  TEST_EQUAL(s, "-123456789");
    n = 0;           TRY(s = format_integer(n, "n0"));  TEST_EQUAL(s, "");
    n = 42;          TRY(s = format_integer(n, "n0"));  TEST_EQUAL(s, "42");
    n = -42;         TRY(s = format_integer(n, "n0"));  TEST_EQUAL(s, "-42");
    n = 0;           TRY(s = format_integer(n, "x"));   TEST_EQUAL(s, "0");
    n = 42;          TRY(s = format_integer(n, "x"));   TEST_EQUAL(s, "2a");
    n = -42;         TRY(s = format_integer(n, "x"));   TEST_EQUAL(s, "-2a");
    n = 123456789;   TRY(s = format_integer(n, "x"));   TEST_EQUAL(s, "75bcd15");
    n = -123456789;  TRY(s = format_integer(n, "x"));   TEST_EQUAL(s, "-75bcd15");
    n = 0;           TRY(s = format_integer(n, "x4"));  TEST_EQUAL(s, "0000");
    n = 42;          TRY(s = format_integer(n, "x4"));  TEST_EQUAL(s, "002a");
    n = -42;         TRY(s = format_integer(n, "x4"));  TEST_EQUAL(s, "-002a");
    n = 123456789;   TRY(s = format_integer(n, "x4"));  TEST_EQUAL(s, "75bcd15");
    n = -123456789;  TRY(s = format_integer(n, "x4"));  TEST_EQUAL(s, "-75bcd15");

    u = 0;           TRY(s = format_integer(u));        TEST_EQUAL(s, "0");
    u = 42;          TRY(s = format_integer(u));        TEST_EQUAL(s, "42");
    u = 123456789;   TRY(s = format_integer(u));        TEST_EQUAL(s, "123456789");
    u = 0;           TRY(s = format_integer(u, "n"));   TEST_EQUAL(s, "0");
    u = 42;          TRY(s = format_integer(u, "n"));   TEST_EQUAL(s, "42");
    u = 123456789;   TRY(s = format_integer(u, "n"));   TEST_EQUAL(s, "123456789");
    u = 0;           TRY(s = format_integer(u, "ns"));  TEST_EQUAL(s, "+0");
    u = 42;          TRY(s = format_integer(u, "ns"));  TEST_EQUAL(s, "+42");
    u = 123456789;   TRY(s = format_integer(u, "ns"));  TEST_EQUAL(s, "+123456789");
    u = 0;           TRY(s = format_integer(u, "n4"));  TEST_EQUAL(s, "0000");
    u = 42;          TRY(s = format_integer(u, "n4"));  TEST_EQUAL(s, "0042");
    u = 123456789;   TRY(s = format_integer(u, "n4"));  TEST_EQUAL(s, "123456789");
    u = 0;           TRY(s = format_integer(u, "n0"));  TEST_EQUAL(s, "");
    u = 42;          TRY(s = format_integer(u, "n0"));  TEST_EQUAL(s, "42");
    u = 0;           TRY(s = format_integer(u, "x"));   TEST_EQUAL(s, "0");
    u = 42;          TRY(s = format_integer(u, "x"));   TEST_EQUAL(s, "2a");
    u = 123456789;   TRY(s = format_integer(u, "x"));   TEST_EQUAL(s, "75bcd15");
    u = 0;           TRY(s = format_integer(u, "x4"));  TEST_EQUAL(s, "0000");
    u = 42;          TRY(s = format_integer(u, "x4"));  TEST_EQUAL(s, "002a");
    u = 123456789;   TRY(s = format_integer(u, "x4"));  TEST_EQUAL(s, "75bcd15");

}

void test_rs_interval_format_floating_point() {

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

void test_rs_interval_format_ranges() {

    std::vector<int> vec;
    std::map<int, std::string> map;
    std::string s;

    vec = {};         TRY(s = format_object(vec));  TEST_EQUAL(s, "[]");
    vec = {1};        TRY(s = format_object(vec));  TEST_EQUAL(s, "[1]");
    vec = {1,2};      TRY(s = format_object(vec));  TEST_EQUAL(s, "[1,2]");
    vec = {1,2,3};    TRY(s = format_object(vec));  TEST_EQUAL(s, "[1,2,3]");
    vec = {1,2,3,4};  TRY(s = format_object(vec));  TEST_EQUAL(s, "[1,2,3,4]");

    map = {};                                         TRY(s = format_object(map));  TEST_EQUAL(s, "{}");
    map = {{1,"abc"}};                                TRY(s = format_object(map));  TEST_EQUAL(s, "{1:abc}");
    map = {{1,"abc"},{2,"def"}};                      TRY(s = format_object(map));  TEST_EQUAL(s, "{1:abc,2:def}");
    map = {{1,"abc"},{2,"def"},{3,"ghi"}};            TRY(s = format_object(map));  TEST_EQUAL(s, "{1:abc,2:def,3:ghi}");
    map = {{1,"abc"},{2,"def"},{3,"ghi"},{4,"jkl"}};  TRY(s = format_object(map));  TEST_EQUAL(s, "{1:abc,2:def,3:ghi,4:jkl}");

}
