#include "rs-intervals/types.hpp"
#include "test/stepwise-example.hpp"
#include "test/unit-test.hpp"
#include <chrono>
#include <complex>
#include <cstddef>
#include <string>
#include <vector>

using namespace RS::Intervals;
using namespace std::literals;

using IB = IntervalBound;
using IC = IntervalCategory;
using IM = IntervalMatch;
using IO = IntervalOrder;

struct Thing {
    std::string name;
    Thing() = default;
    explicit Thing(const std::string& s): name(s) {}
};

void test_rs_interval_types_traits() {

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
    TEST_EQUAL(interval_category<StepwiseType>, IC::stepwise);

    TEST_EQUAL(interval_category<int>, IC::integral);
    TEST_EQUAL(interval_category<const int>, IC::integral);
    TEST_EQUAL(interval_category<volatile int>, IC::integral);
    TEST_EQUAL(interval_category<const volatile int>, IC::integral);
    TEST_EQUAL(interval_category<std::uint8_t>, IC::integral);
    TEST_EQUAL(interval_category<std::int64_t>, IC::integral);

    TEST_EQUAL(interval_category<float>, IC::continuous);
    TEST_EQUAL(interval_category<const float>, IC::continuous);
    TEST_EQUAL(interval_category<volatile float>, IC::continuous);
    TEST_EQUAL(interval_category<const volatile float>, IC::continuous);
    TEST_EQUAL(interval_category<double>, IC::continuous);

    TEST(Ordered<std::string>);
    TEST(Ordered<const std::string>);
    TEST(Ordered<volatile std::string>);
    TEST(Ordered<const volatile std::string>);
    TEST(Ordered<std::vector<int>>);
    TEST(Ordered<void*>);
    TEST(Ordered<const void*>);

    TEST(Stepwise<char*>);
    TEST(Stepwise<const char*>);
    TEST(Stepwise<std::string::const_iterator>);
    TEST(Stepwise<std::string::iterator>);
    TEST(Stepwise<std::vector<int>::const_iterator>);
    TEST(Stepwise<std::vector<int>::iterator>);
    TEST(Stepwise<std::chrono::microseconds>);
    TEST(Stepwise<StepwiseType>);

    TEST(Integral<int>);
    TEST(Integral<const int>);
    TEST(Integral<volatile int>);
    TEST(Integral<const volatile int>);
    TEST(Integral<std::uint8_t>);
    TEST(Integral<std::int64_t>);

    TEST(Continuous<float>);
    TEST(Continuous<const float>);
    TEST(Continuous<volatile float>);
    TEST(Continuous<const volatile float>);
    TEST(Continuous<double>);

    TEST(IntervalCompatible<std::string>);
    TEST(IntervalCompatible<const std::string>);
    TEST(IntervalCompatible<volatile std::string>);
    TEST(IntervalCompatible<const volatile std::string>);
    TEST(IntervalCompatible<std::vector<int>>);
    TEST(IntervalCompatible<void*>);
    TEST(IntervalCompatible<const void*>);
    TEST(IntervalCompatible<char*>);
    TEST(IntervalCompatible<const char*>);
    TEST(IntervalCompatible<std::string::const_iterator>);
    TEST(IntervalCompatible<std::string::iterator>);
    TEST(IntervalCompatible<std::vector<int>::const_iterator>);
    TEST(IntervalCompatible<std::vector<int>::iterator>);
    TEST(IntervalCompatible<std::chrono::microseconds>);
    TEST(IntervalCompatible<StepwiseType>);
    TEST(IntervalCompatible<int>);
    TEST(IntervalCompatible<const int>);
    TEST(IntervalCompatible<volatile int>);
    TEST(IntervalCompatible<const volatile int>);
    TEST(IntervalCompatible<std::uint8_t>);
    TEST(IntervalCompatible<std::int64_t>);
    TEST(IntervalCompatible<float>);
    TEST(IntervalCompatible<const float>);
    TEST(IntervalCompatible<volatile float>);
    TEST(IntervalCompatible<const volatile float>);
    TEST(IntervalCompatible<double>);

    TEST(! IntervalCompatible<bool>);
    TEST(! IntervalCompatible<const bool>);
    TEST(! IntervalCompatible<volatile bool>);
    TEST(! IntervalCompatible<const volatile bool>);
    TEST(! IntervalCompatible<void>);
    TEST(! IntervalCompatible<std::complex<float>>);

}

void test_rs_interval_types_from_string() {

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

void test_rs_interval_types_to_string() {

    TEST((Formatter<DefaultFormatter, int>));
    TEST((Formatter<DefaultFormatter, std::string>));

    TEST_EQUAL(DefaultFormatter()(42),        "42");
    TEST_EQUAL(DefaultFormatter()(123.456),   "123.456");
    TEST_EQUAL(DefaultFormatter()("hello"),   "hello");
    TEST_EQUAL(DefaultFormatter()("hello"s),  "hello");

}
