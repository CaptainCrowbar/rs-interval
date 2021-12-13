#include "rs-interval.hpp"
#include "test/unit-test.hpp"

using namespace RS::Intervals;
using namespace RS::Intervals::Detail;

using B = Boundary<int>;
using IB = IntervalBound;

namespace RS::Intervals {

    namespace Detail {

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
            // The upper flag in the result is not important
            using IB = IntervalBound;
            bool u = l.upper && r.upper;
            if (l.bound == IB::empty || r.bound == IB::empty) // case 1
                return {{}, IB::empty, u};
            bool lzero = is_bounded(l.bound) && l.value == T();
            bool rzero = is_bounded(r.bound) && r.value == T();
            if (lzero && rzero) // case 2a
                return {{}, std::max(l.bound, r.bound), u};
            else if (lzero) // case 2b
                return {{}, l.bound, u};
            else if (rzero) // case 2c
                return {{}, r.bound, u};
            else if (l.bound == IB::unbound || r.bound == IB::unbound) // case 3
                return {{}, IB::unbound, u};
            else // case 4
                return {l.value * r.value, std::max(l.bound, r.bound), u};
        }

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

    /* C1   lb=0=empty    rb=0=empty    !lu  !ru  v=0*0   */  TEST_EQUAL((el0 * el2).str(), "left empty");
    /* C1   lb=0=empty    rb=0=empty    !lu  !ru  v=0*0   */  TEST_EQUAL((el2 * el2).str(), "left empty");
    /* C1   lb=0=empty    rb=0=empty    !lu  !ru  v=0*0   */  TEST_EQUAL((el_3 * el2).str(), "left empty");
    /* C1   lb=0=empty    rb=0=empty    !lu  ru   v=0*0   */  TEST_EQUAL((el0 * eu2).str(), "left empty");
    /* C1   lb=0=empty    rb=0=empty    !lu  ru   v=0*0   */  TEST_EQUAL((el2 * eu2).str(), "left empty");
    /* C1   lb=0=empty    rb=0=empty    !lu  ru   v=0*0   */  TEST_EQUAL((el_3 * eu2).str(), "left empty");
    /* C1   lb=0=empty    rb=0=empty    lu   !ru  v=0*0   */  TEST_EQUAL((eu0 * el2).str(), "left empty");
    /* C1   lb=0=empty    rb=0=empty    lu   !ru  v=0*0   */  TEST_EQUAL((eu2 * el2).str(), "left empty");
    /* C1   lb=0=empty    rb=0=empty    lu   !ru  v=0*0   */  TEST_EQUAL((eu_3 * el2).str(), "left empty");
    /* C1   lb=0=empty    rb=0=empty    lu   ru   v=0*0   */  TEST_EQUAL((eu0 * eu2).str(), "right empty");
    /* C1   lb=0=empty    rb=0=empty    lu   ru   v=0*0   */  TEST_EQUAL((eu2 * eu2).str(), "right empty");
    /* C1   lb=0=empty    rb=0=empty    lu   ru   v=0*0   */  TEST_EQUAL((eu_3 * eu2).str(), "right empty");
    /* C1   lb=0=empty    rb=1=closed   !lu  !ru  v=0*2   */  TEST_EQUAL((el0 * cl2).str(), "left empty");
    /* C1   lb=0=empty    rb=1=closed   !lu  !ru  v=0*2   */  TEST_EQUAL((el2 * cl2).str(), "left empty");
    /* C1   lb=0=empty    rb=1=closed   !lu  !ru  v=0*2   */  TEST_EQUAL((el_3 * cl2).str(), "left empty");
    /* C1   lb=0=empty    rb=1=closed   !lu  ru   v=0*2   */  TEST_EQUAL((el0 * cu2).str(), "left empty");
    /* C1   lb=0=empty    rb=1=closed   !lu  ru   v=0*2   */  TEST_EQUAL((el2 * cu2).str(), "left empty");
    /* C1   lb=0=empty    rb=1=closed   !lu  ru   v=0*2   */  TEST_EQUAL((el_3 * cu2).str(), "left empty");
    /* C1   lb=0=empty    rb=1=closed   lu   !ru  v=0*2   */  TEST_EQUAL((eu0 * cl2).str(), "left empty");
    /* C1   lb=0=empty    rb=1=closed   lu   !ru  v=0*2   */  TEST_EQUAL((eu2 * cl2).str(), "left empty");
    /* C1   lb=0=empty    rb=1=closed   lu   !ru  v=0*2   */  TEST_EQUAL((eu_3 * cl2).str(), "left empty");
    /* C1   lb=0=empty    rb=1=closed   lu   ru   v=0*2   */  TEST_EQUAL((eu0 * cu2).str(), "right empty");
    /* C1   lb=0=empty    rb=1=closed   lu   ru   v=0*2   */  TEST_EQUAL((eu2 * cu2).str(), "right empty");
    /* C1   lb=0=empty    rb=1=closed   lu   ru   v=0*2   */  TEST_EQUAL((eu_3 * cu2).str(), "right empty");
    /* C1   lb=0=empty    rb=2=open     !lu  !ru  v=0*2   */  TEST_EQUAL((el0 * ol2).str(), "left empty");
    /* C1   lb=0=empty    rb=2=open     !lu  !ru  v=0*2   */  TEST_EQUAL((el2 * ol2).str(), "left empty");
    /* C1   lb=0=empty    rb=2=open     !lu  !ru  v=0*2   */  TEST_EQUAL((el_3 * ol2).str(), "left empty");
    /* C1   lb=0=empty    rb=2=open     !lu  ru   v=0*2   */  TEST_EQUAL((el0 * ou2).str(), "left empty");
    /* C1   lb=0=empty    rb=2=open     !lu  ru   v=0*2   */  TEST_EQUAL((el2 * ou2).str(), "left empty");
    /* C1   lb=0=empty    rb=2=open     !lu  ru   v=0*2   */  TEST_EQUAL((el_3 * ou2).str(), "left empty");
    /* C1   lb=0=empty    rb=2=open     lu   !ru  v=0*2   */  TEST_EQUAL((eu0 * ol2).str(), "left empty");
    /* C1   lb=0=empty    rb=2=open     lu   !ru  v=0*2   */  TEST_EQUAL((eu2 * ol2).str(), "left empty");
    /* C1   lb=0=empty    rb=2=open     lu   !ru  v=0*2   */  TEST_EQUAL((eu_3 * ol2).str(), "left empty");
    /* C1   lb=0=empty    rb=2=open     lu   ru   v=0*2   */  TEST_EQUAL((eu0 * ou2).str(), "right empty");
    /* C1   lb=0=empty    rb=2=open     lu   ru   v=0*2   */  TEST_EQUAL((eu2 * ou2).str(), "right empty");
    /* C1   lb=0=empty    rb=2=open     lu   ru   v=0*2   */  TEST_EQUAL((eu_3 * ou2).str(), "right empty");
    /* C1   lb=0=empty    rb=3=unbound  !lu  !ru  v=0*0   */  TEST_EQUAL((el0 * ul2).str(), "left empty");
    /* C1   lb=0=empty    rb=3=unbound  !lu  !ru  v=0*0   */  TEST_EQUAL((el2 * ul2).str(), "left empty");
    /* C1   lb=0=empty    rb=3=unbound  !lu  !ru  v=0*0   */  TEST_EQUAL((el_3 * ul2).str(), "left empty");
    /* C1   lb=0=empty    rb=3=unbound  !lu  ru   v=0*0   */  TEST_EQUAL((el0 * uu2).str(), "left empty");
    /* C1   lb=0=empty    rb=3=unbound  !lu  ru   v=0*0   */  TEST_EQUAL((el2 * uu2).str(), "left empty");
    /* C1   lb=0=empty    rb=3=unbound  !lu  ru   v=0*0   */  TEST_EQUAL((el_3 * uu2).str(), "left empty");
    /* C1   lb=0=empty    rb=3=unbound  lu   !ru  v=0*0   */  TEST_EQUAL((eu0 * ul2).str(), "left empty");
    /* C1   lb=0=empty    rb=3=unbound  lu   !ru  v=0*0   */  TEST_EQUAL((eu2 * ul2).str(), "left empty");
    /* C1   lb=0=empty    rb=3=unbound  lu   !ru  v=0*0   */  TEST_EQUAL((eu_3 * ul2).str(), "left empty");
    /* C1   lb=0=empty    rb=3=unbound  lu   ru   v=0*0   */  TEST_EQUAL((eu0 * uu2).str(), "right empty");
    /* C1   lb=0=empty    rb=3=unbound  lu   ru   v=0*0   */  TEST_EQUAL((eu2 * uu2).str(), "right empty");
    /* C1   lb=0=empty    rb=3=unbound  lu   ru   v=0*0   */  TEST_EQUAL((eu_3 * uu2).str(), "right empty");
    /* C1   lb=1=closed   rb=0=empty    !lu  !ru  v=-3*0  */  TEST_EQUAL((cl_3 * el2).str(), "left empty");
    /* C1   lb=1=closed   rb=0=empty    !lu  !ru  v=0*0   */  TEST_EQUAL((cl0 * el2).str(), "left empty");
    /* C1   lb=1=closed   rb=0=empty    !lu  !ru  v=2*0   */  TEST_EQUAL((cl2 * el2).str(), "left empty");
    /* C1   lb=1=closed   rb=0=empty    !lu  ru   v=-3*0  */  TEST_EQUAL((cl_3 * eu2).str(), "left empty");
    /* C1   lb=1=closed   rb=0=empty    !lu  ru   v=0*0   */  TEST_EQUAL((cl0 * eu2).str(), "left empty");
    /* C1   lb=1=closed   rb=0=empty    !lu  ru   v=2*0   */  TEST_EQUAL((cl2 * eu2).str(), "left empty");
    /* C1   lb=1=closed   rb=0=empty    lu   !ru  v=-3*0  */  TEST_EQUAL((cu_3 * el2).str(), "left empty");
    /* C1   lb=1=closed   rb=0=empty    lu   !ru  v=0*0   */  TEST_EQUAL((cu0 * el2).str(), "left empty");
    /* C1   lb=1=closed   rb=0=empty    lu   !ru  v=2*0   */  TEST_EQUAL((cu2 * el2).str(), "left empty");
    /* C1   lb=1=closed   rb=0=empty    lu   ru   v=-3*0  */  TEST_EQUAL((cu_3 * eu2).str(), "right empty");
    /* C1   lb=1=closed   rb=0=empty    lu   ru   v=0*0   */  TEST_EQUAL((cu0 * eu2).str(), "right empty");
    /* C1   lb=1=closed   rb=0=empty    lu   ru   v=2*0   */  TEST_EQUAL((cu2 * eu2).str(), "right empty");
    /* C1   lb=2=open     rb=0=empty    !lu  !ru  v=-3*0  */  TEST_EQUAL((ol_3 * el2).str(), "left empty");
    /* C1   lb=2=open     rb=0=empty    !lu  !ru  v=0*0   */  TEST_EQUAL((ol0 * el2).str(), "left empty");
    /* C1   lb=2=open     rb=0=empty    !lu  !ru  v=2*0   */  TEST_EQUAL((ol2 * el2).str(), "left empty");
    /* C1   lb=2=open     rb=0=empty    !lu  ru   v=-3*0  */  TEST_EQUAL((ol_3 * eu2).str(), "left empty");
    /* C1   lb=2=open     rb=0=empty    !lu  ru   v=0*0   */  TEST_EQUAL((ol0 * eu2).str(), "left empty");
    /* C1   lb=2=open     rb=0=empty    !lu  ru   v=2*0   */  TEST_EQUAL((ol2 * eu2).str(), "left empty");
    /* C1   lb=2=open     rb=0=empty    lu   !ru  v=-3*0  */  TEST_EQUAL((ou_3 * el2).str(), "left empty");
    /* C1   lb=2=open     rb=0=empty    lu   !ru  v=0*0   */  TEST_EQUAL((ou0 * el2).str(), "left empty");
    /* C1   lb=2=open     rb=0=empty    lu   !ru  v=2*0   */  TEST_EQUAL((ou2 * el2).str(), "left empty");
    /* C1   lb=2=open     rb=0=empty    lu   ru   v=-3*0  */  TEST_EQUAL((ou_3 * eu2).str(), "right empty");
    /* C1   lb=2=open     rb=0=empty    lu   ru   v=0*0   */  TEST_EQUAL((ou0 * eu2).str(), "right empty");
    /* C1   lb=2=open     rb=0=empty    lu   ru   v=2*0   */  TEST_EQUAL((ou2 * eu2).str(), "right empty");
    /* C1   lb=3=unbound  rb=0=empty    !lu  !ru  v=0*0   */  TEST_EQUAL((ul0 * el2).str(), "left empty");
    /* C1   lb=3=unbound  rb=0=empty    !lu  !ru  v=0*0   */  TEST_EQUAL((ul2 * el2).str(), "left empty");
    /* C1   lb=3=unbound  rb=0=empty    !lu  !ru  v=0*0   */  TEST_EQUAL((ul_3 * el2).str(), "left empty");
    /* C1   lb=3=unbound  rb=0=empty    !lu  ru   v=0*0   */  TEST_EQUAL((ul0 * eu2).str(), "left empty");
    /* C1   lb=3=unbound  rb=0=empty    !lu  ru   v=0*0   */  TEST_EQUAL((ul2 * eu2).str(), "left empty");
    /* C1   lb=3=unbound  rb=0=empty    !lu  ru   v=0*0   */  TEST_EQUAL((ul_3 * eu2).str(), "left empty");
    /* C1   lb=3=unbound  rb=0=empty    lu   !ru  v=0*0   */  TEST_EQUAL((uu0 * el2).str(), "left empty");
    /* C1   lb=3=unbound  rb=0=empty    lu   !ru  v=0*0   */  TEST_EQUAL((uu2 * el2).str(), "left empty");
    /* C1   lb=3=unbound  rb=0=empty    lu   !ru  v=0*0   */  TEST_EQUAL((uu_3 * el2).str(), "left empty");
    /* C1   lb=3=unbound  rb=0=empty    lu   ru   v=0*0   */  TEST_EQUAL((uu0 * eu2).str(), "right empty");
    /* C1   lb=3=unbound  rb=0=empty    lu   ru   v=0*0   */  TEST_EQUAL((uu2 * eu2).str(), "right empty");
    /* C1   lb=3=unbound  rb=0=empty    lu   ru   v=0*0   */  TEST_EQUAL((uu_3 * eu2).str(), "right empty");
    /* C2b  lb=1=closed   rb=1=closed   !lu  !ru  v=0*2   */  TEST_EQUAL((cl0 * cl2).str(), "left closed 0");
    /* C2b  lb=1=closed   rb=1=closed   !lu  ru   v=0*2   */  TEST_EQUAL((cl0 * cu2).str(), "left closed 0");
    /* C2b  lb=1=closed   rb=1=closed   lu   !ru  v=0*2   */  TEST_EQUAL((cu0 * cl2).str(), "left closed 0");
    /* C2b  lb=1=closed   rb=1=closed   lu   ru   v=0*2   */  TEST_EQUAL((cu0 * cu2).str(), "right closed 0");
    /* C2b  lb=1=closed   rb=2=open     !lu  !ru  v=0*2   */  TEST_EQUAL((cl0 * ol2).str(), "left closed 0");
    /* C2b  lb=1=closed   rb=2=open     !lu  ru   v=0*2   */  TEST_EQUAL((cl0 * ou2).str(), "left closed 0");
    /* C2b  lb=1=closed   rb=2=open     lu   !ru  v=0*2   */  TEST_EQUAL((cu0 * ol2).str(), "left closed 0");
    /* C2b  lb=1=closed   rb=2=open     lu   ru   v=0*2   */  TEST_EQUAL((cu0 * ou2).str(), "right closed 0");
    /* C2b  lb=1=closed   rb=3=unbound  !lu  !ru  v=0*0   */  TEST_EQUAL((cl0 * ul2).str(), "left closed 0");
    /* C2b  lb=1=closed   rb=3=unbound  !lu  ru   v=0*0   */  TEST_EQUAL((cl0 * uu2).str(), "left closed 0");
    /* C2b  lb=1=closed   rb=3=unbound  lu   !ru  v=0*0   */  TEST_EQUAL((cu0 * ul2).str(), "left closed 0");
    /* C2b  lb=1=closed   rb=3=unbound  lu   ru   v=0*0   */  TEST_EQUAL((cu0 * uu2).str(), "right closed 0");
    /* C2b  lb=2=open     rb=1=closed   !lu  !ru  v=0*2   */  TEST_EQUAL((ol0 * cl2).str(), "left open 0");
    /* C2b  lb=2=open     rb=1=closed   !lu  ru   v=0*2   */  TEST_EQUAL((ol0 * cu2).str(), "left open 0");
    /* C2b  lb=2=open     rb=1=closed   lu   !ru  v=0*2   */  TEST_EQUAL((ou0 * cl2).str(), "left open 0");
    /* C2b  lb=2=open     rb=1=closed   lu   ru   v=0*2   */  TEST_EQUAL((ou0 * cu2).str(), "right open 0");
    /* C2b  lb=2=open     rb=2=open     !lu  !ru  v=0*2   */  TEST_EQUAL((ol0 * ol2).str(), "left open 0");
    /* C2b  lb=2=open     rb=2=open     !lu  ru   v=0*2   */  TEST_EQUAL((ol0 * ou2).str(), "left open 0");
    /* C2b  lb=2=open     rb=2=open     lu   !ru  v=0*2   */  TEST_EQUAL((ou0 * ol2).str(), "left open 0");
    /* C2b  lb=2=open     rb=2=open     lu   ru   v=0*2   */  TEST_EQUAL((ou0 * ou2).str(), "right open 0");
    /* C2b  lb=2=open     rb=3=unbound  !lu  !ru  v=0*0   */  TEST_EQUAL((ol0 * ul2).str(), "left open 0");
    /* C2b  lb=2=open     rb=3=unbound  !lu  ru   v=0*0   */  TEST_EQUAL((ol0 * uu2).str(), "left open 0");
    /* C2b  lb=2=open     rb=3=unbound  lu   !ru  v=0*0   */  TEST_EQUAL((ou0 * ul2).str(), "left open 0");
    /* C2b  lb=2=open     rb=3=unbound  lu   ru   v=0*0   */  TEST_EQUAL((ou0 * uu2).str(), "right open 0");
    /* C3   lb=1=closed   rb=3=unbound  !lu  !ru  v=-3*0  */  TEST_EQUAL((cl_3 * ul2).str(), "left unbound");
    /* C3   lb=1=closed   rb=3=unbound  !lu  !ru  v=2*0   */  TEST_EQUAL((cl2 * ul2).str(), "left unbound");
    /* C3   lb=1=closed   rb=3=unbound  !lu  ru   v=-3*0  */  TEST_EQUAL((cl_3 * uu2).str(), "left unbound");
    /* C3   lb=1=closed   rb=3=unbound  !lu  ru   v=2*0   */  TEST_EQUAL((cl2 * uu2).str(), "left unbound");
    /* C3   lb=1=closed   rb=3=unbound  lu   !ru  v=-3*0  */  TEST_EQUAL((cu_3 * ul2).str(), "left unbound");
    /* C3   lb=1=closed   rb=3=unbound  lu   !ru  v=2*0   */  TEST_EQUAL((cu2 * ul2).str(), "left unbound");
    /* C3   lb=1=closed   rb=3=unbound  lu   ru   v=-3*0  */  TEST_EQUAL((cu_3 * uu2).str(), "right unbound");
    /* C3   lb=1=closed   rb=3=unbound  lu   ru   v=2*0   */  TEST_EQUAL((cu2 * uu2).str(), "right unbound");
    /* C3   lb=2=open     rb=3=unbound  !lu  !ru  v=-3*0  */  TEST_EQUAL((ol_3 * ul2).str(), "left unbound");
    /* C3   lb=2=open     rb=3=unbound  !lu  !ru  v=2*0   */  TEST_EQUAL((ol2 * ul2).str(), "left unbound");
    /* C3   lb=2=open     rb=3=unbound  !lu  ru   v=-3*0  */  TEST_EQUAL((ol_3 * uu2).str(), "left unbound");
    /* C3   lb=2=open     rb=3=unbound  !lu  ru   v=2*0   */  TEST_EQUAL((ol2 * uu2).str(), "left unbound");
    /* C3   lb=2=open     rb=3=unbound  lu   !ru  v=-3*0  */  TEST_EQUAL((ou_3 * ul2).str(), "left unbound");
    /* C3   lb=2=open     rb=3=unbound  lu   !ru  v=2*0   */  TEST_EQUAL((ou2 * ul2).str(), "left unbound");
    /* C3   lb=2=open     rb=3=unbound  lu   ru   v=-3*0  */  TEST_EQUAL((ou_3 * uu2).str(), "right unbound");
    /* C3   lb=2=open     rb=3=unbound  lu   ru   v=2*0   */  TEST_EQUAL((ou2 * uu2).str(), "right unbound");
    /* C3   lb=3=unbound  rb=1=closed   !lu  !ru  v=0*2   */  TEST_EQUAL((ul0 * cl2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=1=closed   !lu  !ru  v=0*2   */  TEST_EQUAL((ul2 * cl2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=1=closed   !lu  !ru  v=0*2   */  TEST_EQUAL((ul_3 * cl2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=1=closed   !lu  ru   v=0*2   */  TEST_EQUAL((ul0 * cu2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=1=closed   !lu  ru   v=0*2   */  TEST_EQUAL((ul2 * cu2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=1=closed   !lu  ru   v=0*2   */  TEST_EQUAL((ul_3 * cu2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=1=closed   lu   !ru  v=0*2   */  TEST_EQUAL((uu0 * cl2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=1=closed   lu   !ru  v=0*2   */  TEST_EQUAL((uu2 * cl2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=1=closed   lu   !ru  v=0*2   */  TEST_EQUAL((uu_3 * cl2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=1=closed   lu   ru   v=0*2   */  TEST_EQUAL((uu0 * cu2).str(), "right unbound");
    /* C3   lb=3=unbound  rb=1=closed   lu   ru   v=0*2   */  TEST_EQUAL((uu2 * cu2).str(), "right unbound");
    /* C3   lb=3=unbound  rb=1=closed   lu   ru   v=0*2   */  TEST_EQUAL((uu_3 * cu2).str(), "right unbound");
    /* C3   lb=3=unbound  rb=2=open     !lu  !ru  v=0*2   */  TEST_EQUAL((ul0 * ol2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=2=open     !lu  !ru  v=0*2   */  TEST_EQUAL((ul2 * ol2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=2=open     !lu  !ru  v=0*2   */  TEST_EQUAL((ul_3 * ol2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=2=open     !lu  ru   v=0*2   */  TEST_EQUAL((ul0 * ou2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=2=open     !lu  ru   v=0*2   */  TEST_EQUAL((ul2 * ou2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=2=open     !lu  ru   v=0*2   */  TEST_EQUAL((ul_3 * ou2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=2=open     lu   !ru  v=0*2   */  TEST_EQUAL((uu0 * ol2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=2=open     lu   !ru  v=0*2   */  TEST_EQUAL((uu2 * ol2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=2=open     lu   !ru  v=0*2   */  TEST_EQUAL((uu_3 * ol2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=2=open     lu   ru   v=0*2   */  TEST_EQUAL((uu0 * ou2).str(), "right unbound");
    /* C3   lb=3=unbound  rb=2=open     lu   ru   v=0*2   */  TEST_EQUAL((uu2 * ou2).str(), "right unbound");
    /* C3   lb=3=unbound  rb=2=open     lu   ru   v=0*2   */  TEST_EQUAL((uu_3 * ou2).str(), "right unbound");
    /* C3   lb=3=unbound  rb=3=unbound  !lu  !ru  v=0*0   */  TEST_EQUAL((ul0 * ul2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=3=unbound  !lu  !ru  v=0*0   */  TEST_EQUAL((ul2 * ul2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=3=unbound  !lu  !ru  v=0*0   */  TEST_EQUAL((ul_3 * ul2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=3=unbound  !lu  ru   v=0*0   */  TEST_EQUAL((ul0 * uu2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=3=unbound  !lu  ru   v=0*0   */  TEST_EQUAL((ul2 * uu2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=3=unbound  !lu  ru   v=0*0   */  TEST_EQUAL((ul_3 * uu2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=3=unbound  lu   !ru  v=0*0   */  TEST_EQUAL((uu0 * ul2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=3=unbound  lu   !ru  v=0*0   */  TEST_EQUAL((uu2 * ul2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=3=unbound  lu   !ru  v=0*0   */  TEST_EQUAL((uu_3 * ul2).str(), "left unbound");
    /* C3   lb=3=unbound  rb=3=unbound  lu   ru   v=0*0   */  TEST_EQUAL((uu0 * uu2).str(), "right unbound");
    /* C3   lb=3=unbound  rb=3=unbound  lu   ru   v=0*0   */  TEST_EQUAL((uu2 * uu2).str(), "right unbound");
    /* C3   lb=3=unbound  rb=3=unbound  lu   ru   v=0*0   */  TEST_EQUAL((uu_3 * uu2).str(), "right unbound");
    /* C4   lb=1=closed   rb=1=closed   !lu  !ru  v=-3*2  */  TEST_EQUAL((cl_3 * cl2).str(), "left closed -6");
    /* C4   lb=1=closed   rb=1=closed   !lu  !ru  v=2*2   */  TEST_EQUAL((cl2 * cl2).str(), "left closed 4");
    /* C4   lb=1=closed   rb=1=closed   !lu  ru   v=-3*2  */  TEST_EQUAL((cl_3 * cu2).str(), "left closed -6");
    /* C4   lb=1=closed   rb=1=closed   !lu  ru   v=2*2   */  TEST_EQUAL((cl2 * cu2).str(), "left closed 4");
    /* C4   lb=1=closed   rb=1=closed   lu   !ru  v=-3*2  */  TEST_EQUAL((cu_3 * cl2).str(), "left closed -6");
    /* C4   lb=1=closed   rb=1=closed   lu   !ru  v=2*2   */  TEST_EQUAL((cu2 * cl2).str(), "left closed 4");
    /* C4   lb=1=closed   rb=1=closed   lu   ru   v=-3*2  */  TEST_EQUAL((cu_3 * cu2).str(), "right closed -6");
    /* C4   lb=1=closed   rb=1=closed   lu   ru   v=2*2   */  TEST_EQUAL((cu2 * cu2).str(), "right closed 4");
    /* C4   lb=1=closed   rb=2=open     !lu  !ru  v=-3*2  */  TEST_EQUAL((cl_3 * ol2).str(), "left open -6");
    /* C4   lb=1=closed   rb=2=open     !lu  !ru  v=2*2   */  TEST_EQUAL((cl2 * ol2).str(), "left open 4");
    /* C4   lb=1=closed   rb=2=open     !lu  ru   v=-3*2  */  TEST_EQUAL((cl_3 * ou2).str(), "left open -6");
    /* C4   lb=1=closed   rb=2=open     !lu  ru   v=2*2   */  TEST_EQUAL((cl2 * ou2).str(), "left open 4");
    /* C4   lb=1=closed   rb=2=open     lu   !ru  v=-3*2  */  TEST_EQUAL((cu_3 * ol2).str(), "left open -6");
    /* C4   lb=1=closed   rb=2=open     lu   !ru  v=2*2   */  TEST_EQUAL((cu2 * ol2).str(), "left open 4");
    /* C4   lb=1=closed   rb=2=open     lu   ru   v=-3*2  */  TEST_EQUAL((cu_3 * ou2).str(), "right open -6");
    /* C4   lb=1=closed   rb=2=open     lu   ru   v=2*2   */  TEST_EQUAL((cu2 * ou2).str(), "right open 4");
    /* C4   lb=2=open     rb=1=closed   !lu  !ru  v=-3*2  */  TEST_EQUAL((ol_3 * cl2).str(), "left open -6");
    /* C4   lb=2=open     rb=1=closed   !lu  !ru  v=2*2   */  TEST_EQUAL((ol2 * cl2).str(), "left open 4");
    /* C4   lb=2=open     rb=1=closed   !lu  ru   v=-3*2  */  TEST_EQUAL((ol_3 * cu2).str(), "left open -6");
    /* C4   lb=2=open     rb=1=closed   !lu  ru   v=2*2   */  TEST_EQUAL((ol2 * cu2).str(), "left open 4");
    /* C4   lb=2=open     rb=1=closed   lu   !ru  v=-3*2  */  TEST_EQUAL((ou_3 * cl2).str(), "left open -6");
    /* C4   lb=2=open     rb=1=closed   lu   !ru  v=2*2   */  TEST_EQUAL((ou2 * cl2).str(), "left open 4");
    /* C4   lb=2=open     rb=1=closed   lu   ru   v=-3*2  */  TEST_EQUAL((ou_3 * cu2).str(), "right open -6");
    /* C4   lb=2=open     rb=1=closed   lu   ru   v=2*2   */  TEST_EQUAL((ou2 * cu2).str(), "right open 4");
    /* C4   lb=2=open     rb=2=open     !lu  !ru  v=-3*2  */  TEST_EQUAL((ol_3 * ol2).str(), "left open -6");
    /* C4   lb=2=open     rb=2=open     !lu  !ru  v=2*2   */  TEST_EQUAL((ol2 * ol2).str(), "left open 4");
    /* C4   lb=2=open     rb=2=open     !lu  ru   v=-3*2  */  TEST_EQUAL((ol_3 * ou2).str(), "left open -6");
    /* C4   lb=2=open     rb=2=open     !lu  ru   v=2*2   */  TEST_EQUAL((ol2 * ou2).str(), "left open 4");
    /* C4   lb=2=open     rb=2=open     lu   !ru  v=-3*2  */  TEST_EQUAL((ou_3 * ol2).str(), "left open -6");
    /* C4   lb=2=open     rb=2=open     lu   !ru  v=2*2   */  TEST_EQUAL((ou2 * ol2).str(), "left open 4");
    /* C4   lb=2=open     rb=2=open     lu   ru   v=-3*2  */  TEST_EQUAL((ou_3 * ou2).str(), "right open -6");
    /* C4   lb=2=open     rb=2=open     lu   ru   v=2*2   */  TEST_EQUAL((ou2 * ou2).str(), "right open 4");

}
