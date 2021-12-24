#include "rs-interval.hpp"
#include "rs-unit-test.hpp"

using namespace RS::Intervals;
using namespace RS::Intervals::Detail;

using Itv = Interval<std::string>;
using IB = IntervalBound;
using B = Boundary<std::string>;
using BT = BoundaryType;

void test_rs_ordered_boundary_formatting() {

    B b;

    TRY((b = {"abc", BT::empty}));           TEST_EQUAL(b.str(), "{}");
    TRY((b = {"abc", BT::minus_infinity}));  TEST_EQUAL(b.str(), "-inf");
    TRY((b = {"abc", BT::plus_infinity}));   TEST_EQUAL(b.str(), "+inf");
    TRY((b = {"abc", BT::closed}));          TEST_EQUAL(b.str(), "abc");
    TRY((b = {"abc", BT::open}));            TEST_EQUAL(b.str(), "(abc)");

}

void test_rs_ordered_boundary_from_interval() {

    static const Itv none;
    static const Itv all = Itv::all();
    static const Itv eq1("a");
    static const Itv ge1("a", "a", IB::closed, IB::unbound);
    static const Itv gt1("a", "a", IB::open, IB::unbound);
    static const Itv le1("a", "a", IB::unbound, IB::closed);
    static const Itv lt1("a", "a", IB::unbound, IB::open);
    static const Itv c12("a", "b", IB::closed, IB::closed);
    static const Itv o12("a", "b", IB::open, IB::open);
    static const Itv o1c2("a", "b", IB::open, IB::closed);
    static const Itv c1o2("a", "b", IB::closed, IB::open);

    TEST_EQUAL(none.str(), "{}");
    TEST_EQUAL(all.str(), "*");
    TEST_EQUAL(eq1.str(), "a");
    TEST_EQUAL(ge1.str(), ">=a");
    TEST_EQUAL(gt1.str(), ">a");
    TEST_EQUAL(le1.str(), "<=a");
    TEST_EQUAL(lt1.str(), "<a");
    TEST_EQUAL(c12.str(), "[a,b]");
    TEST_EQUAL(o12.str(), "(a,b)");
    TEST_EQUAL(o1c2.str(), "(a,b]");
    TEST_EQUAL(c1o2.str(), "[a,b)");

    TEST_EQUAL(left_boundary_of(none), (B{"", BT::empty}));
    TEST_EQUAL(left_boundary_of(all), (B{"", BT::minus_infinity}));
    TEST_EQUAL(left_boundary_of(eq1), (B{"a", BT::closed}));
    TEST_EQUAL(left_boundary_of(ge1), (B{"a", BT::closed}));
    TEST_EQUAL(left_boundary_of(gt1), (B{"a", BT::open}));
    TEST_EQUAL(left_boundary_of(le1), (B{"", BT::minus_infinity}));
    TEST_EQUAL(left_boundary_of(lt1), (B{"", BT::minus_infinity}));
    TEST_EQUAL(left_boundary_of(c12), (B{"a", BT::closed}));
    TEST_EQUAL(left_boundary_of(o12), (B{"a", BT::open}));
    TEST_EQUAL(left_boundary_of(o1c2), (B{"a", BT::open}));
    TEST_EQUAL(left_boundary_of(c1o2), (B{"a", BT::closed}));

    TEST_EQUAL(right_boundary_of(none), (B{"", BT::empty}));
    TEST_EQUAL(right_boundary_of(all), (B{"", BT::plus_infinity}));
    TEST_EQUAL(right_boundary_of(eq1), (B{"a", BT::closed}));
    TEST_EQUAL(right_boundary_of(ge1), (B{"", BT::plus_infinity}));
    TEST_EQUAL(right_boundary_of(gt1), (B{"", BT::plus_infinity}));
    TEST_EQUAL(right_boundary_of(le1), (B{"a", BT::closed}));
    TEST_EQUAL(right_boundary_of(lt1), (B{"a", BT::open}));
    TEST_EQUAL(right_boundary_of(c12), (B{"b", BT::closed}));
    TEST_EQUAL(right_boundary_of(o12), (B{"b", BT::open}));
    TEST_EQUAL(right_boundary_of(o1c2), (B{"b", BT::closed}));
    TEST_EQUAL(right_boundary_of(c1o2), (B{"b", BT::open}));

}

void test_rs_ordered_boundary_to_interval() {

    static const B none = {"", BT::empty};
    static const B ninf = {"", BT::minus_infinity};
    static const B pinf = {"", BT::plus_infinity};
    static const B cl1 = {"a", BT::closed};
    static const B op1 = {"a", BT::open};
    static const B cl2 = {"b", BT::closed};
    static const B op2 = {"b", BT::open};

    TEST_EQUAL(interval_from_boundaries(none, none), Itv());
    TEST_EQUAL(interval_from_boundaries(ninf, pinf), Itv::all());
    TEST_EQUAL(interval_from_boundaries(ninf, cl1), Itv("a", "a", "<="));
    TEST_EQUAL(interval_from_boundaries(ninf, op1), Itv("a", "a", "<"));
    TEST_EQUAL(interval_from_boundaries(cl1, pinf), Itv("a", "a", ">="));
    TEST_EQUAL(interval_from_boundaries(op1, pinf), Itv("a", "a", ">"));
    TEST_EQUAL(interval_from_boundaries(cl1, cl1), Itv("a"));
    TEST_EQUAL(interval_from_boundaries(cl1, cl2), Itv("a", "b", "[]"));
    TEST_EQUAL(interval_from_boundaries(cl1, op2), Itv("a", "b", "[)"));
    TEST_EQUAL(interval_from_boundaries(op1, cl2), Itv("a", "b", "(]"));
    TEST_EQUAL(interval_from_boundaries(op1, op2), Itv("a", "b", "()"));

}
