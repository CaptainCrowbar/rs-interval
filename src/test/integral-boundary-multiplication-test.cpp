#include "rs-interval/types.hpp"
#include "test/unit-test.hpp"

using namespace RS::Interval;
using namespace RS::Interval::Detail;

using B = Boundary<int>;
using BT = BoundaryType;

void test_rs_interval_integral_boundary_multiplication() {

    static const B none = {0, BT::empty};
    static const B ninf = {0, BT::minus_infinity};
    static const B pinf = {0, BT::plus_infinity};
    static const B cl0 = {0, BT::closed};
    static const B cl2 = {2, BT::closed};
    static const B cl3 = {3, BT::closed};
    static const B cl6 = {6, BT::closed};
    static const B cl_2 = {-2, BT::closed};
    static const B cl_3 = {-3, BT::closed};
    static const B cl_6 = {-6, BT::closed};

    TEST_EQUAL(none * none, none);
    TEST_EQUAL(none * ninf, none);
    TEST_EQUAL(none * pinf, none);
    TEST_EQUAL(none * cl_3, none);
    TEST_EQUAL(none * cl3, none);
    TEST_EQUAL(none * cl0, none);
    TEST_EQUAL(ninf * none, none);
    TEST_EQUAL(ninf * ninf, pinf);
    TEST_EQUAL(ninf * pinf, ninf);
    TEST_EQUAL(ninf * cl_3, pinf);
    TEST_EQUAL(ninf * cl3, ninf);
    TEST_EQUAL(ninf * cl0, cl0);
    TEST_EQUAL(pinf * none, none);
    TEST_EQUAL(pinf * ninf, ninf);
    TEST_EQUAL(pinf * pinf, pinf);
    TEST_EQUAL(pinf * cl_3, ninf);
    TEST_EQUAL(pinf * cl3, pinf);
    TEST_EQUAL(pinf * cl0, cl0);
    TEST_EQUAL(cl_2 * none, none);
    TEST_EQUAL(cl_2 * ninf, pinf);
    TEST_EQUAL(cl_2 * pinf, ninf);
    TEST_EQUAL(cl_2 * cl_3, cl6);
    TEST_EQUAL(cl_2 * cl3, cl_6);
    TEST_EQUAL(cl_2 * cl0, cl0);
    TEST_EQUAL(cl2 * none, none);
    TEST_EQUAL(cl2 * ninf, ninf);
    TEST_EQUAL(cl2 * pinf, pinf);
    TEST_EQUAL(cl2 * cl_3, cl_6);
    TEST_EQUAL(cl2 * cl3, cl6);
    TEST_EQUAL(cl2 * cl0, cl0);
    TEST_EQUAL(cl0 * none, none);
    TEST_EQUAL(cl0 * ninf, cl0);
    TEST_EQUAL(cl0 * pinf, cl0);
    TEST_EQUAL(cl0 * cl_3, cl0);
    TEST_EQUAL(cl0 * cl3, cl0);
    TEST_EQUAL(cl0 * cl0, cl0);

}
