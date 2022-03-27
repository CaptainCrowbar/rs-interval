#include "rs-interval/types.hpp"
#include "test/stepwise.hpp"
#include "rs-unit-test.hpp"
#include <chrono>
#include <complex>
#include <string>
#include <vector>

using namespace RS::Intervals;

using IB = IntervalBound;
using IC = IntervalCategory;
using IM = IntervalMatch;
using IO = IntervalOrder;

struct Thing {
    std::string name;
    Thing() = default;
    explicit Thing(const std::string& s): name(s) {}
};

void test_rs_interval_type_traits() {

    TEST_EQUAL(interval_category<bool>, IC::none);
    TEST_EQUAL(interval_category<const bool>, IC::none);
    TEST_EQUAL(interval_category<volatile bool>, IC::none);
    TEST_EQUAL(interval_category<const volatile bool>, IC::none);
    TEST_EQUAL(interval_category<void>, IC::none);
    TEST_EQUAL(interval_category<std::complex<float>>, IC::none);

    TEST_EQUAL(interval_category<std::string>, IC::ordered);
    TEST_EQUAL(interval_category<const std::string>, IC::ordered);
    TEST_EQUAL(interval_category<volatile std::string>, IC::ordered);
    TEST_EQUAL(interval_category<const volatile std::string>, IC::ordered);
    TEST_EQUAL(interval_category<std::vector<int>>, IC::ordered);
    TEST_EQUAL(interval_category<void*>, IC::ordered);
    TEST_EQUAL(interval_category<const void*>, IC::ordered);

    TEST_EQUAL(interval_category<char*>, IC::stepwise);
    TEST_EQUAL(interval_category<const char*>, IC::stepwise);
    TEST_EQUAL(interval_category<std::string::const_iterator>, IC::stepwise);
    TEST_EQUAL(interval_category<std::string::iterator>, IC::stepwise);
    TEST_EQUAL(interval_category<std::vector<int>::const_iterator>, IC::stepwise);
    TEST_EQUAL(interval_category<std::vector<int>::iterator>, IC::stepwise);
    TEST_EQUAL(interval_category<std::chrono::microseconds>, IC::stepwise);
    TEST_EQUAL(interval_category<Stepwise>, IC::stepwise);

    TEST_EQUAL(interval_category<int>, IC::integral);
    TEST_EQUAL(interval_category<const int>, IC::integral);
    TEST_EQUAL(interval_category<volatile int>, IC::integral);
    TEST_EQUAL(interval_category<const volatile int>, IC::integral);
    TEST_EQUAL(interval_category<uint8_t>, IC::integral);
    TEST_EQUAL(interval_category<int64_t>, IC::integral);

    TEST_EQUAL(interval_category<float>, IC::continuous);
    TEST_EQUAL(interval_category<const float>, IC::continuous);
    TEST_EQUAL(interval_category<volatile float>, IC::continuous);
    TEST_EQUAL(interval_category<const volatile float>, IC::continuous);
    TEST_EQUAL(interval_category<double>, IC::continuous);

}

void test_rs_interval_string_conversion() {

    int n = 0;
    std::string s;
    Thing t;

    s = "Hello";
    TRY(t = Detail::from_string<Thing>(s));
    TEST_EQUAL(t.name, "Hello");

    s = "42";
    TRY(n = Detail::from_string<int>(s));
    TEST_EQUAL(n, 42);

}
