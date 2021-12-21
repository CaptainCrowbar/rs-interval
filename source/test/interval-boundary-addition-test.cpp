#include "rs-interval.hpp"
#include "rs-unit-test.hpp"

using namespace RS::Intervals;
using namespace RS::Intervals::Detail;

using B = Boundary<double>;
using BT = BoundaryType;

void test_rs_interval_boundary_inversion() {

    B em0(0, BT::empty);
    B mi0(0, BT::minus_infinity);
    B pi0(0, BT::plus_infinity);
    B jb2(2, BT::just_below);
    B cl2(2, BT::closed);
    B ja2(2, BT::just_above);
    B jb_2(-2, BT::just_below);
    B cl_2(-2, BT::closed);
    B ja_2(-2, BT::just_above);

    TEST_EQUAL(- em0, em0);
    TEST_EQUAL(- mi0, pi0);
    TEST_EQUAL(- pi0, mi0);
    TEST_EQUAL(- jb2, ja_2);
    TEST_EQUAL(- cl2, cl_2);
    TEST_EQUAL(- ja2, jb_2);
    TEST_EQUAL(- jb_2, ja2);
    TEST_EQUAL(- cl_2, cl2);
    TEST_EQUAL(- ja_2, jb2);

}

void test_rs_interval_boundary_addition() {

    B em0(0, BT::empty);
    B mi0(0, BT::minus_infinity);
    B pi0(0, BT::plus_infinity);
    B jb1(1, BT::just_below);
    B cl1(1, BT::closed);
    B ja1(1, BT::just_above);
    B jb2(2, BT::just_below);
    B cl2(2, BT::closed);
    B ja2(2, BT::just_above);
    B jb3(3, BT::just_below);
    B cl3(3, BT::closed);
    B ja3(3, BT::just_above);

    TEST_EQUAL(em0 + em0, em0);
    TEST_EQUAL(em0 + mi0, em0);
    TEST_EQUAL(em0 + pi0, em0);
    TEST_EQUAL(em0 + jb2, em0);
    TEST_EQUAL(em0 + cl2, em0);
    TEST_EQUAL(em0 + ja2, em0);
    TEST_EQUAL(mi0 + em0, em0);
    TEST_EQUAL(mi0 + mi0, mi0);
    TEST_EQUAL(mi0 + cl2, mi0);
    TEST_EQUAL(mi0 + ja2, mi0);
    TEST_EQUAL(pi0 + em0, em0);
    TEST_EQUAL(pi0 + pi0, pi0);
    TEST_EQUAL(pi0 + jb2, pi0);
    TEST_EQUAL(pi0 + cl2, pi0);
    TEST_EQUAL(jb1 + em0, em0);
    TEST_EQUAL(jb1 + pi0, pi0);
    TEST_EQUAL(jb1 + jb2, jb3);
    TEST_EQUAL(jb1 + cl2, jb3);
    TEST_EQUAL(cl1 + em0, em0);
    TEST_EQUAL(cl1 + mi0, mi0);
    TEST_EQUAL(cl1 + pi0, pi0);
    TEST_EQUAL(cl1 + jb2, jb3);
    TEST_EQUAL(cl1 + cl2, cl3);
    TEST_EQUAL(cl1 + ja2, ja3);
    TEST_EQUAL(ja1 + em0, em0);
    TEST_EQUAL(ja1 + mi0, mi0);
    TEST_EQUAL(ja1 + cl2, ja3);
    TEST_EQUAL(ja1 + ja2, ja3);

}

void test_rs_interval_boundary_subtraction() {

    B em0(0, BT::empty);
    B mi0(0, BT::minus_infinity);
    B pi0(0, BT::plus_infinity);
    B jb1(1, BT::just_below);
    B cl1(1, BT::closed);
    B ja1(1, BT::just_above);
    B jb2(2, BT::just_below);
    B cl2(2, BT::closed);
    B ja2(2, BT::just_above);
    B jb3(3, BT::just_below);
    B cl3(3, BT::closed);
    B ja3(3, BT::just_above);

    TEST_EQUAL(em0 - em0, em0);
    TEST_EQUAL(em0 - mi0, em0);
    TEST_EQUAL(em0 - pi0, em0);
    TEST_EQUAL(em0 - jb2, em0);
    TEST_EQUAL(em0 - cl2, em0);
    TEST_EQUAL(em0 - ja2, em0);
    TEST_EQUAL(mi0 - em0, em0);
    TEST_EQUAL(mi0 - pi0, mi0);
    TEST_EQUAL(mi0 - jb2, mi0);
    TEST_EQUAL(mi0 - cl2, mi0);
    TEST_EQUAL(pi0 - em0, em0);
    TEST_EQUAL(pi0 - mi0, pi0);
    TEST_EQUAL(pi0 - cl2, pi0);
    TEST_EQUAL(pi0 - ja2, pi0);
    TEST_EQUAL(jb3 - em0, em0);
    TEST_EQUAL(jb3 - mi0, pi0);
    TEST_EQUAL(jb3 - cl2, jb1);
    TEST_EQUAL(jb3 - ja2, jb1);
    TEST_EQUAL(cl3 - em0, em0);
    TEST_EQUAL(cl3 - mi0, pi0);
    TEST_EQUAL(cl3 - pi0, mi0);
    TEST_EQUAL(cl3 - jb2, ja1);
    TEST_EQUAL(cl3 - cl2, cl1);
    TEST_EQUAL(cl3 - ja2, jb1);
    TEST_EQUAL(ja3 - em0, em0);
    TEST_EQUAL(ja3 - pi0, mi0);
    TEST_EQUAL(ja3 - jb2, ja1);
    TEST_EQUAL(ja3 - cl2, ja1);

}
