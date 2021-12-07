#include "rs-interval/interval.hpp"
#include "test/unit-test.hpp"
#include <string>

using namespace RS;
using namespace RS::Intervals;

using IntervalType = Interval<int>;
using MapType = IntervalMap<int, std::string>;

void test_rs_integral_interval_map() {

    MapType map;
    MapType::iterator it;

    TEST(map.empty());
    TEST_EQUAL(map.size(), 0u);
    TEST_EQUAL(map.default_value(), "");
    TEST_EQUAL(map[42], "");
    TEST_EQUAL(map.str(), "{}");

    TRY(map.insert(IntervalType(2,2,"<="), "alpha"));
    TRY(map.insert(IntervalType(3,7,"()"), "bravo"));
    TRY(map.insert(IntervalType(8,8,">="), "charlie"));
    TEST_EQUAL(map.size(), 3u);
    TEST_EQUAL(map[1], "alpha");
    TEST_EQUAL(map[2], "alpha");
    TEST_EQUAL(map[3], "");
    TEST_EQUAL(map[4], "bravo");
    TEST_EQUAL(map[5], "bravo");
    TEST_EQUAL(map[6], "bravo");
    TEST_EQUAL(map[7], "");
    TEST_EQUAL(map[8], "charlie");
    TEST_EQUAL(map[9], "charlie");
    TRY(map.default_value("nil"));
    TEST_EQUAL(map[1], "alpha");
    TEST_EQUAL(map[2], "alpha");
    TEST_EQUAL(map[3], "nil");
    TEST_EQUAL(map[4], "bravo");
    TEST_EQUAL(map[5], "bravo");
    TEST_EQUAL(map[6], "bravo");
    TEST_EQUAL(map[7], "nil");
    TEST_EQUAL(map[8], "charlie");
    TEST_EQUAL(map[9], "charlie");
    TEST_EQUAL(map.str(), "{<=2:alpha,[4,6]:bravo,>=8:charlie}");

    TEST(map.contains(1));
    TEST(map.contains(2));
    TEST(! map.contains(3));
    TEST(map.contains(4));
    TEST(map.contains(5));
    TEST(map.contains(6));
    TEST(! map.contains(7));
    TEST(map.contains(8));
    TEST(map.contains(9));

    TRY(it = map.find(1));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "alpha");
    TRY(it = map.find(2));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "alpha");
    TRY(it = map.find(3));  TEST(it == map.end());
    TRY(it = map.find(4));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.find(5));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.find(6));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.find(7));  TEST(it == map.end());
    TRY(it = map.find(8));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.find(9));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");

    TRY(it = map.lower_bound(1));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "alpha");
    TRY(it = map.lower_bound(2));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "alpha");
    TRY(it = map.lower_bound(3));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.lower_bound(4));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.lower_bound(5));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.lower_bound(6));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.lower_bound(7));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.lower_bound(8));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.lower_bound(9));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");

    TRY(it = map.upper_bound(1));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.upper_bound(2));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.upper_bound(3));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.upper_bound(4));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.upper_bound(5));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.upper_bound(6));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.upper_bound(7));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.upper_bound(8));  TEST(it == map.end());
    TRY(it = map.upper_bound(9));  TEST(it == map.end());

    TRY(map.clear());
    TEST(map.empty());
    TRY((map = {
        {IntervalType(20,20,"<="), "alpha"},
        {IntervalType(30,70,"()"), "bravo"},
        {IntervalType(80,80,">="), "charlie"},
    }));
    TEST_EQUAL(map.size(), 3u);
    TEST_EQUAL(map.str(), "{<=20:alpha,[31,69]:bravo,>=80:charlie}");

    TRY(map.clear());
    TRY(map.default_value("nil"));
    TRY(map.insert(IntervalType(10,10,"<="), "alpha"));
    TRY(map.insert(IntervalType(1,5,"[]"), "bravo"));
    TRY(map.insert(IntervalType(5,6,"[]"), "charlie"));
    TRY(map.insert(IntervalType(2,3,"[]"), "delta"));
    TRY(map.insert(IntervalType(7,8,"[]"), "charlie"));
    TEST_EQUAL(map.size(), 6u);
    TEST_EQUAL(map.str(),
        "{<=0:alpha,"
        "1:bravo,"
        "[2,3]:delta,"
        "4:bravo,"
        "[5,8]:charlie,"
        "[9,10]:alpha}");
    TEST_EQUAL(map[0], "alpha");
    TEST_EQUAL(map[1], "bravo");
    TEST_EQUAL(map[2], "delta");
    TEST_EQUAL(map[3], "delta");
    TEST_EQUAL(map[4], "bravo");
    TEST_EQUAL(map[5], "charlie");
    TEST_EQUAL(map[6], "charlie");
    TEST_EQUAL(map[7], "charlie");
    TEST_EQUAL(map[8], "charlie");
    TEST_EQUAL(map[9], "alpha");
    TEST_EQUAL(map[10], "alpha");
    TEST_EQUAL(map[11], "nil");
    TEST_EQUAL(map[12], "nil");

    TRY(map.erase(IntervalType(1,2,"[]")));
    TRY(map.erase(IntervalType(6,7,"[]")));
    TRY(map.erase(IntervalType(10,10,">=")));
    TEST_EQUAL(map.size(), 6u);
    TEST_EQUAL(map.str(),
        "{<=0:alpha,"
        "3:delta,"
        "4:bravo,"
        "5:charlie,"
        "8:charlie,"
        "9:alpha}");
    TEST_EQUAL(map[0], "alpha");
    TEST_EQUAL(map[1], "nil");
    TEST_EQUAL(map[2], "nil");
    TEST_EQUAL(map[3], "delta");
    TEST_EQUAL(map[4], "bravo");
    TEST_EQUAL(map[5], "charlie");
    TEST_EQUAL(map[6], "nil");
    TEST_EQUAL(map[7], "nil");
    TEST_EQUAL(map[8], "charlie");
    TEST_EQUAL(map[9], "alpha");
    TEST_EQUAL(map[10], "nil");
    TEST_EQUAL(map[11], "nil");
    TEST_EQUAL(map[12], "nil");

}
