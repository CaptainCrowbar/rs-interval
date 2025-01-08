#include "rs-interval/types.hpp"
#include "test/stepwise-example.hpp"
#include "test/unit-test.hpp"
#include <chrono>
#include <complex>
#include <cstdint>
#include <format>
#include <string>
#include <vector>

using namespace RS::Interval;
using namespace std::literals;

struct Thing {
    std::string name;
    Thing() = default;
    explicit Thing(const std::string& s): name(s) {}
};

void test_rs_interval_types_traits() {

    TEST_EQUAL(interval_category<bool>, Category::none);
    TEST_EQUAL(interval_category<const bool>, Category::none);
    TEST_EQUAL(interval_category<volatile bool>, Category::none);
    TEST_EQUAL(interval_category<const volatile bool>, Category::none);
    TEST_EQUAL(interval_category<void>, Category::none);
    TEST_EQUAL(interval_category<std::complex<float>>, Category::none);

    TEST_EQUAL(interval_category<std::string>, Category::ordered);
    TEST_EQUAL(interval_category<const std::string>, Category::ordered);
    TEST_EQUAL(interval_category<volatile std::string>, Category::ordered);
    TEST_EQUAL(interval_category<const volatile std::string>, Category::ordered);
    TEST_EQUAL(interval_category<std::vector<int>>, Category::ordered);
    TEST_EQUAL(interval_category<void*>, Category::ordered);
    TEST_EQUAL(interval_category<const void*>, Category::ordered);

    TEST_EQUAL(interval_category<char*>, Category::stepwise);
    TEST_EQUAL(interval_category<const char*>, Category::stepwise);
    TEST_EQUAL(interval_category<std::string::const_iterator>, Category::stepwise);
    TEST_EQUAL(interval_category<std::string::iterator>, Category::stepwise);
    TEST_EQUAL(interval_category<std::vector<int>::const_iterator>, Category::stepwise);
    TEST_EQUAL(interval_category<std::vector<int>::iterator>, Category::stepwise);
    TEST_EQUAL(interval_category<std::chrono::microseconds>, Category::stepwise);
    TEST_EQUAL(interval_category<StepwiseType>, Category::stepwise);

    TEST_EQUAL(interval_category<int>, Category::integral);
    TEST_EQUAL(interval_category<const int>, Category::integral);
    TEST_EQUAL(interval_category<volatile int>, Category::integral);
    TEST_EQUAL(interval_category<const volatile int>, Category::integral);
    TEST_EQUAL(interval_category<std::uint8_t>, Category::integral);
    TEST_EQUAL(interval_category<std::int64_t>, Category::integral);

    TEST_EQUAL(interval_category<float>, Category::continuous);
    TEST_EQUAL(interval_category<const float>, Category::continuous);
    TEST_EQUAL(interval_category<volatile float>, Category::continuous);
    TEST_EQUAL(interval_category<const volatile float>, Category::continuous);
    TEST_EQUAL(interval_category<double>, Category::continuous);

}

void test_rs_interval_types_concepts() {

    TEST(Ordered<std::string>);
    TEST(Ordered<const std::string>);
    TEST(Ordered<volatile std::string>);
    TEST(Ordered<const volatile std::string>);
    TEST(Ordered<std::vector<int>>);
    TEST(Ordered<void*>);
    TEST(Ordered<const void*>);
    TEST(! Ordered<char*>);
    TEST(! Ordered<const char*>);
    TEST(! Ordered<std::string::const_iterator>);
    TEST(! Ordered<std::string::iterator>);
    TEST(! Ordered<std::vector<int>::const_iterator>);
    TEST(! Ordered<std::vector<int>::iterator>);
    TEST(! Ordered<std::chrono::microseconds>);
    TEST(! Ordered<StepwiseType>);
    TEST(! Ordered<int>);
    TEST(! Ordered<const int>);
    TEST(! Ordered<volatile int>);
    TEST(! Ordered<const volatile int>);
    TEST(! Ordered<std::uint8_t>);
    TEST(! Ordered<std::int64_t>);
    TEST(! Ordered<float>);
    TEST(! Ordered<const float>);
    TEST(! Ordered<volatile float>);
    TEST(! Ordered<const volatile float>);
    TEST(! Ordered<double>);
    TEST(! Ordered<bool>);
    TEST(! Ordered<const bool>);
    TEST(! Ordered<volatile bool>);
    TEST(! Ordered<const volatile bool>);
    TEST(! Ordered<void>);
    TEST(! Ordered<std::complex<float>>);

    TEST(! Stepwise<std::string>);
    TEST(! Stepwise<const std::string>);
    TEST(! Stepwise<volatile std::string>);
    TEST(! Stepwise<const volatile std::string>);
    TEST(! Stepwise<std::vector<int>>);
    TEST(! Stepwise<void*>);
    TEST(! Stepwise<const void*>);
    TEST(Stepwise<char*>);
    TEST(Stepwise<const char*>);
    TEST(Stepwise<std::string::const_iterator>);
    TEST(Stepwise<std::string::iterator>);
    TEST(Stepwise<std::vector<int>::const_iterator>);
    TEST(Stepwise<std::vector<int>::iterator>);
    TEST(Stepwise<std::chrono::microseconds>);
    TEST(Stepwise<StepwiseType>);
    TEST(! Stepwise<int>);
    TEST(! Stepwise<const int>);
    TEST(! Stepwise<volatile int>);
    TEST(! Stepwise<const volatile int>);
    TEST(! Stepwise<std::uint8_t>);
    TEST(! Stepwise<std::int64_t>);
    TEST(! Stepwise<float>);
    TEST(! Stepwise<const float>);
    TEST(! Stepwise<volatile float>);
    TEST(! Stepwise<const volatile float>);
    TEST(! Stepwise<double>);
    TEST(! Stepwise<bool>);
    TEST(! Stepwise<const bool>);
    TEST(! Stepwise<volatile bool>);
    TEST(! Stepwise<const volatile bool>);
    TEST(! Stepwise<void>);
    TEST(! Stepwise<std::complex<float>>);

    TEST(! Integral<std::string>);
    TEST(! Integral<const std::string>);
    TEST(! Integral<volatile std::string>);
    TEST(! Integral<const volatile std::string>);
    TEST(! Integral<std::vector<int>>);
    TEST(! Integral<void*>);
    TEST(! Integral<const void*>);
    TEST(! Integral<char*>);
    TEST(! Integral<const char*>);
    TEST(! Integral<std::string::const_iterator>);
    TEST(! Integral<std::string::iterator>);
    TEST(! Integral<std::vector<int>::const_iterator>);
    TEST(! Integral<std::vector<int>::iterator>);
    TEST(! Integral<std::chrono::microseconds>);
    TEST(! Integral<StepwiseType>);
    TEST(Integral<int>);
    TEST(Integral<const int>);
    TEST(Integral<volatile int>);
    TEST(Integral<const volatile int>);
    TEST(Integral<std::uint8_t>);
    TEST(Integral<std::int64_t>);
    TEST(! Integral<float>);
    TEST(! Integral<const float>);
    TEST(! Integral<volatile float>);
    TEST(! Integral<const volatile float>);
    TEST(! Integral<double>);
    TEST(! Integral<bool>);
    TEST(! Integral<const bool>);
    TEST(! Integral<volatile bool>);
    TEST(! Integral<const volatile bool>);
    TEST(! Integral<void>);
    TEST(! Integral<std::complex<float>>);

    TEST(! Continuous<std::string>);
    TEST(! Continuous<const std::string>);
    TEST(! Continuous<volatile std::string>);
    TEST(! Continuous<const volatile std::string>);
    TEST(! Continuous<std::vector<int>>);
    TEST(! Continuous<void*>);
    TEST(! Continuous<const void*>);
    TEST(! Continuous<char*>);
    TEST(! Continuous<const char*>);
    TEST(! Continuous<std::string::const_iterator>);
    TEST(! Continuous<std::string::iterator>);
    TEST(! Continuous<std::vector<int>::const_iterator>);
    TEST(! Continuous<std::vector<int>::iterator>);
    TEST(! Continuous<std::chrono::microseconds>);
    TEST(! Continuous<StepwiseType>);
    TEST(! Continuous<int>);
    TEST(! Continuous<const int>);
    TEST(! Continuous<volatile int>);
    TEST(! Continuous<const volatile int>);
    TEST(! Continuous<std::uint8_t>);
    TEST(! Continuous<std::int64_t>);
    TEST(Continuous<float>);
    TEST(Continuous<const float>);
    TEST(Continuous<volatile float>);
    TEST(Continuous<const volatile float>);
    TEST(Continuous<double>);
    TEST(! Continuous<bool>);
    TEST(! Continuous<const bool>);
    TEST(! Continuous<volatile bool>);
    TEST(! Continuous<const volatile bool>);
    TEST(! Continuous<void>);
    TEST(! Continuous<std::complex<float>>);

    TEST(! Arithmetic<std::string>);
    TEST(! Arithmetic<const std::string>);
    TEST(! Arithmetic<volatile std::string>);
    TEST(! Arithmetic<const volatile std::string>);
    TEST(! Arithmetic<std::vector<int>>);
    TEST(! Arithmetic<void*>);
    TEST(! Arithmetic<const void*>);
    TEST(! Arithmetic<char*>);
    TEST(! Arithmetic<const char*>);
    TEST(! Arithmetic<std::string::const_iterator>);
    TEST(! Arithmetic<std::string::iterator>);
    TEST(! Arithmetic<std::vector<int>::const_iterator>);
    TEST(! Arithmetic<std::vector<int>::iterator>);
    TEST(! Arithmetic<std::chrono::microseconds>);
    TEST(! Arithmetic<StepwiseType>);
    TEST(Arithmetic<int>);
    TEST(Arithmetic<const int>);
    TEST(Arithmetic<volatile int>);
    TEST(Arithmetic<const volatile int>);
    TEST(Arithmetic<std::uint8_t>);
    TEST(Arithmetic<std::int64_t>);
    TEST(Arithmetic<float>);
    TEST(Arithmetic<const float>);
    TEST(Arithmetic<volatile float>);
    TEST(Arithmetic<const volatile float>);
    TEST(Arithmetic<double>);
    TEST(! Arithmetic<bool>);
    TEST(! Arithmetic<const bool>);
    TEST(! Arithmetic<volatile bool>);
    TEST(! Arithmetic<const volatile bool>);
    TEST(! Arithmetic<void>);
    TEST(! Arithmetic<std::complex<float>>);

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

    TEST(! Scalar<std::string>);
    TEST(! Scalar<const std::string>);
    TEST(! Scalar<volatile std::string>);
    TEST(! Scalar<const volatile std::string>);
    TEST(! Scalar<std::vector<int>>);
    TEST(! Scalar<void*>);
    TEST(! Scalar<const void*>);
    TEST(! Scalar<char*>);
    TEST(! Scalar<const char*>);
    TEST(! Scalar<std::string::const_iterator>);
    TEST(! Scalar<std::string::iterator>);
    TEST(! Scalar<std::vector<int>::const_iterator>);
    TEST(! Scalar<std::vector<int>::iterator>);
    TEST(! Scalar<std::chrono::microseconds>);
    TEST(! Scalar<StepwiseType>);
    TEST(! Scalar<int>);
    TEST(! Scalar<const int>);
    TEST(! Scalar<volatile int>);
    TEST(! Scalar<const volatile int>);
    TEST(! Scalar<std::uint8_t>);
    TEST(! Scalar<std::int64_t>);
    TEST(Scalar<float>);
    TEST(Scalar<const float>);
    TEST(Scalar<volatile float>);
    TEST(Scalar<const volatile float>);
    TEST(Scalar<double>);
    TEST(! Scalar<bool>);
    TEST(! Scalar<const bool>);
    TEST(! Scalar<volatile bool>);
    TEST(! Scalar<const volatile bool>);
    TEST(! Scalar<void>);
    TEST(! Scalar<std::complex<float>>);

}

void test_rs_interval_types_boundary() {

    using BT = Detail::BoundaryType;
    using BI = Detail::Boundary<int>;

    BI bi;
    std::string str;

    TRY((bi = {}));                        TRY(str = std::format("{}", bi));  TEST_EQUAL(str, "{}");
    TRY((bi = {{}, BT::empty}));           TRY(str = std::format("{}", bi));  TEST_EQUAL(str, "{}");
    TRY((bi = {{}, BT::minus_infinity}));  TRY(str = std::format("{}", bi));  TEST_EQUAL(str, "-inf");
    TRY((bi = {{}, BT::plus_infinity}));   TRY(str = std::format("{}", bi));  TEST_EQUAL(str, "+inf");
    TRY((bi = {42, BT::closed}));          TRY(str = std::format("{}", bi));  TEST_EQUAL(str, "42");
    TRY((bi = {42, BT::open}));            TRY(str = std::format("{}", bi));  TEST_EQUAL(str, "(42)");

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
