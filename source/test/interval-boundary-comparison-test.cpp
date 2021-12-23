#include "rs-interval.hpp"
#include "rs-unit-test.hpp"

using namespace RS::Intervals;
using namespace RS::Intervals::Detail;

using B = Boundary<double>;
using BT = BoundaryType;

void test_rs_interval_boundary_formatting() {

    B b;

    TRY((b = {42, BT::null}));            TEST_EQUAL(b.str(), "{}");
    TRY((b = {42, BT::minus_infinity}));  TEST_EQUAL(b.str(), "-inf");
    TRY((b = {42, BT::minus_epsilon}));   TEST_EQUAL(b.str(), "42-eps");
    TRY((b = {42, BT::exact}));           TEST_EQUAL(b.str(), "42");
    TRY((b = {42, BT::plus_epsilon}));    TEST_EQUAL(b.str(), "42+eps");
    TRY((b = {42, BT::plus_infinity}));   TEST_EQUAL(b.str(), "+inf");

}

void test_rs_interval_boundary_adjacency() {

    static const B null(0, BT::null);
    static const B minf(0, BT::minus_infinity);
    static const B pinf(0, BT::plus_infinity);
    static const B me1(1, BT::minus_epsilon);
    static const B ex1(1, BT::exact);
    static const B pe1(1, BT::plus_epsilon);
    static const B me2(2, BT::minus_epsilon);
    static const B ex2(2, BT::exact);
    static const B pe2(2, BT::plus_epsilon);

    TEST(! null.adjacent(null));
    TEST(! null.adjacent(minf));
    TEST(! null.adjacent(pinf));
    TEST(! null.adjacent(me1));
    TEST(! null.adjacent(ex1));
    TEST(! null.adjacent(pe1));
    TEST(! null.adjacent(me2));
    TEST(! null.adjacent(ex2));
    TEST(! null.adjacent(pe2));
    TEST(! minf.adjacent(null));
    TEST(! minf.adjacent(minf));
    TEST(! minf.adjacent(pinf));
    TEST(! minf.adjacent(me1));
    TEST(! minf.adjacent(ex1));
    TEST(! minf.adjacent(pe1));
    TEST(! minf.adjacent(me2));
    TEST(! minf.adjacent(ex2));
    TEST(! minf.adjacent(pe2));
    TEST(! pinf.adjacent(null));
    TEST(! pinf.adjacent(minf));
    TEST(! pinf.adjacent(pinf));
    TEST(! pinf.adjacent(me1));
    TEST(! pinf.adjacent(ex1));
    TEST(! pinf.adjacent(pe1));
    TEST(! pinf.adjacent(me2));
    TEST(! pinf.adjacent(ex2));
    TEST(! pinf.adjacent(pe2));
    TEST(! me1.adjacent(null));
    TEST(! me1.adjacent(minf));
    TEST(! me1.adjacent(pinf));
    TEST(! me1.adjacent(me1));
    TEST(me1.adjacent(ex1));
    TEST(! me1.adjacent(pe1));
    TEST(! me1.adjacent(me2));
    TEST(! me1.adjacent(ex2));
    TEST(! me1.adjacent(pe2));
    TEST(! ex1.adjacent(null));
    TEST(! ex1.adjacent(minf));
    TEST(! ex1.adjacent(pinf));
    TEST(ex1.adjacent(me1));
    TEST(! ex1.adjacent(ex1));
    TEST(ex1.adjacent(pe1));
    TEST(! ex1.adjacent(me2));
    TEST(! ex1.adjacent(ex2));
    TEST(! ex1.adjacent(pe2));
    TEST(! pe1.adjacent(null));
    TEST(! pe1.adjacent(minf));
    TEST(! pe1.adjacent(pinf));
    TEST(! pe1.adjacent(me1));
    TEST(pe1.adjacent(ex1));
    TEST(! pe1.adjacent(pe1));
    TEST(! pe1.adjacent(me2));
    TEST(! pe1.adjacent(ex2));
    TEST(! pe1.adjacent(pe2));
    TEST(! me2.adjacent(null));
    TEST(! me2.adjacent(minf));
    TEST(! me2.adjacent(pinf));
    TEST(! me2.adjacent(me1));
    TEST(! me2.adjacent(ex1));
    TEST(! me2.adjacent(pe1));
    TEST(! me2.adjacent(me2));
    TEST(me2.adjacent(ex2));
    TEST(! me2.adjacent(pe2));
    TEST(! ex2.adjacent(null));
    TEST(! ex2.adjacent(minf));
    TEST(! ex2.adjacent(pinf));
    TEST(! ex2.adjacent(me1));
    TEST(! ex2.adjacent(ex1));
    TEST(! ex2.adjacent(pe1));
    TEST(ex2.adjacent(me2));
    TEST(! ex2.adjacent(ex2));
    TEST(ex2.adjacent(pe2));
    TEST(! pe2.adjacent(null));
    TEST(! pe2.adjacent(minf));
    TEST(! pe2.adjacent(pinf));
    TEST(! pe2.adjacent(me1));
    TEST(! pe2.adjacent(ex1));
    TEST(! pe2.adjacent(pe1));
    TEST(! pe2.adjacent(me2));
    TEST(pe2.adjacent(ex2));
    TEST(! pe2.adjacent(pe2));

}

void test_rs_interval_boundary_comparison() {

    static const B null(0, BT::null);
    static const B minf(0, BT::minus_infinity);
    static const B pinf(0, BT::plus_infinity);
    static const B me1(1, BT::minus_epsilon);
    static const B ex1(1, BT::exact);
    static const B pe1(1, BT::plus_epsilon);
    static const B me2(2, BT::minus_epsilon);
    static const B ex2(2, BT::exact);
    static const B pe2(2, BT::plus_epsilon);

    TEST_EQUAL(null.compare(null), 0);
    TEST_EQUAL(null.compare(minf), -1);
    TEST_EQUAL(null.compare(pinf), -1);
    TEST_EQUAL(null.compare(me1), -1);
    TEST_EQUAL(null.compare(ex1), -1);
    TEST_EQUAL(null.compare(pe1), -1);
    TEST_EQUAL(null.compare(me2), -1);
    TEST_EQUAL(null.compare(ex2), -1);
    TEST_EQUAL(null.compare(pe2), -1);
    TEST_EQUAL(minf.compare(null), 1);
    TEST_EQUAL(minf.compare(minf), 0);
    TEST_EQUAL(minf.compare(pinf), -1);
    TEST_EQUAL(minf.compare(me1), -1);
    TEST_EQUAL(minf.compare(ex1), -1);
    TEST_EQUAL(minf.compare(pe1), -1);
    TEST_EQUAL(minf.compare(me2), -1);
    TEST_EQUAL(minf.compare(ex2), -1);
    TEST_EQUAL(minf.compare(pe2), -1);
    TEST_EQUAL(pinf.compare(null), 1);
    TEST_EQUAL(pinf.compare(minf), 1);
    TEST_EQUAL(pinf.compare(pinf), 0);
    TEST_EQUAL(pinf.compare(me1), 1);
    TEST_EQUAL(pinf.compare(ex1), 1);
    TEST_EQUAL(pinf.compare(pe1), 1);
    TEST_EQUAL(pinf.compare(me2), 1);
    TEST_EQUAL(pinf.compare(ex2), 1);
    TEST_EQUAL(pinf.compare(pe2), 1);
    TEST_EQUAL(me1.compare(null), 1);
    TEST_EQUAL(me1.compare(minf), 1);
    TEST_EQUAL(me1.compare(pinf), -1);
    TEST_EQUAL(me1.compare(me1), 0);
    TEST_EQUAL(me1.compare(ex1), -1);
    TEST_EQUAL(me1.compare(pe1), -1);
    TEST_EQUAL(me1.compare(me2), -1);
    TEST_EQUAL(me1.compare(ex2), -1);
    TEST_EQUAL(me1.compare(pe2), -1);
    TEST_EQUAL(ex1.compare(null), 1);
    TEST_EQUAL(ex1.compare(minf), 1);
    TEST_EQUAL(ex1.compare(pinf), -1);
    TEST_EQUAL(ex1.compare(me1), 1);
    TEST_EQUAL(ex1.compare(ex1), 0);
    TEST_EQUAL(ex1.compare(pe1), -1);
    TEST_EQUAL(ex1.compare(me2), -1);
    TEST_EQUAL(ex1.compare(ex2), -1);
    TEST_EQUAL(ex1.compare(pe2), -1);
    TEST_EQUAL(pe1.compare(null), 1);
    TEST_EQUAL(pe1.compare(minf), 1);
    TEST_EQUAL(pe1.compare(pinf), -1);
    TEST_EQUAL(pe1.compare(me1), 1);
    TEST_EQUAL(pe1.compare(ex1), 1);
    TEST_EQUAL(pe1.compare(pe1), 0);
    TEST_EQUAL(pe1.compare(me2), -1);
    TEST_EQUAL(pe1.compare(ex2), -1);
    TEST_EQUAL(pe1.compare(pe2), -1);
    TEST_EQUAL(me2.compare(null), 1);
    TEST_EQUAL(me2.compare(minf), 1);
    TEST_EQUAL(me2.compare(pinf), -1);
    TEST_EQUAL(me2.compare(me1), 1);
    TEST_EQUAL(me2.compare(ex1), 1);
    TEST_EQUAL(me2.compare(pe1), 1);
    TEST_EQUAL(me2.compare(me2), 0);
    TEST_EQUAL(me2.compare(ex2), -1);
    TEST_EQUAL(me2.compare(pe2), -1);
    TEST_EQUAL(ex2.compare(null), 1);
    TEST_EQUAL(ex2.compare(minf), 1);
    TEST_EQUAL(ex2.compare(pinf), -1);
    TEST_EQUAL(ex2.compare(me1), 1);
    TEST_EQUAL(ex2.compare(ex1), 1);
    TEST_EQUAL(ex2.compare(pe1), 1);
    TEST_EQUAL(ex2.compare(me2), 1);
    TEST_EQUAL(ex2.compare(ex2), 0);
    TEST_EQUAL(ex2.compare(pe2), -1);
    TEST_EQUAL(pe2.compare(null), 1);
    TEST_EQUAL(pe2.compare(minf), 1);
    TEST_EQUAL(pe2.compare(pinf), -1);
    TEST_EQUAL(pe2.compare(me1), 1);
    TEST_EQUAL(pe2.compare(ex1), 1);
    TEST_EQUAL(pe2.compare(pe1), 1);
    TEST_EQUAL(pe2.compare(me2), 1);
    TEST_EQUAL(pe2.compare(ex2), 1);
    TEST_EQUAL(pe2.compare(pe2), 0);

}
