#include "rs-intervals/interval.hpp"
#include "rs-intervals/map.hpp"
#include "rs-intervals/set.hpp"
#include "rs-intervals/types.hpp"
#include "test/stepwise-example.hpp"
#include "test/unit-test.hpp"
#include <string>

using namespace RS::Intervals;

using Itv = Interval<StepwiseType>;
using Map = IntervalMap<StepwiseType, std::string>;
using IB = IntervalBound;
using IC = IntervalCategory;
using IM = IntervalMatch;
using IO = IntervalOrder;

void test_rs_interval_stepwise_interval_map() {

    Map map;
    Map::iterator it;

    TEST(map.empty());
    TEST_EQUAL(map.size(), 0u);
    TEST_EQUAL(map.default_value(), "");
    TEST_EQUAL(map[42_sw], "");
    TEST_EQUAL(map.str(), "{}");

    TRY(map.insert(Itv(2_sw,2_sw,"<="), "alpha"));
    TRY(map.insert(Itv(3_sw,7_sw,"()"), "bravo"));
    TRY(map.insert(Itv(8_sw,8_sw,">="), "charlie"));
    TEST_EQUAL(map.size(), 3u);
    TEST_EQUAL(map[1_sw], "alpha");
    TEST_EQUAL(map[2_sw], "alpha");
    TEST_EQUAL(map[3_sw], "");
    TEST_EQUAL(map[4_sw], "bravo");
    TEST_EQUAL(map[5_sw], "bravo");
    TEST_EQUAL(map[6_sw], "bravo");
    TEST_EQUAL(map[7_sw], "");
    TEST_EQUAL(map[8_sw], "charlie");
    TEST_EQUAL(map[9_sw], "charlie");
    TRY(map.default_value("nil"));
    TEST_EQUAL(map[1_sw], "alpha");
    TEST_EQUAL(map[2_sw], "alpha");
    TEST_EQUAL(map[3_sw], "nil");
    TEST_EQUAL(map[4_sw], "bravo");
    TEST_EQUAL(map[5_sw], "bravo");
    TEST_EQUAL(map[6_sw], "bravo");
    TEST_EQUAL(map[7_sw], "nil");
    TEST_EQUAL(map[8_sw], "charlie");
    TEST_EQUAL(map[9_sw], "charlie");
    TEST_EQUAL(map.str(), "{<=2:alpha,[4,6]:bravo,>=8:charlie}");

    TEST(map.contains(1_sw));
    TEST(map.contains(2_sw));
    TEST(! map.contains(3_sw));
    TEST(map.contains(4_sw));
    TEST(map.contains(5_sw));
    TEST(map.contains(6_sw));
    TEST(! map.contains(7_sw));
    TEST(map.contains(8_sw));
    TEST(map.contains(9_sw));

    TRY(it = map.find(1_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "alpha");
    TRY(it = map.find(2_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "alpha");
    TRY(it = map.find(3_sw));  TEST(it == map.end());
    TRY(it = map.find(4_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.find(5_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.find(6_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.find(7_sw));  TEST(it == map.end());
    TRY(it = map.find(8_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.find(9_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");

    TRY(it = map.lower_bound(1_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "alpha");
    TRY(it = map.lower_bound(2_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "alpha");
    TRY(it = map.lower_bound(3_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.lower_bound(4_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.lower_bound(5_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.lower_bound(6_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.lower_bound(7_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.lower_bound(8_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.lower_bound(9_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");

    TRY(it = map.upper_bound(1_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.upper_bound(2_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.upper_bound(3_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.upper_bound(4_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.upper_bound(5_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.upper_bound(6_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.upper_bound(7_sw));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.upper_bound(8_sw));  TEST(it == map.end());
    TRY(it = map.upper_bound(9_sw));  TEST(it == map.end());

    TRY(map.clear());
    TEST(map.empty());
    TRY((map = {
        {Itv(20_sw,20_sw,"<="), "alpha"},
        {Itv(30_sw,70_sw,"()"), "bravo"},
        {Itv(80_sw,80_sw,">="), "charlie"},
    }));
    TEST_EQUAL(map.size(), 3u);
    TEST_EQUAL(map.str(), "{<=20:alpha,[31,69]:bravo,>=80:charlie}");

    TRY(map.clear());
    TRY(map.default_value("nil"));
    TRY(map.insert(Itv(10_sw,10_sw,"<="), "alpha"));
    TRY(map.insert(Itv(1_sw,5_sw,"[]"), "bravo"));
    TRY(map.insert(Itv(5_sw,6_sw,"[]"), "charlie"));
    TRY(map.insert(Itv(2_sw,3_sw,"[]"), "delta"));
    TRY(map.insert(Itv(7_sw,8_sw,"[]"), "charlie"));
    TEST_EQUAL(map.size(), 6u);
    TEST_EQUAL(map.str(),
        "{<=0:alpha,"
        "1:bravo,"
        "[2,3]:delta,"
        "4:bravo,"
        "[5,8]:charlie,"
        "[9,10]:alpha}");
    TEST_EQUAL(map[0_sw], "alpha");
    TEST_EQUAL(map[1_sw], "bravo");
    TEST_EQUAL(map[2_sw], "delta");
    TEST_EQUAL(map[3_sw], "delta");
    TEST_EQUAL(map[4_sw], "bravo");
    TEST_EQUAL(map[5_sw], "charlie");
    TEST_EQUAL(map[6_sw], "charlie");
    TEST_EQUAL(map[7_sw], "charlie");
    TEST_EQUAL(map[8_sw], "charlie");
    TEST_EQUAL(map[9_sw], "alpha");
    TEST_EQUAL(map[10_sw], "alpha");
    TEST_EQUAL(map[11_sw], "nil");
    TEST_EQUAL(map[12_sw], "nil");

    TRY(map.erase(Itv(1_sw,2_sw,"[]")));
    TRY(map.erase(Itv(6_sw,7_sw,"[]")));
    TRY(map.erase(Itv(10_sw,10_sw,">=")));
    TEST_EQUAL(map.size(), 6u);
    TEST_EQUAL(map.str(),
        "{<=0:alpha,"
        "3:delta,"
        "4:bravo,"
        "5:charlie,"
        "8:charlie,"
        "9:alpha}");
    TEST_EQUAL(map[0_sw], "alpha");
    TEST_EQUAL(map[1_sw], "nil");
    TEST_EQUAL(map[2_sw], "nil");
    TEST_EQUAL(map[3_sw], "delta");
    TEST_EQUAL(map[4_sw], "bravo");
    TEST_EQUAL(map[5_sw], "charlie");
    TEST_EQUAL(map[6_sw], "nil");
    TEST_EQUAL(map[7_sw], "nil");
    TEST_EQUAL(map[8_sw], "charlie");
    TEST_EQUAL(map[9_sw], "alpha");
    TEST_EQUAL(map[10_sw], "nil");
    TEST_EQUAL(map[11_sw], "nil");
    TEST_EQUAL(map[12_sw], "nil");

}
