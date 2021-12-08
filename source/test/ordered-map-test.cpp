#include "rs-interval.hpp"
#include "test/unit-test.hpp"
#include <string>
#include <unordered_set>

using namespace RS::Intervals;

using IntervalType = Interval<std::string>;
using MapType = IntervalMap<std::string, std::string>;
using IB = IntervalBound;
using IC = IntervalCategory;
using IM = IntervalMatch;
using IO = IntervalOrder;

void test_rs_ordered_interval_map() {

    MapType map;
    MapType::iterator it;

    TEST(map.empty());
    TEST_EQUAL(map.size(), 0u);
    TEST_EQUAL(map.default_value(), "");
    TEST_EQUAL(map["hello"], "");
    TEST_EQUAL(map.str(), "{}");

    TRY(map.insert(IntervalType("b","b","<="), "alpha"));
    TRY(map.insert(IntervalType("c","g","()"), "bravo"));
    TRY(map.insert(IntervalType("h","h",">="), "charlie"));
    TEST_EQUAL(map.size(), 3u);
    TEST_EQUAL(map["a"], "alpha");
    TEST_EQUAL(map["b"], "alpha");
    TEST_EQUAL(map["c"], "");
    TEST_EQUAL(map["d"], "bravo");
    TEST_EQUAL(map["e"], "bravo");
    TEST_EQUAL(map["f"], "bravo");
    TEST_EQUAL(map["g"], "");
    TEST_EQUAL(map["h"], "charlie");
    TEST_EQUAL(map["i"], "charlie");
    TRY(map.default_value("nil"));
    TEST_EQUAL(map["a"], "alpha");
    TEST_EQUAL(map["b"], "alpha");
    TEST_EQUAL(map["c"], "nil");
    TEST_EQUAL(map["d"], "bravo");
    TEST_EQUAL(map["e"], "bravo");
    TEST_EQUAL(map["f"], "bravo");
    TEST_EQUAL(map["g"], "nil");
    TEST_EQUAL(map["h"], "charlie");
    TEST_EQUAL(map["i"], "charlie");
    TEST_EQUAL(map.str(), "{<=b:alpha,(c,g):bravo,>=h:charlie}");

    TEST(map.contains("a"));
    TEST(map.contains("b"));
    TEST(! map.contains("c"));
    TEST(map.contains("d"));
    TEST(map.contains("e"));
    TEST(map.contains("f"));
    TEST(! map.contains("g"));
    TEST(map.contains("h"));
    TEST(map.contains("i"));

    TRY(it = map.find("a"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "alpha");
    TRY(it = map.find("b"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "alpha");
    TRY(it = map.find("c"));  TEST(it == map.end());
    TRY(it = map.find("d"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.find("e"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.find("f"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.find("g"));  TEST(it == map.end());
    TRY(it = map.find("h"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.find("i"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");

    TRY(it = map.lower_bound("a"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "alpha");
    TRY(it = map.lower_bound("b"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "alpha");
    TRY(it = map.lower_bound("c"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.lower_bound("d"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.lower_bound("e"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.lower_bound("f"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.lower_bound("g"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.lower_bound("h"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.lower_bound("i"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");

    TRY(it = map.upper_bound("a"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.upper_bound("b"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.upper_bound("c"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "bravo");
    TRY(it = map.upper_bound("d"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.upper_bound("e"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.upper_bound("f"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.upper_bound("g"));  REQUIRE(it != map.end());  TEST_EQUAL(it->second, "charlie");
    TRY(it = map.upper_bound("h"));  TEST(it == map.end());
    TRY(it = map.upper_bound("i"));  TEST(it == map.end());

    TRY(map.clear());
    TEST(map.empty());
    TRY((map = {
        {IntervalType("bb","bb","<="), "alpha"},
        {IntervalType("cc","gg","()"), "bravo"},
        {IntervalType("hh","hh",">="), "charlie"},
    }));
    TEST_EQUAL(map.size(), 3u);
    TEST_EQUAL(map.str(), "{<=bb:alpha,(cc,gg):bravo,>=hh:charlie}");

    TRY(map.clear());
    TRY(map.default_value("nil"));
    TRY(map.insert(IntervalType("j","j","<="), "alpha"));
    TRY(map.insert(IntervalType("a","e","[]"), "bravo"));
    TRY(map.insert(IntervalType("e","f","[]"), "charlie"));
    TRY(map.insert(IntervalType("a","d","()"), "delta"));
    TRY(map.insert(IntervalType("f","h","[]"), "charlie"));
    TEST_EQUAL(map.size(), 6u);
    TEST_EQUAL(map.str(),
        "{<a:alpha,"
        "a:bravo,"
        "(a,d):delta,"
        "[d,e):bravo,"
        "[e,h]:charlie,"
        "(h,j]:alpha}");
    TEST_EQUAL(map["!"], "alpha");
    TEST_EQUAL(map["a"], "bravo");
    TEST_EQUAL(map["b"], "delta");
    TEST_EQUAL(map["c"], "delta");
    TEST_EQUAL(map["d"], "bravo");
    TEST_EQUAL(map["e"], "charlie");
    TEST_EQUAL(map["f"], "charlie");
    TEST_EQUAL(map["g"], "charlie");
    TEST_EQUAL(map["h"], "charlie");
    TEST_EQUAL(map["i"], "alpha");
    TEST_EQUAL(map["j"], "alpha");
    TEST_EQUAL(map["k"], "nil");
    TEST_EQUAL(map["l"], "nil");

    TRY(map.erase(IntervalType("a","b","[]")));
    TRY(map.erase(IntervalType("f","h","()")));
    TRY(map.erase(IntervalType("j","j",">=")));
    TEST_EQUAL(map.size(), 6u);
    TEST_EQUAL(map.str(),
        "{<a:alpha,"
        "(b,d):delta,"
        "[d,e):bravo,"
        "[e,f]:charlie,"
        "h:charlie,"
        "(h,j):alpha}");
    TEST_EQUAL(map["!"], "alpha");
    TEST_EQUAL(map["a"], "nil");
    TEST_EQUAL(map["b"], "nil");
    TEST_EQUAL(map["c"], "delta");
    TEST_EQUAL(map["d"], "bravo");
    TEST_EQUAL(map["e"], "charlie");
    TEST_EQUAL(map["f"], "charlie");
    TEST_EQUAL(map["g"], "nil");
    TEST_EQUAL(map["h"], "charlie");
    TEST_EQUAL(map["i"], "alpha");
    TEST_EQUAL(map["j"], "nil");
    TEST_EQUAL(map["k"], "nil");
    TEST_EQUAL(map["l"], "nil");

}

void test_rs_ordered_interval_map_hashing() {

    std::unordered_set<MapType> map;
    TEST(map.empty());

}
