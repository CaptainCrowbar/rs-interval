#include "rs-interval.hpp"
#include "rs-unit-test.hpp"
#include <string>

using namespace RS::Intervals;

using Itv = Interval<int>;
using Set = IntervalSet<int>;
using IB = IntervalBound;
using IC = IntervalCategory;
using IM = IntervalMatch;
using IO = IntervalOrder;

void test_rs_integral_interval_arithmetic() {

    Itv in;
    std::string str;

    TRY(in = + Itv());          TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = + Itv::all());     TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = + Itv(1));         TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = - Itv());          TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = - Itv::all());     TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = - Itv(1));         TRY(str = in.str());  TEST_EQUAL(str, "-1");
    TRY(in = - Itv(1,5,"[]"));  TRY(str = in.str());  TEST_EQUAL(str, "[-5,-1]");
    TRY(in = - Itv(1,5,"()"));  TRY(str = in.str());  TEST_EQUAL(str, "[-4,-2]");
    TRY(in = - Itv(1,5,"[)"));  TRY(str = in.str());  TEST_EQUAL(str, "[-4,-1]");
    TRY(in = - Itv(1,5,"(]"));  TRY(str = in.str());  TEST_EQUAL(str, "[-5,-2]");
    TRY(in = - Itv(0,5,"<"));   TRY(str = in.str());  TEST_EQUAL(str, ">=-4");
    TRY(in = - Itv(0,5,"<="));  TRY(str = in.str());  TEST_EQUAL(str, ">=-5");
    TRY(in = - Itv(5,0,">"));   TRY(str = in.str());  TEST_EQUAL(str, "<=-6");
    TRY(in = - Itv(5,0,">="));  TRY(str = in.str());  TEST_EQUAL(str, "<=-5");

    // TODO

}
