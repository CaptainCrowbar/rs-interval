#include "rs-interval/types.hpp"
#include "test/unit-test.hpp"

using namespace RS::Interval;
using namespace RS::Interval::Detail;

using B = Boundary<int>;
using BT = BoundaryType;

void test_rs_interval_integral_boundary_adjacency() {

    static const B none = {0, BT::empty};
    static const B ninf = {0, BT::minus_infinity};
    static const B pinf = {0, BT::plus_infinity};
    static const B cl1 = {1, BT::closed};
    static const B cl2 = {2, BT::closed};

    TEST(! none.adjacent(none));
    TEST(! none.adjacent(ninf));
    TEST(! none.adjacent(pinf));
    TEST(! none.adjacent(cl1));
    TEST(! ninf.adjacent(none));
    TEST(! ninf.adjacent(ninf));
    TEST(! ninf.adjacent(pinf));
    TEST(! ninf.adjacent(cl1));
    TEST(! pinf.adjacent(none));
    TEST(! pinf.adjacent(ninf));
    TEST(! pinf.adjacent(pinf));
    TEST(! pinf.adjacent(cl1));
    TEST(! cl1.adjacent(none));
    TEST(! cl1.adjacent(ninf));
    TEST(! cl1.adjacent(pinf));
    TEST(! cl1.adjacent(cl1));
    TEST(cl1.adjacent(cl2));
    TEST(cl2.adjacent(cl1));

}

void test_rs_interval_integral_boundary_comparison() {

    static const B none = {0, BT::empty};
    static const B ninf = {0, BT::minus_infinity};
    static const B pinf = {0, BT::plus_infinity};
    static const B cl1 = {1, BT::closed};
    static const B cl4 = {4, BT::closed};

    TEST(! none.compare_ll(none));
    TEST(none.compare_ll(ninf));
    TEST(none.compare_ll(pinf));
    TEST(none.compare_ll(cl1));
    TEST(! ninf.compare_ll(none));
    TEST(! ninf.compare_ll(ninf));
    TEST(ninf.compare_ll(pinf));
    TEST(ninf.compare_ll(cl1));
    TEST(! pinf.compare_ll(none));
    TEST(! pinf.compare_ll(ninf));
    TEST(! pinf.compare_ll(pinf));
    TEST(! pinf.compare_ll(cl1));
    TEST(! cl1.compare_ll(none));
    TEST(! cl1.compare_ll(ninf));
    TEST(cl1.compare_ll(pinf));
    TEST(! cl1.compare_ll(cl1));
    TEST(cl1.compare_ll(cl4));
    TEST(! cl4.compare_ll(cl1));

    TEST(! none.compare_rr(none));
    TEST(none.compare_rr(ninf));
    TEST(none.compare_rr(pinf));
    TEST(none.compare_rr(cl1));
    TEST(! ninf.compare_rr(none));
    TEST(! ninf.compare_rr(ninf));
    TEST(ninf.compare_rr(pinf));
    TEST(ninf.compare_rr(cl1));
    TEST(! pinf.compare_rr(none));
    TEST(! pinf.compare_rr(ninf));
    TEST(! pinf.compare_rr(pinf));
    TEST(! pinf.compare_rr(cl1));
    TEST(! cl1.compare_rr(none));
    TEST(! cl1.compare_rr(ninf));
    TEST(cl1.compare_rr(pinf));
    TEST(! cl1.compare_rr(cl1));
    TEST(cl1.compare_rr(cl4));
    TEST(! cl4.compare_rr(cl1));

    TEST(! none.compare_lr(none));
    TEST(none.compare_lr(ninf));
    TEST(none.compare_lr(pinf));
    TEST(none.compare_lr(cl1));
    TEST(! ninf.compare_lr(none));
    TEST(! ninf.compare_lr(ninf));
    TEST(ninf.compare_lr(pinf));
    TEST(ninf.compare_lr(cl1));
    TEST(! pinf.compare_lr(none));
    TEST(! pinf.compare_lr(ninf));
    TEST(! pinf.compare_lr(pinf));
    TEST(! pinf.compare_lr(cl1));
    TEST(! cl1.compare_lr(none));
    TEST(! cl1.compare_lr(ninf));
    TEST(cl1.compare_lr(pinf));
    TEST(! cl1.compare_lr(cl1));
    TEST(cl1.compare_lr(cl4));
    TEST(! cl4.compare_lr(cl1));

    TEST(! none.compare_rl(none));
    TEST(none.compare_rl(ninf));
    TEST(none.compare_rl(pinf));
    TEST(none.compare_rl(cl1));
    TEST(! ninf.compare_rl(none));
    TEST(! ninf.compare_rl(ninf));
    TEST(ninf.compare_rl(pinf));
    TEST(ninf.compare_rl(cl1));
    TEST(! pinf.compare_rl(none));
    TEST(! pinf.compare_rl(ninf));
    TEST(! pinf.compare_rl(pinf));
    TEST(! pinf.compare_rl(cl1));
    TEST(! cl1.compare_rl(none));
    TEST(! cl1.compare_rl(ninf));
    TEST(cl1.compare_rl(pinf));
    TEST(! cl1.compare_rl(cl1));
    TEST(cl1.compare_rl(cl4));
    TEST(! cl4.compare_rl(cl1));

}
