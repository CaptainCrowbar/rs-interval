#include "rs-interval/type-traits.hpp"
#include "test/unit-test.hpp"
#include <chrono>
#include <complex>
#include <string>
#include <vector>

using namespace RS::Intervals;

void test_rs_interval_type_traits() {

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
