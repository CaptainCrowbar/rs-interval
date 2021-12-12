#include "rs-interval.hpp"
#include "test/unit-test.hpp"

using namespace RS::Intervals;
using namespace RS::Intervals::Detail;

using B = Boundary<int>;
using IB = IntervalBound;

void test_rs_interval_boundary_addition() {

    B el1(1, IB::empty, false);    B el2(2, IB::empty, false);
    B eu1(1, IB::empty, true);     B eu2(2, IB::empty, true);
    B cl1(1, IB::closed, false);   B cl2(2, IB::closed, false);
    B cu1(1, IB::closed, true);    B cu2(2, IB::closed, true);
    B ol1(1, IB::open, false);     B ol2(2, IB::open, false);
    B ou1(1, IB::open, true);      B ou2(2, IB::open, true);
    B ul1(1, IB::unbound, false);  B ul2(2, IB::unbound, false);
    B uu1(1, IB::unbound, true);   B uu2(2, IB::unbound, true);

    /* lb=0=empty    rb=0=empty    !lu  !ru  */  TEST_EQUAL((el1 + el2).str(), "left empty");
    /* lb=0=empty    rb=0=empty    lu   ru   */  TEST_EQUAL((eu1 + eu2).str(), "right empty");
    /* lb=0=empty    rb=1=closed   !lu  !ru  */  TEST_EQUAL((el1 + cl2).str(), "left empty");
    /* lb=0=empty    rb=1=closed   lu   ru   */  TEST_EQUAL((eu1 + cu2).str(), "right empty");
    /* lb=0=empty    rb=2=open     !lu  !ru  */  TEST_EQUAL((el1 + ol2).str(), "left empty");
    /* lb=0=empty    rb=2=open     lu   ru   */  TEST_EQUAL((eu1 + ou2).str(), "right empty");
    /* lb=0=empty    rb=3=unbound  !lu  !ru  */  TEST_EQUAL((el1 + ul2).str(), "left empty");
    /* lb=0=empty    rb=3=unbound  lu   ru   */  TEST_EQUAL((eu1 + uu2).str(), "right empty");
    /* lb=1=closed   rb=0=empty    !lu  !ru  */  TEST_EQUAL((cl1 + el2).str(), "left empty");
    /* lb=1=closed   rb=0=empty    lu   ru   */  TEST_EQUAL((cu1 + eu2).str(), "right empty");
    /* lb=1=closed   rb=1=closed   !lu  !ru  */  TEST_EQUAL((cl1 + cl2).str(), "left closed 3");
    /* lb=1=closed   rb=1=closed   lu   ru   */  TEST_EQUAL((cu1 + cu2).str(), "right closed 3");
    /* lb=1=closed   rb=2=open     !lu  !ru  */  TEST_EQUAL((cl1 + ol2).str(), "left open 3");
    /* lb=1=closed   rb=2=open     lu   ru   */  TEST_EQUAL((cu1 + ou2).str(), "right open 3");
    /* lb=1=closed   rb=3=unbound  !lu  !ru  */  TEST_EQUAL((cl1 + ul2).str(), "left unbound");
    /* lb=1=closed   rb=3=unbound  lu   ru   */  TEST_EQUAL((cu1 + uu2).str(), "right unbound");
    /* lb=2=open     rb=0=empty    !lu  !ru  */  TEST_EQUAL((ol1 + el2).str(), "left empty");
    /* lb=2=open     rb=0=empty    lu   ru   */  TEST_EQUAL((ou1 + eu2).str(), "right empty");
    /* lb=2=open     rb=1=closed   !lu  !ru  */  TEST_EQUAL((ol1 + cl2).str(), "left open 3");
    /* lb=2=open     rb=1=closed   lu   ru   */  TEST_EQUAL((ou1 + cu2).str(), "right open 3");
    /* lb=2=open     rb=2=open     !lu  !ru  */  TEST_EQUAL((ol1 + ol2).str(), "left open 3");
    /* lb=2=open     rb=2=open     lu   ru   */  TEST_EQUAL((ou1 + ou2).str(), "right open 3");
    /* lb=2=open     rb=3=unbound  !lu  !ru  */  TEST_EQUAL((ol1 + ul2).str(), "left unbound");
    /* lb=2=open     rb=3=unbound  lu   ru   */  TEST_EQUAL((ou1 + uu2).str(), "right unbound");
    /* lb=3=unbound  rb=0=empty    !lu  !ru  */  TEST_EQUAL((ul1 + el2).str(), "left empty");
    /* lb=3=unbound  rb=0=empty    lu   ru   */  TEST_EQUAL((uu1 + eu2).str(), "right empty");
    /* lb=3=unbound  rb=1=closed   !lu  !ru  */  TEST_EQUAL((ul1 + cl2).str(), "left unbound");
    /* lb=3=unbound  rb=1=closed   lu   ru   */  TEST_EQUAL((uu1 + cu2).str(), "right unbound");
    /* lb=3=unbound  rb=2=open     !lu  !ru  */  TEST_EQUAL((ul1 + ol2).str(), "left unbound");
    /* lb=3=unbound  rb=2=open     lu   ru   */  TEST_EQUAL((uu1 + ou2).str(), "right unbound");
    /* lb=3=unbound  rb=3=unbound  !lu  !ru  */  TEST_EQUAL((ul1 + ul2).str(), "left unbound");
    /* lb=3=unbound  rb=3=unbound  lu   ru   */  TEST_EQUAL((uu1 + uu2).str(), "right unbound");

}

void test_rs_interval_boundary_subtraction() {

    B el1(1, IB::empty, false);    B el2(2, IB::empty, false);
    B eu1(1, IB::empty, true);     B eu2(2, IB::empty, true);
    B cl1(1, IB::closed, false);   B cl2(2, IB::closed, false);
    B cu1(1, IB::closed, true);    B cu2(2, IB::closed, true);
    B ol1(1, IB::open, false);     B ol2(2, IB::open, false);
    B ou1(1, IB::open, true);      B ou2(2, IB::open, true);
    B ul1(1, IB::unbound, false);  B ul2(2, IB::unbound, false);
    B uu1(1, IB::unbound, true);   B uu2(2, IB::unbound, true);

    /* lb=0=empty    rb=0=empty    !lu  ru   */  TEST_EQUAL((el1 - el2).str(), "left empty");
    /* lb=0=empty    rb=0=empty    lu   !ru  */  TEST_EQUAL((eu1 - eu2).str(), "right empty");
    /* lb=0=empty    rb=1=closed   !lu  ru   */  TEST_EQUAL((el1 - cl2).str(), "left empty");
    /* lb=0=empty    rb=1=closed   lu   !ru  */  TEST_EQUAL((eu1 - cu2).str(), "right empty");
    /* lb=0=empty    rb=2=open     !lu  ru   */  TEST_EQUAL((el1 - ol2).str(), "left empty");
    /* lb=0=empty    rb=2=open     lu   !ru  */  TEST_EQUAL((eu1 - ou2).str(), "right empty");
    /* lb=0=empty    rb=3=unbound  !lu  ru   */  TEST_EQUAL((el1 - ul2).str(), "left empty");
    /* lb=0=empty    rb=3=unbound  lu   !ru  */  TEST_EQUAL((eu1 - uu2).str(), "right empty");
    /* lb=1=closed   rb=0=empty    !lu  ru   */  TEST_EQUAL((cl1 - el2).str(), "left empty");
    /* lb=1=closed   rb=0=empty    lu   !ru  */  TEST_EQUAL((cu1 - eu2).str(), "right empty");
    /* lb=1=closed   rb=1=closed   !lu  ru   */  TEST_EQUAL((cl1 - cl2).str(), "left closed -1");
    /* lb=1=closed   rb=1=closed   lu   !ru  */  TEST_EQUAL((cu1 - cu2).str(), "right closed -1");
    /* lb=1=closed   rb=2=open     !lu  ru   */  TEST_EQUAL((cl1 - ol2).str(), "left open -1");
    /* lb=1=closed   rb=2=open     lu   !ru  */  TEST_EQUAL((cu1 - ou2).str(), "right open -1");
    /* lb=1=closed   rb=3=unbound  !lu  ru   */  TEST_EQUAL((cl1 - ul2).str(), "left unbound");
    /* lb=1=closed   rb=3=unbound  lu   !ru  */  TEST_EQUAL((cu1 - uu2).str(), "right unbound");
    /* lb=2=open     rb=0=empty    !lu  ru   */  TEST_EQUAL((ol1 - el2).str(), "left empty");
    /* lb=2=open     rb=0=empty    lu   !ru  */  TEST_EQUAL((ou1 - eu2).str(), "right empty");
    /* lb=2=open     rb=1=closed   !lu  ru   */  TEST_EQUAL((ol1 - cl2).str(), "left open -1");
    /* lb=2=open     rb=1=closed   lu   !ru  */  TEST_EQUAL((ou1 - cu2).str(), "right open -1");
    /* lb=2=open     rb=2=open     !lu  ru   */  TEST_EQUAL((ol1 - ol2).str(), "left open -1");
    /* lb=2=open     rb=2=open     lu   !ru  */  TEST_EQUAL((ou1 - ou2).str(), "right open -1");
    /* lb=2=open     rb=3=unbound  !lu  ru   */  TEST_EQUAL((ol1 - ul2).str(), "left unbound");
    /* lb=2=open     rb=3=unbound  lu   !ru  */  TEST_EQUAL((ou1 - uu2).str(), "right unbound");
    /* lb=3=unbound  rb=0=empty    !lu  ru   */  TEST_EQUAL((ul1 - el2).str(), "left empty");
    /* lb=3=unbound  rb=0=empty    lu   !ru  */  TEST_EQUAL((uu1 - eu2).str(), "right empty");
    /* lb=3=unbound  rb=1=closed   !lu  ru   */  TEST_EQUAL((ul1 - cl2).str(), "left unbound");
    /* lb=3=unbound  rb=1=closed   lu   !ru  */  TEST_EQUAL((uu1 - cu2).str(), "right unbound");
    /* lb=3=unbound  rb=2=open     !lu  ru   */  TEST_EQUAL((ul1 - ol2).str(), "left unbound");
    /* lb=3=unbound  rb=2=open     lu   !ru  */  TEST_EQUAL((uu1 - ou2).str(), "right unbound");
    /* lb=3=unbound  rb=3=unbound  !lu  ru   */  TEST_EQUAL((ul1 - ul2).str(), "left unbound");
    /* lb=3=unbound  rb=3=unbound  lu   !ru  */  TEST_EQUAL((uu1 - uu2).str(), "right unbound");

}
