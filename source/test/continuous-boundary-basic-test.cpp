#include "rs-intervals/interval.hpp"
#include "rs-intervals/types.hpp"
#include "test/unit-test.hpp"

using namespace RS::Intervals;
using namespace RS::Intervals::Detail;

using Itv = Interval<double>;
using B = Boundary<double>;
using BT = BoundaryType;

void test_rs_interval_continuous_boundary_formatting() {

    B b;

    TRY((b = {42, BT::empty}));           TEST_EQUAL(b.str(), "{}");
    TRY((b = {42, BT::minus_infinity}));  TEST_EQUAL(b.str(), "-inf");
    TRY((b = {42, BT::plus_infinity}));   TEST_EQUAL(b.str(), "+inf");
    TRY((b = {42, BT::closed}));          TEST_EQUAL(b.str(), "42");
    TRY((b = {42, BT::open}));            TEST_EQUAL(b.str(), "(42)");

}

void test_rs_interval_continuous_boundary_from_interval() {

    static const Itv none;
    static const Itv all = Itv::all();
    static const Itv eq1(1);
    static const Itv ge1(1, 1, Bound::closed, Bound::unbound);
    static const Itv gt1(1, 1, Bound::open, Bound::unbound);
    static const Itv le1(1, 1, Bound::unbound, Bound::closed);
    static const Itv lt1(1, 1, Bound::unbound, Bound::open);
    static const Itv c12(1, 2, Bound::closed, Bound::closed);
    static const Itv o12(1, 2, Bound::open, Bound::open);
    static const Itv o1c2(1, 2, Bound::open, Bound::closed);
    static const Itv c1o2(1, 2, Bound::closed, Bound::open);

    TEST_EQUAL(none.str(), "{}");
    TEST_EQUAL(all.str(), "*");
    TEST_EQUAL(eq1.str(), "1");
    TEST_EQUAL(ge1.str(), ">=1");
    TEST_EQUAL(gt1.str(), ">1");
    TEST_EQUAL(le1.str(), "<=1");
    TEST_EQUAL(lt1.str(), "<1");
    TEST_EQUAL(c12.str(), "[1,2]");
    TEST_EQUAL(o12.str(), "(1,2)");
    TEST_EQUAL(o1c2.str(), "(1,2]");
    TEST_EQUAL(c1o2.str(), "[1,2)");

    TEST_EQUAL(left_boundary_of(none), (B{0, BT::empty}));
    TEST_EQUAL(left_boundary_of(all), (B{0, BT::minus_infinity}));
    TEST_EQUAL(left_boundary_of(eq1), (B{1, BT::closed}));
    TEST_EQUAL(left_boundary_of(ge1), (B{1, BT::closed}));
    TEST_EQUAL(left_boundary_of(gt1), (B{1, BT::open}));
    TEST_EQUAL(left_boundary_of(le1), (B{0, BT::minus_infinity}));
    TEST_EQUAL(left_boundary_of(lt1), (B{0, BT::minus_infinity}));
    TEST_EQUAL(left_boundary_of(c12), (B{1, BT::closed}));
    TEST_EQUAL(left_boundary_of(o12), (B{1, BT::open}));
    TEST_EQUAL(left_boundary_of(o1c2), (B{1, BT::open}));
    TEST_EQUAL(left_boundary_of(c1o2), (B{1, BT::closed}));

    TEST_EQUAL(right_boundary_of(none), (B{0, BT::empty}));
    TEST_EQUAL(right_boundary_of(all), (B{0, BT::plus_infinity}));
    TEST_EQUAL(right_boundary_of(eq1), (B{1, BT::closed}));
    TEST_EQUAL(right_boundary_of(ge1), (B{0, BT::plus_infinity}));
    TEST_EQUAL(right_boundary_of(gt1), (B{0, BT::plus_infinity}));
    TEST_EQUAL(right_boundary_of(le1), (B{1, BT::closed}));
    TEST_EQUAL(right_boundary_of(lt1), (B{1, BT::open}));
    TEST_EQUAL(right_boundary_of(c12), (B{2, BT::closed}));
    TEST_EQUAL(right_boundary_of(o12), (B{2, BT::open}));
    TEST_EQUAL(right_boundary_of(o1c2), (B{2, BT::closed}));
    TEST_EQUAL(right_boundary_of(c1o2), (B{2, BT::open}));

}

void test_rs_interval_continuous_boundary_to_interval() {

    static const B none = {0, BT::empty};
    static const B ninf = {0, BT::minus_infinity};
    static const B pinf = {0, BT::plus_infinity};
    static const B cl1 = {1, BT::closed};
    static const B op1 = {1, BT::open};
    static const B cl2 = {2, BT::closed};
    static const B op2 = {2, BT::open};

    TEST_EQUAL(interval_from_boundaries(none, none), Itv());
    TEST_EQUAL(interval_from_boundaries(ninf, pinf), Itv::all());
    TEST_EQUAL(interval_from_boundaries(ninf, cl1), Itv(1, 1, "<="));
    TEST_EQUAL(interval_from_boundaries(ninf, op1), Itv(1, 1, "<"));
    TEST_EQUAL(interval_from_boundaries(cl1, pinf), Itv(1, 1, ">="));
    TEST_EQUAL(interval_from_boundaries(op1, pinf), Itv(1, 1, ">"));
    TEST_EQUAL(interval_from_boundaries(cl1, cl1), Itv(1));
    TEST_EQUAL(interval_from_boundaries(cl1, cl2), Itv(1, 2, "[]"));
    TEST_EQUAL(interval_from_boundaries(cl1, op2), Itv(1, 2, "[)"));
    TEST_EQUAL(interval_from_boundaries(op1, cl2), Itv(1, 2, "(]"));
    TEST_EQUAL(interval_from_boundaries(op1, op2), Itv(1, 2, "()"));

}
