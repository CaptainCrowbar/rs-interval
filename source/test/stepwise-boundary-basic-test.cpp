#include "rs-intervals/interval.hpp"
#include "rs-intervals/types.hpp"
#include "test/stepwise-example.hpp"
#include "test/unit-test.hpp"

using namespace RS::Intervals;
using namespace RS::Intervals::Detail;

using Itv = Interval<StepwiseType>;
using B = Boundary<StepwiseType>;
using BT = BoundaryType;

void test_rs_interval_stepwise_boundary_formatting() {

    B b;

    TRY((b = {42, BT::empty}));           TEST_EQUAL(b.str(), "{}");
    TRY((b = {42, BT::minus_infinity}));  TEST_EQUAL(b.str(), "-inf");
    TRY((b = {42, BT::plus_infinity}));   TEST_EQUAL(b.str(), "+inf");
    TRY((b = {42, BT::closed}));          TEST_EQUAL(b.str(), "42");

}

void test_rs_interval_stepwise_boundary_from_interval() {

    static const Itv none;
    static const Itv all = Itv::all();
    static const Itv eq1(1);
    static const Itv ge1(1, 1, Bound::closed, Bound::unbound);
    static const Itv le1(1, 1, Bound::unbound, Bound::closed);
    static const Itv c12(1, 2, Bound::closed, Bound::closed);

    TEST_EQUAL(none.str(), "{}");
    TEST_EQUAL(all.str(), "*");
    TEST_EQUAL(eq1.str(), "1");
    TEST_EQUAL(ge1.str(), ">=1");
    TEST_EQUAL(le1.str(), "<=1");
    TEST_EQUAL(c12.str(), "[1,2]");

    TEST_EQUAL(left_boundary_of(none), (B{0, BT::empty}));
    TEST_EQUAL(left_boundary_of(all), (B{0, BT::minus_infinity}));
    TEST_EQUAL(left_boundary_of(eq1), (B{1, BT::closed}));
    TEST_EQUAL(left_boundary_of(ge1), (B{1, BT::closed}));
    TEST_EQUAL(left_boundary_of(le1), (B{0, BT::minus_infinity}));
    TEST_EQUAL(left_boundary_of(c12), (B{1, BT::closed}));

    TEST_EQUAL(right_boundary_of(none), (B{0, BT::empty}));
    TEST_EQUAL(right_boundary_of(all), (B{0, BT::plus_infinity}));
    TEST_EQUAL(right_boundary_of(eq1), (B{1, BT::closed}));
    TEST_EQUAL(right_boundary_of(ge1), (B{0, BT::plus_infinity}));
    TEST_EQUAL(right_boundary_of(le1), (B{1, BT::closed}));
    TEST_EQUAL(right_boundary_of(c12), (B{2, BT::closed}));

}
