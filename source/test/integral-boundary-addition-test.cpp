#include "rs-intervals/types.hpp"
#include "test/unit-test.hpp"

using namespace RS::Intervals;
using namespace RS::Intervals::Detail;

using B = Boundary<int>;
using BT = BoundaryType;

void test_rs_interval_integral_boundary_inversion() {

    static const B none = {0, BT::empty};
    static const B ninf = {0, BT::minus_infinity};
    static const B pinf = {0, BT::plus_infinity};
    static const B cl2 = {2, BT::closed};
    static const B op2 = {2, BT::open};
    static const B cl_2 = {-2, BT::closed};
    static const B op_2 = {-2, BT::open};

    TEST_EQUAL(- none, none);
    TEST_EQUAL(- ninf, pinf);
    TEST_EQUAL(- pinf, ninf);
    TEST_EQUAL(- cl2, cl_2);
    TEST_EQUAL(- op2, op_2);
    TEST_EQUAL(- cl_2, cl2);
    TEST_EQUAL(- op_2, op2);

}

void test_rs_interval_integral_boundary_addition() {

    static const B none = {0, BT::empty};
    static const B ninf = {0, BT::minus_infinity};
    static const B pinf = {0, BT::plus_infinity};
    static const B cl1 = {1, BT::closed};
    static const B op1 = {1, BT::open};
    static const B cl2 = {2, BT::closed};
    static const B op2 = {2, BT::open};
    static const B cl3 = {3, BT::closed};
    static const B op3 = {3, BT::open};

    TEST_EQUAL(none + none, none);
    TEST_EQUAL(none + ninf, none);
    TEST_EQUAL(none + pinf, none);
    TEST_EQUAL(none + cl2, none);
    TEST_EQUAL(none + op2, none);
    TEST_EQUAL(ninf + none, none);
    TEST_EQUAL(ninf + ninf, ninf);
    TEST_EQUAL(ninf + cl2, ninf);
    TEST_EQUAL(ninf + op2, ninf);
    TEST_EQUAL(pinf + none, none);
    TEST_EQUAL(pinf + pinf, pinf);
    TEST_EQUAL(pinf + cl2, pinf);
    TEST_EQUAL(cl1 + none, none);
    TEST_EQUAL(cl1 + ninf, ninf);
    TEST_EQUAL(cl1 + pinf, pinf);
    TEST_EQUAL(cl1 + cl2, cl3);
    TEST_EQUAL(cl1 + op2, op3);
    TEST_EQUAL(op1 + none, none);
    TEST_EQUAL(op1 + ninf, ninf);
    TEST_EQUAL(op1 + cl2, op3);
    TEST_EQUAL(op1 + op2, op3);

}

void test_rs_interval_integral_boundary_subtraction() {

    static const B none = {0, BT::empty};
    static const B ninf = {0, BT::minus_infinity};
    static const B pinf = {0, BT::plus_infinity};
    static const B cl1 = {1, BT::closed};
    static const B op1 = {1, BT::open};
    static const B cl2 = {2, BT::closed};
    static const B op2 = {2, BT::open};
    static const B cl3 = {3, BT::closed};
    static const B op3 = {3, BT::open};

    TEST_EQUAL(none - none, none);
    TEST_EQUAL(none - ninf, none);
    TEST_EQUAL(none - pinf, none);
    TEST_EQUAL(none - cl2, none);
    TEST_EQUAL(none - op2, none);
    TEST_EQUAL(ninf - none, none);
    TEST_EQUAL(ninf - pinf, ninf);
    TEST_EQUAL(ninf - cl2, ninf);
    TEST_EQUAL(pinf - none, none);
    TEST_EQUAL(pinf - ninf, pinf);
    TEST_EQUAL(pinf - cl2, pinf);
    TEST_EQUAL(pinf - op2, pinf);
    TEST_EQUAL(cl3 - none, none);
    TEST_EQUAL(cl3 - ninf, pinf);
    TEST_EQUAL(cl3 - pinf, ninf);
    TEST_EQUAL(cl3 - cl2, cl1);
    TEST_EQUAL(cl3 - op2, op1);
    TEST_EQUAL(op3 - none, none);
    TEST_EQUAL(op3 - pinf, ninf);
    TEST_EQUAL(op3 - cl2, op1);

}
