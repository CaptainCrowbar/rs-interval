#include "rs-intervals/version.hpp"
#include "test/unit-test.hpp"

using namespace RS::Intervals;

void test_rs_interval_version() {

    TEST_IN_RANGE(version()[0], 0, 100);
    TEST_IN_RANGE(version()[1], 0, 1'000);
    TEST_IN_RANGE(version()[2], 0, 10'000);
    TEST_MATCH(version_string(), R"(\d+\.\d+\.\d+)");

}
