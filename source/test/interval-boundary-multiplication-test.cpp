#include "rs-interval.hpp"
#include "test/unit-test.hpp"

using namespace RS::Intervals;
using namespace RS::Intervals::Detail;

using B = Boundary<int>;
using IB = IntervalBound;

namespace RS::Intervals::Detail {

    // Temporary work

    // enum class IntervalBound
    //     empty
    //     closed
    //     open
    //     unbound
    // struct Boundary
    //     T value
    //     IntervalBound bound
    //     bool upper

    template <typename T>
    Boundary<T> operator*(const Boundary<T>& l, const Boundary<T>& r) {
        // TODO
        (void)l;
        (void)r;
        return {};
    }

}

void test_rs_interval_boundary_multiplication() {

    B el0(0, IB::empty, false);    B el2(2, IB::empty, false);    B el_3(-3, IB::empty, false);
    B eu0(0, IB::empty, true);     B eu2(2, IB::empty, true);     B eu_3(-3, IB::empty, true);
    B cl0(0, IB::closed, false);   B cl2(2, IB::closed, false);   B cl_3(-3, IB::closed, false);
    B cu0(0, IB::closed, true);    B cu2(2, IB::closed, true);    B cu_3(-3, IB::closed, true);
    B ol0(0, IB::open, false);     B ol2(2, IB::open, false);     B ol_3(-3, IB::open, false);
    B ou0(0, IB::open, true);      B ou2(2, IB::open, true);      B ou_3(-3, IB::open, true);
    B ul0(0, IB::unbound, false);  B ul2(2, IB::unbound, false);  B ul_3(-3, IB::unbound, false);
    B uu0(0, IB::unbound, true);   B uu2(2, IB::unbound, true);   B uu_3(-3, IB::unbound, true);

    // TODO

    // [2,3) * (4,5]
    // 2*4=8   op  l,l -> l
    // 2*5=10  cl  l,r -> -
    // 3*4=12  op  r,l -> -
    // 3*5=15  op  r,r -> r

    // [2,3) * (-4,5]
    // 2*(-4)=-8   op  l,l -> -
    // 2*5=10      cl  l,r -> -
    // 3*(-4)=-12  op  r,l -> l
    // 3*5=15      op  r,r -> r

    // [2,3) * (-5,-4]
    // 2*(-4)=-8   op  l,l -> -
    // 2*(-5)=-10  cl  l,r -> -
    // 3*(-4)=-12  op  r,l -> l
    // 3*(-5)=-15  op  r,r -> r


    /* lb=0=empty    rb=0=empty    !lu  !ru  */  //TEST_EQUAL((el2 * el2).str(), "");
    /* lb=0=empty    rb=0=empty    !lu  ru   */  //TEST_EQUAL((el2 * eu2).str(), "");
    /* lb=0=empty    rb=0=empty    lu   !ru  */  //TEST_EQUAL((eu2 * el2).str(), "");
    /* lb=0=empty    rb=0=empty    lu   ru   */  //TEST_EQUAL((eu2 * eu2).str(), "");
    /* lb=0=empty    rb=1=closed   !lu  !ru  */  //TEST_EQUAL((el2 * cl2).str(), "");
    /* lb=0=empty    rb=1=closed   !lu  ru   */  //TEST_EQUAL((el2 * cu2).str(), "");
    /* lb=0=empty    rb=1=closed   lu   !ru  */  //TEST_EQUAL((eu2 * cl2).str(), "");
    /* lb=0=empty    rb=1=closed   lu   ru   */  //TEST_EQUAL((eu2 * cu2).str(), "");
    /* lb=0=empty    rb=2=open     !lu  !ru  */  //TEST_EQUAL((el2 * ol2).str(), "");
    /* lb=0=empty    rb=2=open     !lu  ru   */  //TEST_EQUAL((el2 * ou2).str(), "");
    /* lb=0=empty    rb=2=open     lu   !ru  */  //TEST_EQUAL((eu2 * ol2).str(), "");
    /* lb=0=empty    rb=2=open     lu   ru   */  //TEST_EQUAL((eu2 * ou2).str(), "");
    /* lb=0=empty    rb=3=unbound  !lu  !ru  */  //TEST_EQUAL((el2 * ul2).str(), "");
    /* lb=0=empty    rb=3=unbound  !lu  ru   */  //TEST_EQUAL((el2 * uu2).str(), "");
    /* lb=0=empty    rb=3=unbound  lu   !ru  */  //TEST_EQUAL((eu2 * ul2).str(), "");
    /* lb=0=empty    rb=3=unbound  lu   ru   */  //TEST_EQUAL((eu2 * uu2).str(), "");
    /* lb=1=closed   rb=0=empty    !lu  !ru  */  //TEST_EQUAL((cl2 * el2).str(), "");
    /* lb=1=closed   rb=0=empty    !lu  ru   */  //TEST_EQUAL((cl2 * eu2).str(), "");
    /* lb=1=closed   rb=0=empty    lu   !ru  */  //TEST_EQUAL((cu2 * el2).str(), "");
    /* lb=1=closed   rb=0=empty    lu   ru   */  //TEST_EQUAL((cu2 * eu2).str(), "");
    /* lb=1=closed   rb=1=closed   !lu  !ru  */  //TEST_EQUAL((cl2 * cl2).str(), "");
    /* lb=1=closed   rb=1=closed   !lu  ru   */  //TEST_EQUAL((cl2 * cu2).str(), "");
    /* lb=1=closed   rb=1=closed   lu   !ru  */  //TEST_EQUAL((cu2 * cl2).str(), "");
    /* lb=1=closed   rb=1=closed   lu   ru   */  //TEST_EQUAL((cu2 * cu2).str(), "");
    /* lb=1=closed   rb=2=open     !lu  !ru  */  //TEST_EQUAL((cl2 * ol2).str(), "");
    /* lb=1=closed   rb=2=open     !lu  ru   */  //TEST_EQUAL((cl2 * ou2).str(), "");
    /* lb=1=closed   rb=2=open     lu   !ru  */  //TEST_EQUAL((cu2 * ol2).str(), "");
    /* lb=1=closed   rb=2=open     lu   ru   */  //TEST_EQUAL((cu2 * ou2).str(), "");
    /* lb=1=closed   rb=3=unbound  !lu  !ru  */  //TEST_EQUAL((cl2 * ul2).str(), "");
    /* lb=1=closed   rb=3=unbound  !lu  ru   */  //TEST_EQUAL((cl2 * uu2).str(), "");
    /* lb=1=closed   rb=3=unbound  lu   !ru  */  //TEST_EQUAL((cu2 * ul2).str(), "");
    /* lb=1=closed   rb=3=unbound  lu   ru   */  //TEST_EQUAL((cu2 * uu2).str(), "");
    /* lb=2=open     rb=0=empty    !lu  !ru  */  //TEST_EQUAL((ol2 * el2).str(), "");
    /* lb=2=open     rb=0=empty    !lu  ru   */  //TEST_EQUAL((ol2 * eu2).str(), "");
    /* lb=2=open     rb=0=empty    lu   !ru  */  //TEST_EQUAL((ou2 * el2).str(), "");
    /* lb=2=open     rb=0=empty    lu   ru   */  //TEST_EQUAL((ou2 * eu2).str(), "");
    /* lb=2=open     rb=1=closed   !lu  !ru  */  //TEST_EQUAL((ol2 * cl2).str(), "");
    /* lb=2=open     rb=1=closed   !lu  ru   */  //TEST_EQUAL((ol2 * cu2).str(), "");
    /* lb=2=open     rb=1=closed   lu   !ru  */  //TEST_EQUAL((ou2 * cl2).str(), "");
    /* lb=2=open     rb=1=closed   lu   ru   */  //TEST_EQUAL((ou2 * cu2).str(), "");
    /* lb=2=open     rb=2=open     !lu  !ru  */  //TEST_EQUAL((ol2 * ol2).str(), "");
    /* lb=2=open     rb=2=open     !lu  ru   */  //TEST_EQUAL((ol2 * ou2).str(), "");
    /* lb=2=open     rb=2=open     lu   !ru  */  //TEST_EQUAL((ou2 * ol2).str(), "");
    /* lb=2=open     rb=2=open     lu   ru   */  //TEST_EQUAL((ou2 * ou2).str(), "");
    /* lb=2=open     rb=3=unbound  !lu  !ru  */  //TEST_EQUAL((ol2 * ul2).str(), "");
    /* lb=2=open     rb=3=unbound  !lu  ru   */  //TEST_EQUAL((ol2 * uu2).str(), "");
    /* lb=2=open     rb=3=unbound  lu   !ru  */  //TEST_EQUAL((ou2 * ul2).str(), "");
    /* lb=2=open     rb=3=unbound  lu   ru   */  //TEST_EQUAL((ou2 * uu2).str(), "");
    /* lb=3=unbound  rb=0=empty    !lu  !ru  */  //TEST_EQUAL((ul2 * el2).str(), "");
    /* lb=3=unbound  rb=0=empty    !lu  ru   */  //TEST_EQUAL((ul2 * eu2).str(), "");
    /* lb=3=unbound  rb=0=empty    lu   !ru  */  //TEST_EQUAL((uu2 * el2).str(), "");
    /* lb=3=unbound  rb=0=empty    lu   ru   */  //TEST_EQUAL((uu2 * eu2).str(), "");
    /* lb=3=unbound  rb=1=closed   !lu  !ru  */  //TEST_EQUAL((ul2 * cl2).str(), "");
    /* lb=3=unbound  rb=1=closed   !lu  ru   */  //TEST_EQUAL((ul2 * cu2).str(), "");
    /* lb=3=unbound  rb=1=closed   lu   !ru  */  //TEST_EQUAL((uu2 * cl2).str(), "");
    /* lb=3=unbound  rb=1=closed   lu   ru   */  //TEST_EQUAL((uu2 * cu2).str(), "");
    /* lb=3=unbound  rb=2=open     !lu  !ru  */  //TEST_EQUAL((ul2 * ol2).str(), "");
    /* lb=3=unbound  rb=2=open     !lu  ru   */  //TEST_EQUAL((ul2 * ou2).str(), "");
    /* lb=3=unbound  rb=2=open     lu   !ru  */  //TEST_EQUAL((uu2 * ol2).str(), "");
    /* lb=3=unbound  rb=2=open     lu   ru   */  //TEST_EQUAL((uu2 * ou2).str(), "");
    /* lb=3=unbound  rb=3=unbound  !lu  !ru  */  //TEST_EQUAL((ul2 * ul2).str(), "");
    /* lb=3=unbound  rb=3=unbound  !lu  ru   */  //TEST_EQUAL((ul2 * uu2).str(), "");
    /* lb=3=unbound  rb=3=unbound  lu   !ru  */  //TEST_EQUAL((uu2 * ul2).str(), "");
    /* lb=3=unbound  rb=3=unbound  lu   ru   */  //TEST_EQUAL((uu2 * uu2).str(), "");

    // TODO

    // 2*_3
    // 0*2

}
