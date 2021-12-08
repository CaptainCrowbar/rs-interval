#include "rs-interval.hpp"
#include "test/unit-test.hpp"
#include <string>

using namespace RS::Intervals;

using IntervalType = Interval<int>;
using SetType = IntervalSet<int>;
using IB = IntervalBound;
using IC = IntervalCategory;
using IM = IntervalMatch;
using IO = IntervalOrder;

void test_rs_integral_interval_arithmetic() {

    IntervalType in;
    std::string str;

    TRY(in = + IntervalType());          TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = + IntervalType::all());     TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = + IntervalType(1));         TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = - IntervalType());          TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = - IntervalType::all());     TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = - IntervalType(1));         TRY(str = in.str());  TEST_EQUAL(str, "-1");
    TRY(in = - IntervalType(1,5,"[]"));  TRY(str = in.str());  TEST_EQUAL(str, "[-5,-1]");
    TRY(in = - IntervalType(1,5,"()"));  TRY(str = in.str());  TEST_EQUAL(str, "[-4,-2]");
    TRY(in = - IntervalType(1,5,"[)"));  TRY(str = in.str());  TEST_EQUAL(str, "[-4,-1]");
    TRY(in = - IntervalType(1,5,"(]"));  TRY(str = in.str());  TEST_EQUAL(str, "[-5,-2]");
    TRY(in = - IntervalType(0,5,"<"));   TRY(str = in.str());  TEST_EQUAL(str, ">=-4");
    TRY(in = - IntervalType(0,5,"<="));  TRY(str = in.str());  TEST_EQUAL(str, ">=-5");
    TRY(in = - IntervalType(5,0,">"));   TRY(str = in.str());  TEST_EQUAL(str, "<=-6");
    TRY(in = - IntervalType(5,0,">="));  TRY(str = in.str());  TEST_EQUAL(str, "<=-5");

    // TODO

}
