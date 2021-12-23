#include "rs-interval.hpp"
#include "rs-unit-test.hpp"
#include <cstdlib>

using namespace RS::Intervals;
using namespace RS::Intervals::Detail;

using B = Boundary<double>;
using BT = BoundaryType;

namespace RS::Intervals::Detail {

    template <typename T>
    Boundary<T> Boundary<T>::operator*(const Boundary& b) const {

        using BT = BoundaryType;

        static const Boundary zero(T(), BT::exact);

        const Boundary& a = *this;

        // (1) If either argument is empty, the result is empty
        if (a.type == BT::null || b.type == BT::null)
            return {};

        // (2) Symmetric cases
        if (a < zero && b < zero)
            return (- a) * (- b);
        if (a < zero)
            return - (-a) * b;
        if (b < zero)
            return - a * (-b);
        if (a < b)
            return b * a;

        // (3) If the RHS is a closed zero, the result is a closed zero (LHS zero is already covered)
        if (b == zero)
            return zero;

        // (4) if the RHS is zero plus epsilon, the result is zero + epsilon
        if (b == Boundary(T(), BT::plus_epsilon))
            return b;

        // (5) If the LHS is positive infinity, the result is positive infinity
        if (a == Boundary(T(), BT::plus_infinity))
            return a;

        // All cases that include an infinity have now been covered

        T product = a.value * b.value;

        // (6) If both arguments are closed, the result is closed
        if (a.type == BT::exact && b.type == BT::exact)
            return {product, BT::exact};

        // (7) If one argument has an epsilon but the other does not, the result has the same epsilon
        if (std::abs(a.type) == 1 && b.type == BT::exact)
            return {product, a.type};
        if (std::abs(b.type) == 1 && a.type == BT::exact)
            return {product, b.type};

        // (8) If both sides have the same epsilon, the result has the same epsilon
        if (a.type == b.type)
            return {product, a.type};

        // (9) The remaining case is where the arguments have epsilons of opposite signs
        int sign = -1;
        if (a.value != b.value) {
            sign = int(a.type);
            if (a.value < b.value)
                sign = - sign;
        }
        return {product, BT(sign)};

    }



    //  case 9   087  +x-eps  *  +y+eps  ->  +xy-x*eps+y*eps-eps^2
    //  case 9   107  +x+eps  *  +y-eps  ->  +xy+x*eps-y*eps-eps^2
    //  case 8   085  +x-eps  *  +y-eps  ->  +xy-eps
    //  case 8   109  +x+eps  *  +y+eps  ->  +xy+eps
    //  case 7a  086  +x-eps  *  +y      ->  +xy-eps
    //  case 7a  108  +x+eps  *  +y      ->  +xy+eps
    //  case 7b  096  +x      *  +y-eps  ->  +xy-eps
    //  case 7b  098  +x      *  +y+eps  ->  +xy+eps
    //  case 6   097  +x      *  +y      ->  +xy
    //  case 5   118  +inf    *  +y-eps  ->  +inf
    //  case 5   119  +inf    *  +y      ->  +inf
    //  case 5   120  +inf    *  +y+eps  ->  +inf
    //  case 5   121  +inf    *  +inf    ->  +inf
    //  case 4   073  0+eps   *  0+eps   ->  0+eps
    //  case 4   084  +x-eps  *  0+eps   ->  0+eps
    //  case 4   095  +x      *  0+eps   ->  0+eps
    //  case 4   106  +x+eps  *  0+eps   ->  0+eps
    //  case 4   117  +inf    *  0+eps   ->  0+eps
    //  case 2a  001  -inf    *  -inf    ->  +inf
    //  case 2a  002  -inf    *  -y-eps  ->  +inf
    //  case 2a  003  -inf    *  -y      ->  +inf
    //  case 2a  004  -inf    *  -y+eps  ->  +inf
    //  case 2a  012  -x-eps  *  -inf    ->  +inf
    //  case 2a  013  -x-eps  *  -y-eps  ->  +xy+eps
    //  case 2a  014  -x-eps  *  -y      ->  +xy+eps
    //  case 2a  015  -x-eps  *  -y+eps  ->  +xy-eps
    //  case 2a  023  -x      *  -inf    ->  +inf
    //  case 2a  024  -x      *  -y-eps  ->  +xy+eps
    //  case 2a  025  -x      *  -y      ->  +xy
    //  case 2a  026  -x      *  -y+eps  ->  +xy-eps
    //  case 2a  034  -x+eps  *  -inf    ->  +inf
    //  case 2a  035  -x+eps  *  -y-eps  ->  +xy-eps
    //  case 2a  036  -x+eps  *  -y      ->  +xy-eps
    //  case 2a  037  -x+eps  *  -y+eps  ->  +xy-eps
    //  case 2b  005  -inf    *  0-eps   ->  0+eps
    //  case 2b  006  -inf    *  0       ->  0
    //  case 2b  007  -inf    *  0+eps   ->  0-eps
    //  case 2b  008  -inf    *  +y-eps  ->  -inf
    //  case 2b  009  -inf    *  +y      ->  -inf
    //  case 2b  010  -inf    *  +y+eps  ->  -inf
    //  case 2b  011  -inf    *  +inf    ->  -inf
    //  case 2b  016  -x-eps  *  0-eps   ->  0+eps
    //  case 2b  017  -x-eps  *  0       ->  0
    //  case 2b  018  -x-eps  *  0+eps   ->  0-eps
    //  case 2b  019  -x-eps  *  +y-eps  ->  -xy+eps
    //  case 2b  020  -x-eps  *  +y      ->  -xy-eps
    //  case 2b  021  -x-eps  *  +y+eps  ->  -xy-eps
    //  case 2b  022  -x-eps  *  +inf    ->  -inf
    //  case 2b  027  -x      *  0-eps   ->  0+eps
    //  case 2b  028  -x      *  0       ->  0
    //  case 2b  029  -x      *  0+eps   ->  0-eps
    //  case 2b  030  -x      *  +y-eps  ->  -xy+eps
    //  case 2b  031  -x      *  +y      ->  -xy
    //  case 2b  032  -x      *  +y+eps  ->  -xy-eps
    //  case 2b  033  -x      *  +inf    ->  -inf
    //  case 2b  038  -x+eps  *  0-eps   ->  0+eps
    //  case 2b  039  -x+eps  *  0       ->  0
    //  case 2b  040  -x+eps  *  0+eps   ->  0-eps
    //  case 2b  041  -x+eps  *  +y-eps  ->  -xy+eps
    //  case 2b  042  -x+eps  *  +y      ->  -xy+eps
    //  case 2b  043  -x+eps  *  +y+eps  ->  -xy+eps
    //  case 2b  044  -x+eps  *  +inf    ->  -inf
    //  case 2b  049  0-eps   *  0-eps   ->  0+eps
    //  case 2b  050  0-eps   *  0       ->  0
    //  case 2b  051  0-eps   *  0+eps   ->  0-eps
    //  case 2b  052  0-eps   *  +y-eps  ->  0-eps
    //  case 2b  053  0-eps   *  +y      ->  0-eps
    //  case 2b  054  0-eps   *  +y+eps  ->  0-eps
    //  case 2b  055  0-eps   *  +inf    ->  0-eps
    //  case 2c  045  0-eps   *  -inf    ->  0+eps
    //  case 2c  046  0-eps   *  -y-eps  ->  0+eps
    //  case 2c  047  0-eps   *  -y      ->  0+eps
    //  case 2c  048  0-eps   *  -y+eps  ->  0+eps
    //  case 2c  056  0       *  -inf    ->  0
    //  case 2c  057  0       *  -y-eps  ->  0
    //  case 2c  058  0       *  -y      ->  0
    //  case 2c  059  0       *  -y+eps  ->  0
    //  case 2c  060  0       *  0-eps   ->  0
    //  case 2c  067  0+eps   *  -inf    ->  0-eps
    //  case 2c  068  0+eps   *  -y-eps  ->  0-eps
    //  case 2c  069  0+eps   *  -y      ->  0-eps
    //  case 2c  070  0+eps   *  -y+eps  ->  0-eps
    //  case 2c  071  0+eps   *  0-eps   ->  0-eps
    //  case 2c  078  +x-eps  *  -inf    ->  -inf
    //  case 2c  079  +x-eps  *  -y-eps  ->  -xy+eps
    //  case 2c  080  +x-eps  *  -y      ->  -xy+eps
    //  case 2c  081  +x-eps  *  -y+eps  ->  -xy+eps
    //  case 2c  082  +x-eps  *  0-eps   ->  0-eps
    //  case 2c  089  +x      *  -inf    ->  -inf
    //  case 2c  090  +x      *  -y-eps  ->  -xy-eps
    //  case 2c  091  +x      *  -y      ->  -xy
    //  case 2c  092  +x      *  -y+eps  ->  -xy+eps
    //  case 2c  093  +x      *  0-eps   ->  0-eps
    //  case 2c  100  +x+eps  *  -inf    ->  -inf
    //  case 2c  101  +x+eps  *  -y-eps  ->  -xy-eps
    //  case 2c  102  +x+eps  *  -y      ->  -xy-eps
    //  case 2c  103  +x+eps  *  -y+eps  ->  -xy+eps
    //  case 2c  104  +x+eps  *  0-eps   ->  0-eps
    //  case 2c  111  +inf    *  -inf    ->  -inf
    //  case 2c  112  +inf    *  -y-eps  ->  -inf
    //  case 2c  113  +inf    *  -y      ->  -inf
    //  case 2c  114  +inf    *  -y+eps  ->  -inf
    //  case 2c  115  +inf    *  0-eps   ->  0-eps
    //  case 2d  062  0       *  0+eps   ->  0
    //  case 2d  063  0       *  +y-eps  ->  0
    //  case 2d  064  0       *  +y      ->  0
    //  case 2d  065  0       *  +y+eps  ->  0
    //  case 2d  066  0       *  +inf    ->  0
    //  case 2d  074  0+eps   *  +y-eps  ->  0+eps
    //  case 2d  075  0+eps   *  +y      ->  0+eps
    //  case 2d  076  0+eps   *  +y+eps  ->  0+eps
    //  case 2d  077  0+eps   *  +inf    ->  0+eps
    //  case 2d  088  +x-eps  *  +inf    ->  +inf
    //  case 2d  099  +x      *  +inf    ->  +inf
    //  case 2d  110  +x+eps  *  +inf    ->  +inf
    //  case 3   061  0       *  0       ->  0
    //  case 3   072  0+eps   *  0       ->  0
    //  case 3   083  +x-eps  *  0       ->  0
    //  case 3   094  +x      *  0       ->  0
    //  case 3   105  +x+eps  *  0       ->  0
    //  case 3   116  +inf    *  0       ->  0



}

void test_rs_interval_boundary_multiplication() {

    static const B null(0, BT::null);
    static const B minf(0, BT::minus_infinity);
    static const B pinf(0, BT::plus_infinity);
    static const B me2(2, BT::minus_epsilon);
    static const B ex2(2, BT::exact);
    static const B pe2(2, BT::plus_epsilon);
    static const B me4(4, BT::minus_epsilon);
    static const B ex4(4, BT::exact);
    static const B pe4(4, BT::plus_epsilon);
    static const B me9(9, BT::minus_epsilon);
    static const B ex9(9, BT::exact);
    static const B pe9(9, BT::plus_epsilon);
    static const B me_3(-3, BT::minus_epsilon);
    static const B ex_3(-3, BT::exact);
    static const B pe_3(-3, BT::plus_epsilon);
    static const B me_6(-6, BT::minus_epsilon);
    static const B ex_6(-6, BT::exact);
    static const B pe_6(-6, BT::plus_epsilon);

    // TODO

}
