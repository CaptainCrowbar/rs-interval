#include "rs-interval/interval.hpp"
#include "rs-interval/types.hpp"
#include "test/stepwise-example.hpp"
#include "test/unit-test.hpp"
#include <format>

using namespace RS::Interval;
using namespace RS::Interval::Detail;

using Itv = Interval<StepwiseType>;
using B = Boundary<StepwiseType>;
using BT = BoundaryType;

void test_rs_interval_stepwise_boundary_formatting() {

    B b;

    TRY((b = {42, BT::empty}));           TEST_EQUAL(std::format("{}", b), "{}");
    TRY((b = {42, BT::minus_infinity}));  TEST_EQUAL(std::format("{}", b), "-inf");
    TRY((b = {42, BT::plus_infinity}));   TEST_EQUAL(std::format("{}", b), "+inf");
    TRY((b = {42, BT::closed}));          TEST_EQUAL(std::format("{}", b), "42");

}

void test_rs_interval_stepwise_boundary_from_interval() {

    static const Itv none;
    static const Itv all = Itv::all();
    static const Itv eq1(1);
    static const Itv ge1(1, 1, Bound::closed, Bound::unbound);
    static const Itv le1(1, 1, Bound::unbound, Bound::closed);
    static const Itv c12(1, 2, Bound::closed, Bound::closed);

    TEST_EQUAL(std::format("{}", none), "{}");
    TEST_EQUAL(std::format("{}", all), "*");
    TEST_EQUAL(std::format("{}", eq1), "1");
    TEST_EQUAL(std::format("{}", ge1), ">=1");
    TEST_EQUAL(std::format("{}", le1), "<=1");
    TEST_EQUAL(std::format("{}", c12), "[1,2]");

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
