#include "rs-interval.hpp"
#include "test/unit-test.hpp"

using namespace RS::Intervals;
using namespace RS::Intervals::Detail;

using B = Boundary<int>;
using IB = IntervalBound;

namespace RS::Intervals::Detail {

    // Temporary work

    template <typename T>
    Boundary<T> operator*(const Boundary<T>& l, const Boundary<T>& r) {
        // TODO
        (void)l;
        (void)r;
        return {};
    }

}

void test_rs_interval_boundary_multiplication() {

    B el0(0, IB::empty, false);    B el2(2, IB::empty, false);    B el_2(-2, IB::empty, false);
    B eu0(0, IB::empty, true);     B eu2(2, IB::empty, true);     B eu_2(-2, IB::empty, true);
    B cl0(0, IB::closed, false);   B cl2(2, IB::closed, false);   B cl_2(-2, IB::closed, false);
    B cu0(0, IB::closed, true);    B cu2(2, IB::closed, true);    B cu_2(-2, IB::closed, true);
    B ol0(0, IB::open, false);     B ol2(2, IB::open, false);     B ol_2(-2, IB::open, false);
    B ou0(0, IB::open, true);      B ou2(2, IB::open, true);      B ou_2(-2, IB::open, true);
    B ul0(0, IB::unbound, false);  B ul2(2, IB::unbound, false);  B ul_2(-2, IB::unbound, false);
    B uu0(0, IB::unbound, true);   B uu2(2, IB::unbound, true);   B uu_2(-2, IB::unbound, true);

    // TODO

}
