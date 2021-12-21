#include "rs-interval.hpp"
#include "rs-unit-test.hpp"
#include <string>

using namespace RS::Intervals;
using namespace RS::Intervals::Detail;

using B = Boundary<double>;
using BT = BoundaryType;

void test_rs_interval_boundary_formatting() {

    B b;

    TRY((b = {42, BT::empty}));           TEST_EQUAL(b.str(), "empty");
    TRY((b = {42, BT::minus_infinity}));  TEST_EQUAL(b.str(), "minus_infinity");
    TRY((b = {42, BT::just_below}));      TEST_EQUAL(b.str(), "just_below 42");
    TRY((b = {42, BT::closed}));          TEST_EQUAL(b.str(), "closed 42");
    TRY((b = {42, BT::just_above}));      TEST_EQUAL(b.str(), "just_above 42");
    TRY((b = {42, BT::plus_infinity}));   TEST_EQUAL(b.str(), "plus_infinity");

}

void test_rs_interval_boundary_adjacency() {

    B em0(0, BT::empty);
    B mi0(0, BT::minus_infinity);
    B pi0(0, BT::plus_infinity);
    B jb1(1, BT::just_below);
    B cl1(1, BT::closed);
    B ja1(1, BT::just_above);
    B jb2(2, BT::just_below);
    B cl2(2, BT::closed);
    B ja2(2, BT::just_above);

    TEST(! em0.adjacent(em0));
    TEST(! em0.adjacent(mi0));
    TEST(! em0.adjacent(pi0));
    TEST(! em0.adjacent(jb1));
    TEST(! em0.adjacent(cl1));
    TEST(! em0.adjacent(ja1));
    TEST(! em0.adjacent(jb2));
    TEST(! em0.adjacent(cl2));
    TEST(! em0.adjacent(ja2));
    TEST(! mi0.adjacent(em0));
    TEST(! mi0.adjacent(mi0));
    TEST(! mi0.adjacent(pi0));
    TEST(! mi0.adjacent(jb1));
    TEST(! mi0.adjacent(cl1));
    TEST(! mi0.adjacent(ja1));
    TEST(! mi0.adjacent(jb2));
    TEST(! mi0.adjacent(cl2));
    TEST(! mi0.adjacent(ja2));
    TEST(! pi0.adjacent(em0));
    TEST(! pi0.adjacent(mi0));
    TEST(! pi0.adjacent(pi0));
    TEST(! pi0.adjacent(jb1));
    TEST(! pi0.adjacent(cl1));
    TEST(! pi0.adjacent(ja1));
    TEST(! pi0.adjacent(jb2));
    TEST(! pi0.adjacent(cl2));
    TEST(! pi0.adjacent(ja2));
    TEST(! jb1.adjacent(em0));
    TEST(! jb1.adjacent(mi0));
    TEST(! jb1.adjacent(pi0));
    TEST(! jb1.adjacent(jb1));
    TEST(jb1.adjacent(cl1));
    TEST(! jb1.adjacent(ja1));
    TEST(! jb1.adjacent(jb2));
    TEST(! jb1.adjacent(cl2));
    TEST(! jb1.adjacent(ja2));
    TEST(! cl1.adjacent(em0));
    TEST(! cl1.adjacent(mi0));
    TEST(! cl1.adjacent(pi0));
    TEST(cl1.adjacent(jb1));
    TEST(! cl1.adjacent(cl1));
    TEST(cl1.adjacent(ja1));
    TEST(! cl1.adjacent(jb2));
    TEST(! cl1.adjacent(cl2));
    TEST(! cl1.adjacent(ja2));
    TEST(! ja1.adjacent(em0));
    TEST(! ja1.adjacent(mi0));
    TEST(! ja1.adjacent(pi0));
    TEST(! ja1.adjacent(jb1));
    TEST(ja1.adjacent(cl1));
    TEST(! ja1.adjacent(ja1));
    TEST(! ja1.adjacent(jb2));
    TEST(! ja1.adjacent(cl2));
    TEST(! ja1.adjacent(ja2));
    TEST(! jb2.adjacent(em0));
    TEST(! jb2.adjacent(mi0));
    TEST(! jb2.adjacent(pi0));
    TEST(! jb2.adjacent(jb1));
    TEST(! jb2.adjacent(cl1));
    TEST(! jb2.adjacent(ja1));
    TEST(! jb2.adjacent(jb2));
    TEST(jb2.adjacent(cl2));
    TEST(! jb2.adjacent(ja2));
    TEST(! cl2.adjacent(em0));
    TEST(! cl2.adjacent(mi0));
    TEST(! cl2.adjacent(pi0));
    TEST(! cl2.adjacent(jb1));
    TEST(! cl2.adjacent(cl1));
    TEST(! cl2.adjacent(ja1));
    TEST(cl2.adjacent(jb2));
    TEST(! cl2.adjacent(cl2));
    TEST(cl2.adjacent(ja2));
    TEST(! ja2.adjacent(em0));
    TEST(! ja2.adjacent(mi0));
    TEST(! ja2.adjacent(pi0));
    TEST(! ja2.adjacent(jb1));
    TEST(! ja2.adjacent(cl1));
    TEST(! ja2.adjacent(ja1));
    TEST(! ja2.adjacent(jb2));
    TEST(ja2.adjacent(cl2));
    TEST(! ja2.adjacent(ja2));

}

void test_rs_interval_boundary_comparison() {

    B em0(0, BT::empty);
    B mi0(0, BT::minus_infinity);
    B pi0(0, BT::plus_infinity);
    B jb1(1, BT::just_below);
    B cl1(1, BT::closed);
    B ja1(1, BT::just_above);
    B jb2(2, BT::just_below);
    B cl2(2, BT::closed);
    B ja2(2, BT::just_above);

    TEST_EQUAL(em0.compare(em0), 0);
    TEST_EQUAL(em0.compare(mi0), -1);
    TEST_EQUAL(em0.compare(pi0), -1);
    TEST_EQUAL(em0.compare(jb1), -1);
    TEST_EQUAL(em0.compare(cl1), -1);
    TEST_EQUAL(em0.compare(ja1), -1);
    TEST_EQUAL(em0.compare(jb2), -1);
    TEST_EQUAL(em0.compare(cl2), -1);
    TEST_EQUAL(em0.compare(ja2), -1);
    TEST_EQUAL(mi0.compare(em0), 1);
    TEST_EQUAL(mi0.compare(mi0), 0);
    TEST_EQUAL(mi0.compare(pi0), -1);
    TEST_EQUAL(mi0.compare(jb1), -1);
    TEST_EQUAL(mi0.compare(cl1), -1);
    TEST_EQUAL(mi0.compare(ja1), -1);
    TEST_EQUAL(mi0.compare(jb2), -1);
    TEST_EQUAL(mi0.compare(cl2), -1);
    TEST_EQUAL(mi0.compare(ja2), -1);
    TEST_EQUAL(pi0.compare(em0), 1);
    TEST_EQUAL(pi0.compare(mi0), 1);
    TEST_EQUAL(pi0.compare(pi0), 0);
    TEST_EQUAL(pi0.compare(jb1), 1);
    TEST_EQUAL(pi0.compare(cl1), 1);
    TEST_EQUAL(pi0.compare(ja1), 1);
    TEST_EQUAL(pi0.compare(jb2), 1);
    TEST_EQUAL(pi0.compare(cl2), 1);
    TEST_EQUAL(pi0.compare(ja2), 1);
    TEST_EQUAL(jb1.compare(em0), 1);
    TEST_EQUAL(jb1.compare(mi0), 1);
    TEST_EQUAL(jb1.compare(pi0), -1);
    TEST_EQUAL(jb1.compare(jb1), 0);
    TEST_EQUAL(jb1.compare(cl1), -1);
    TEST_EQUAL(jb1.compare(ja1), -1);
    TEST_EQUAL(jb1.compare(jb2), -1);
    TEST_EQUAL(jb1.compare(cl2), -1);
    TEST_EQUAL(jb1.compare(ja2), -1);
    TEST_EQUAL(cl1.compare(em0), 1);
    TEST_EQUAL(cl1.compare(mi0), 1);
    TEST_EQUAL(cl1.compare(pi0), -1);
    TEST_EQUAL(cl1.compare(jb1), 1);
    TEST_EQUAL(cl1.compare(cl1), 0);
    TEST_EQUAL(cl1.compare(ja1), -1);
    TEST_EQUAL(cl1.compare(jb2), -1);
    TEST_EQUAL(cl1.compare(cl2), -1);
    TEST_EQUAL(cl1.compare(ja2), -1);
    TEST_EQUAL(ja1.compare(em0), 1);
    TEST_EQUAL(ja1.compare(mi0), 1);
    TEST_EQUAL(ja1.compare(pi0), -1);
    TEST_EQUAL(ja1.compare(jb1), 1);
    TEST_EQUAL(ja1.compare(cl1), 1);
    TEST_EQUAL(ja1.compare(ja1), 0);
    TEST_EQUAL(ja1.compare(jb2), -1);
    TEST_EQUAL(ja1.compare(cl2), -1);
    TEST_EQUAL(ja1.compare(ja2), -1);
    TEST_EQUAL(jb2.compare(em0), 1);
    TEST_EQUAL(jb2.compare(mi0), 1);
    TEST_EQUAL(jb2.compare(pi0), -1);
    TEST_EQUAL(jb2.compare(jb1), 1);
    TEST_EQUAL(jb2.compare(cl1), 1);
    TEST_EQUAL(jb2.compare(ja1), 1);
    TEST_EQUAL(jb2.compare(jb2), 0);
    TEST_EQUAL(jb2.compare(cl2), -1);
    TEST_EQUAL(jb2.compare(ja2), -1);
    TEST_EQUAL(cl2.compare(em0), 1);
    TEST_EQUAL(cl2.compare(mi0), 1);
    TEST_EQUAL(cl2.compare(pi0), -1);
    TEST_EQUAL(cl2.compare(jb1), 1);
    TEST_EQUAL(cl2.compare(cl1), 1);
    TEST_EQUAL(cl2.compare(ja1), 1);
    TEST_EQUAL(cl2.compare(jb2), 1);
    TEST_EQUAL(cl2.compare(cl2), 0);
    TEST_EQUAL(cl2.compare(ja2), -1);
    TEST_EQUAL(ja2.compare(em0), 1);
    TEST_EQUAL(ja2.compare(mi0), 1);
    TEST_EQUAL(ja2.compare(pi0), -1);
    TEST_EQUAL(ja2.compare(jb1), 1);
    TEST_EQUAL(ja2.compare(cl1), 1);
    TEST_EQUAL(ja2.compare(ja1), 1);
    TEST_EQUAL(ja2.compare(jb2), 1);
    TEST_EQUAL(ja2.compare(cl2), 1);
    TEST_EQUAL(ja2.compare(ja2), 0);

}
