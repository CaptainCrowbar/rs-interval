#include "rs-interval/type-traits.hpp"
#include "test/unit-test.hpp"
#include <chrono>
#include <complex>
#include <string>
#include <type_traits>
#include <vector>

using namespace RS::Intervals;

RS_INTERVAL_ENUM(Etype, uint32_t, 1, alpha, bravo, charlie)
RS_INTERVAL_ENUM_CLASS(Eclass, int64_t, -1, xray, yankee, zulu)

void test_rs_interval_enumeration() {

    std::string s;

    TEST(std::is_enum_v<Etype>);
    TEST(std::is_enum_v<Eclass>);

    TEST_TYPE(std::underlying_type_t<Etype>, uint32_t);
    TEST_TYPE(std::underlying_type_t<Eclass>, int64_t);

    TEST_EQUAL(uint32_t(alpha), 1u);
    TEST_EQUAL(uint32_t(bravo), 2u);
    TEST_EQUAL(uint32_t(charlie), 3u);
    TEST_EQUAL(int64_t(Eclass::xray), -1);
    TEST_EQUAL(int64_t(Eclass::yankee), 0);
    TEST_EQUAL(int64_t(Eclass::zulu), 1);

    TRY(s = to_string(alpha));           TEST_EQUAL(s, "alpha");
    TRY(s = to_string(bravo));           TEST_EQUAL(s, "bravo");
    TRY(s = to_string(charlie));         TEST_EQUAL(s, "charlie");
    TRY(s = to_string(Etype(0)));        TEST_EQUAL(s, "0");
    TRY(s = to_string(Eclass::xray));    TEST_EQUAL(s, "xray");
    TRY(s = to_string(Eclass::yankee));  TEST_EQUAL(s, "yankee");
    TRY(s = to_string(Eclass::zulu));    TEST_EQUAL(s, "zulu");
    TRY(s = to_string(Eclass(-2)));      TEST_EQUAL(s, "-2");

}

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
