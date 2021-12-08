#include "rs-interval/format.hpp"
#include "test/unit-test.hpp"
#include <map>
#include <string>
#include <vector>

using namespace RS::Intervals;

void test_rs_interval_format_signed_integers() {

    int n;
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
    n = 0;           TRY(s = format_integer(n, "X"));   TEST_EQUAL(s, "0");
    n = 42;          TRY(s = format_integer(n, "X"));   TEST_EQUAL(s, "2A");
    n = -42;         TRY(s = format_integer(n, "X"));   TEST_EQUAL(s, "-2A");
    n = 123456789;   TRY(s = format_integer(n, "X"));   TEST_EQUAL(s, "75BCD15");
    n = -123456789;  TRY(s = format_integer(n, "X"));   TEST_EQUAL(s, "-75BCD15");

}

void test_rs_interval_format_unsigned_integers() {

    unsigned u;
    std::string s;

    u = 0;          TRY(s = format_integer(u));        TEST_EQUAL(s, "0");
    u = 42;         TRY(s = format_integer(u));        TEST_EQUAL(s, "42");
    u = 123456789;  TRY(s = format_integer(u));        TEST_EQUAL(s, "123456789");
    u = 0;          TRY(s = format_integer(u, "n"));   TEST_EQUAL(s, "0");
    u = 42;         TRY(s = format_integer(u, "n"));   TEST_EQUAL(s, "42");
    u = 123456789;  TRY(s = format_integer(u, "n"));   TEST_EQUAL(s, "123456789");
    u = 0;          TRY(s = format_integer(u, "ns"));  TEST_EQUAL(s, "+0");
    u = 42;         TRY(s = format_integer(u, "ns"));  TEST_EQUAL(s, "+42");
    u = 123456789;  TRY(s = format_integer(u, "ns"));  TEST_EQUAL(s, "+123456789");
    u = 0;          TRY(s = format_integer(u, "n4"));  TEST_EQUAL(s, "0000");
    u = 42;         TRY(s = format_integer(u, "n4"));  TEST_EQUAL(s, "0042");
    u = 123456789;  TRY(s = format_integer(u, "n4"));  TEST_EQUAL(s, "123456789");
    u = 0;          TRY(s = format_integer(u, "n0"));  TEST_EQUAL(s, "");
    u = 42;         TRY(s = format_integer(u, "n0"));  TEST_EQUAL(s, "42");
    u = 0;          TRY(s = format_integer(u, "x"));   TEST_EQUAL(s, "0");
    u = 42;         TRY(s = format_integer(u, "x"));   TEST_EQUAL(s, "2a");
    u = 123456789;  TRY(s = format_integer(u, "x"));   TEST_EQUAL(s, "75bcd15");
    u = 0;          TRY(s = format_integer(u, "x4"));  TEST_EQUAL(s, "0000");
    u = 42;         TRY(s = format_integer(u, "x4"));  TEST_EQUAL(s, "002a");
    u = 123456789;  TRY(s = format_integer(u, "x4"));  TEST_EQUAL(s, "75bcd15");
    u = 0;          TRY(s = format_integer(u, "X"));   TEST_EQUAL(s, "0");
    u = 42;         TRY(s = format_integer(u, "X"));   TEST_EQUAL(s, "2A");
    u = 123456789;  TRY(s = format_integer(u, "X"));   TEST_EQUAL(s, "75BCD15");

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
