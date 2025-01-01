#include "rs-interval/interval.hpp"
#include "rs-interval/types.hpp"
#include "test/unit-test.hpp"
#include <format>
#include <string>

using namespace RS::Interval;
using namespace RS::Interval::Detail;

using Itv = Interval<std::string>;
using B = Boundary<std::string>;
using BT = BoundaryType;

void test_rs_interval_ordered_boundary_formatting() {

    B b;

    TRY((b = {"abc", BT::empty}));           TEST_EQUAL(std::format("{}", b), "{}");
    TRY((b = {"abc", BT::minus_infinity}));  TEST_EQUAL(std::format("{}", b), "-inf");
    TRY((b = {"abc", BT::plus_infinity}));   TEST_EQUAL(std::format("{}", b), "+inf");
    TRY((b = {"abc", BT::closed}));          TEST_EQUAL(std::format("{}", b), "abc");
    TRY((b = {"abc", BT::open}));            TEST_EQUAL(std::format("{}", b), "(abc)");

}

void test_rs_interval_ordered_boundary_from_interval() {

    static const Itv none;
    static const Itv all = Itv::all();
    static const Itv eq1("a");
    static const Itv ge1("a", "a", Bound::closed, Bound::unbound);
    static const Itv gt1("a", "a", Bound::open, Bound::unbound);
    static const Itv le1("a", "a", Bound::unbound, Bound::closed);
    static const Itv lt1("a", "a", Bound::unbound, Bound::open);
    static const Itv c12("a", "b", Bound::closed, Bound::closed);
    static const Itv o12("a", "b", Bound::open, Bound::open);
    static const Itv o1c2("a", "b", Bound::open, Bound::closed);
    static const Itv c1o2("a", "b", Bound::closed, Bound::open);

    TEST_EQUAL(std::format("{}", none), "{}");
    TEST_EQUAL(std::format("{}", all), "*");
    TEST_EQUAL(std::format("{}", eq1), "a");
    TEST_EQUAL(std::format("{}", ge1), ">=a");
    TEST_EQUAL(std::format("{}", gt1), ">a");
    TEST_EQUAL(std::format("{}", le1), "<=a");
    TEST_EQUAL(std::format("{}", lt1), "<a");
    TEST_EQUAL(std::format("{}", c12), "[a,b]");
    TEST_EQUAL(std::format("{}", o12), "(a,b)");
    TEST_EQUAL(std::format("{}", o1c2), "(a,b]");
    TEST_EQUAL(std::format("{}", c1o2), "[a,b)");

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
