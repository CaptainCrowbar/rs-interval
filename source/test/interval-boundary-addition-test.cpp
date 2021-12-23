#include "rs-interval.hpp"
#include "rs-unit-test.hpp"

using namespace RS::Intervals;
using namespace RS::Intervals::Detail;

using B = Boundary<double>;
using BT = BoundaryType;

void test_rs_interval_boundary_inversion() {

    static const B null(0, BT::null);
    static const B minf(0, BT::minus_infinity);
    static const B pinf(0, BT::plus_infinity);
    static const B me2(2, BT::minus_epsilon);
    static const B ex2(2, BT::exact);
    static const B pe2(2, BT::plus_epsilon);
    static const B me_2(-2, BT::minus_epsilon);
    static const B ex_2(-2, BT::exact);
    static const B pe_2(-2, BT::plus_epsilon);

    TEST_EQUAL(- null, null);
    TEST_EQUAL(- minf, pinf);
    TEST_EQUAL(- pinf, minf);
    TEST_EQUAL(- me2, pe_2);
    TEST_EQUAL(- ex2, ex_2);
    TEST_EQUAL(- pe2, me_2);
    TEST_EQUAL(- me_2, pe2);
    TEST_EQUAL(- ex_2, ex2);
    TEST_EQUAL(- pe_2, me2);

}

void test_rs_interval_boundary_addition() {

    static const B null(0, BT::null);
    static const B minf(0, BT::minus_infinity);
    static const B pinf(0, BT::plus_infinity);
    static const B me1(1, BT::minus_epsilon);
    static const B ex1(1, BT::exact);
    static const B pe1(1, BT::plus_epsilon);
    static const B me2(2, BT::minus_epsilon);
    static const B ex2(2, BT::exact);
    static const B pe2(2, BT::plus_epsilon);
    static const B me3(3, BT::minus_epsilon);
    static const B ex3(3, BT::exact);
    static const B pe3(3, BT::plus_epsilon);

    TEST_EQUAL(null + null, null);
    TEST_EQUAL(null + minf, null);
    TEST_EQUAL(null + pinf, null);
    TEST_EQUAL(null + me2, null);
    TEST_EQUAL(null + ex2, null);
    TEST_EQUAL(null + pe2, null);
    TEST_EQUAL(minf + null, null);
    TEST_EQUAL(minf + minf, minf);
    TEST_EQUAL(minf + ex2, minf);
    TEST_EQUAL(minf + pe2, minf);
    TEST_EQUAL(pinf + null, null);
    TEST_EQUAL(pinf + pinf, pinf);
    TEST_EQUAL(pinf + me2, pinf);
    TEST_EQUAL(pinf + ex2, pinf);
    TEST_EQUAL(me1 + null, null);
    TEST_EQUAL(me1 + pinf, pinf);
    TEST_EQUAL(me1 + me2, me3);
    TEST_EQUAL(me1 + ex2, me3);
    TEST_EQUAL(ex1 + null, null);
    TEST_EQUAL(ex1 + minf, minf);
    TEST_EQUAL(ex1 + pinf, pinf);
    TEST_EQUAL(ex1 + me2, me3);
    TEST_EQUAL(ex1 + ex2, ex3);
    TEST_EQUAL(ex1 + pe2, pe3);
    TEST_EQUAL(pe1 + null, null);
    TEST_EQUAL(pe1 + minf, minf);
    TEST_EQUAL(pe1 + ex2, pe3);
    TEST_EQUAL(pe1 + pe2, pe3);

}

void test_rs_interval_boundary_subtraction() {

    static const B null(0, BT::null);
    static const B minf(0, BT::minus_infinity);
    static const B pinf(0, BT::plus_infinity);
    static const B me1(1, BT::minus_epsilon);
    static const B ex1(1, BT::exact);
    static const B pe1(1, BT::plus_epsilon);
    static const B me2(2, BT::minus_epsilon);
    static const B ex2(2, BT::exact);
    static const B pe2(2, BT::plus_epsilon);
    static const B me3(3, BT::minus_epsilon);
    static const B ex3(3, BT::exact);
    static const B pe3(3, BT::plus_epsilon);

    TEST_EQUAL(null - null, null);
    TEST_EQUAL(null - minf, null);
    TEST_EQUAL(null - pinf, null);
    TEST_EQUAL(null - me2, null);
    TEST_EQUAL(null - ex2, null);
    TEST_EQUAL(null - pe2, null);
    TEST_EQUAL(minf - null, null);
    TEST_EQUAL(minf - pinf, minf);
    TEST_EQUAL(minf - me2, minf);
    TEST_EQUAL(minf - ex2, minf);
    TEST_EQUAL(pinf - null, null);
    TEST_EQUAL(pinf - minf, pinf);
    TEST_EQUAL(pinf - ex2, pinf);
    TEST_EQUAL(pinf - pe2, pinf);
    TEST_EQUAL(me3 - null, null);
    TEST_EQUAL(me3 - minf, pinf);
    TEST_EQUAL(me3 - ex2, me1);
    TEST_EQUAL(me3 - pe2, me1);
    TEST_EQUAL(ex3 - null, null);
    TEST_EQUAL(ex3 - minf, pinf);
    TEST_EQUAL(ex3 - pinf, minf);
    TEST_EQUAL(ex3 - me2, pe1);
    TEST_EQUAL(ex3 - ex2, ex1);
    TEST_EQUAL(ex3 - pe2, me1);
    TEST_EQUAL(pe3 - null, null);
    TEST_EQUAL(pe3 - pinf, minf);
    TEST_EQUAL(pe3 - me2, pe1);
    TEST_EQUAL(pe3 - ex2, pe1);

}
