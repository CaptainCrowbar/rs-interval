#include "rs-intervals/interval.hpp"
#include "rs-intervals/types.hpp"
#include "test/unit-test.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace RS::Intervals;

using Itv = Interval<int>;
using IB = IntervalBound;
using IC = IntervalCategory;
using IM = IntervalMatch;
using IO = IntervalOrder;

void test_rs_interval_integral_contains_zero() {

    using namespace RS::Intervals::Detail;

    Itv in;

    TRY(in = Itv());            TEST(! contains_zero(in));
    TRY(in = Itv::all());       TEST(contains_zero(in));
    TRY(in = Itv(-2));          TEST(! contains_zero(in));
    TRY(in = Itv(-1));          TEST(! contains_zero(in));
    TRY(in = Itv(0));           TEST(contains_zero(in));
    TRY(in = Itv(1));           TEST(! contains_zero(in));
    TRY(in = Itv(2));           TEST(! contains_zero(in));
    TRY(in = Itv(-2,-2,"<="));  TEST(! contains_zero(in));
    TRY(in = Itv(-1,-1,"<="));  TEST(! contains_zero(in));
    TRY(in = Itv(0,0,"<="));    TEST(contains_zero(in));
    TRY(in = Itv(1,1,"<="));    TEST(contains_zero(in));
    TRY(in = Itv(2,2,"<="));    TEST(contains_zero(in));
    TRY(in = Itv(-2,-2,"<"));   TEST(! contains_zero(in));
    TRY(in = Itv(-1,-1,"<"));   TEST(! contains_zero(in));
    TRY(in = Itv(0,0,"<"));     TEST(! contains_zero(in));
    TRY(in = Itv(1,1,"<"));     TEST(contains_zero(in));
    TRY(in = Itv(2,2,"<"));     TEST(contains_zero(in));
    TRY(in = Itv(-2,-2,">="));  TEST(contains_zero(in));
    TRY(in = Itv(-1,-1,">="));  TEST(contains_zero(in));
    TRY(in = Itv(0,0,">="));    TEST(contains_zero(in));
    TRY(in = Itv(1,1,">="));    TEST(! contains_zero(in));
    TRY(in = Itv(2,2,">="));    TEST(! contains_zero(in));
    TRY(in = Itv(-2,-2,">"));   TEST(contains_zero(in));
    TRY(in = Itv(-1,-1,">"));   TEST(contains_zero(in));
    TRY(in = Itv(0,0,">"));     TEST(! contains_zero(in));
    TRY(in = Itv(1,1,">"));     TEST(! contains_zero(in));
    TRY(in = Itv(2,2,">"));     TEST(! contains_zero(in));
    TRY(in = Itv(-2,-1,"[]"));  TEST(! contains_zero(in));
    TRY(in = Itv(-2,-1,"()"));  TEST(! contains_zero(in));
    TRY(in = Itv(-1,0,"[]"));   TEST(contains_zero(in));
    TRY(in = Itv(-1,0,"()"));   TEST(! contains_zero(in));
    TRY(in = Itv(-1,1,"[]"));   TEST(contains_zero(in));
    TRY(in = Itv(-1,1,"()"));   TEST(contains_zero(in));
    TRY(in = Itv(0,1,"[]"));    TEST(contains_zero(in));
    TRY(in = Itv(0,1,"()"));    TEST(! contains_zero(in));
    TRY(in = Itv(1,2,"[]"));    TEST(! contains_zero(in));
    TRY(in = Itv(1,2,"()"));    TEST(! contains_zero(in));

}

void test_rs_interval_integral_interval_arithmetic() {

    Itv in;
    std::string str;

    TRY(in = + Itv());          TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = + Itv::all());     TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = + Itv(1));         TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = - Itv());          TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = - Itv::all());     TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = - Itv(1));         TRY(str = in.str());  TEST_EQUAL(str, "-1");
    TRY(in = - Itv(1,5,"[]"));  TRY(str = in.str());  TEST_EQUAL(str, "[-5,-1]");
    TRY(in = - Itv(1,5,"()"));  TRY(str = in.str());  TEST_EQUAL(str, "[-4,-2]");
    TRY(in = - Itv(1,5,"[)"));  TRY(str = in.str());  TEST_EQUAL(str, "[-4,-1]");
    TRY(in = - Itv(1,5,"(]"));  TRY(str = in.str());  TEST_EQUAL(str, "[-5,-2]");
    TRY(in = - Itv(0,5,"<"));   TRY(str = in.str());  TEST_EQUAL(str, ">=-4");
    TRY(in = - Itv(0,5,"<="));  TRY(str = in.str());  TEST_EQUAL(str, ">=-5");
    TRY(in = - Itv(5,0,">"));   TRY(str = in.str());  TEST_EQUAL(str, "<=-6");
    TRY(in = - Itv(5,0,">="));  TRY(str = in.str());  TEST_EQUAL(str, "<=-5");

    TRY(in = Itv() + 42);           TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = Itv::all() + 42);      TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = Itv(5) + 42);          TRY(str = in.str());  TEST_EQUAL(str, "47");
    TRY(in = Itv(5,10,"[]") + 42);  TRY(str = in.str());  TEST_EQUAL(str, "[47,52]");
    TRY(in = Itv(5,10,"()") + 42);  TRY(str = in.str());  TEST_EQUAL(str, "[48,51]");
    TRY(in = Itv(5,10,"[)") + 42);  TRY(str = in.str());  TEST_EQUAL(str, "[47,51]");
    TRY(in = Itv(5,10,"(]") + 42);  TRY(str = in.str());  TEST_EQUAL(str, "[48,52]");
    TRY(in = Itv(5,10,"<") + 42);   TRY(str = in.str());  TEST_EQUAL(str, "<=51");
    TRY(in = Itv(5,10,"<=") + 42);  TRY(str = in.str());  TEST_EQUAL(str, "<=52");
    TRY(in = Itv(5,10,">") + 42);   TRY(str = in.str());  TEST_EQUAL(str, ">=48");
    TRY(in = Itv(5,10,">=") + 42);  TRY(str = in.str());  TEST_EQUAL(str, ">=47");

    struct test_info {
        int line;
        Itv lhs;
        Itv rhs;
        Itv add;
        Itv sub1;
        Itv sub2;
        Itv mul;
    };

    static const std::vector<test_info> tests = {

        // line      lhs            rhs             add              sub1             sub2             mul                 --
        { __LINE__,  {},            {},             {},              {},              {},              {},                 },
        { __LINE__,  {0,0,"*"},     {},             {},              {},              {},              {},                 },
        { __LINE__,  {10},          {},             {},              {},              {},              {},                 },
        { __LINE__,  {10,20,"[]"},  {},             {},              {},              {},              {},                 },
        { __LINE__,  {10,10,"<="},  {},             {},              {},              {},              {},                 },
        { __LINE__,  {10,10,">="},  {},             {},              {},              {},              {},                 },
        { __LINE__,  {0,0,"*"},     {0,0,"*"},      {0,0,"*"},       {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10},          {0,0,"*"},      {0,0,"*"},       {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10,20,"[]"},  {0,0,"*"},      {0,0,"*"},       {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10,10,"<="},  {0,0,"*"},      {0,0,"*"},       {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10,10,">="},  {0,0,"*"},      {0,0,"*"},       {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10},          {42},           {52},            {-32},           {32},            {420},              },
        { __LINE__,  {10,20,"[]"},  {42},           {52,62,"[]"},    {-32,-22,"[]"},  {22,32,"[]"},    {420,840,"[]"},     },
        { __LINE__,  {10,10,"<="},  {42},           {52,52,"<="},    {-32,-32,"<="},  {32,32,">="},    {420,420,"<="},     },
        { __LINE__,  {10,10,">="},  {42},           {52,52,">="},    {-32,-32,">="},  {32,32,"<="},    {420,420,">="},     },
        { __LINE__,  {10,20,"[]"},  {-86,99,"[]"},  {-76,119,"[]"},  {-89,106,"[]"},  {-106,89,"[]"},  {-1720,1980,"[]"},  },
        { __LINE__,  {10,10,"<="},  {-86,99,"[]"},  {109,109,"<="},  {96,96,"<="},    {-96,-96,">="},  {0,0,"*"},          },
        { __LINE__,  {10,10,">="},  {-86,99,"[]"},  {-76,-76,">="},  {-89,-89,">="},  {89,89,"<="},    {0,0,"*"},          },
        { __LINE__,  {10,10,"<="},  {42,42,"<="},   {52,52,"<="},    {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10,10,">="},  {42,42,"<="},   {0,0,"*"},       {-32,-32,">="},  {32,32,"<="},    {0,0,"*"},          },
        { __LINE__,  {10,10,">="},  {42,42,">="},   {52,52,">="},    {0,0,"*"},       {0,0,"*"},       {420,420,">="},     },

    };

    Itv a, b, c;

    for (auto& t: tests) {

        int errors = 0;

        TRY(a = t.lhs);
        TRY(b = t.rhs);

        TRY(c = a + b);           TEST_EQUAL(c, t.add);   errors += int(c != t.add);
        TRY(c = a - b);           TEST_EQUAL(c, t.sub1);  errors += int(c != t.sub1);
        TRY(c = b - a);           TEST_EQUAL(c, t.sub2);  errors += int(c != t.sub2);
        TRY(c = a * b);           TEST_EQUAL(c, t.mul);   errors += int(c != t.mul);
        TRY(c = a); TRY(c += b);  TEST_EQUAL(c, t.add);   errors += int(c != t.add);
        TRY(c = a); TRY(c -= b);  TEST_EQUAL(c, t.sub1);  errors += int(c != t.sub1);
        TRY(c = b); TRY(c -= a);  TEST_EQUAL(c, t.sub2);  errors += int(c != t.sub2);
        TRY(c = a); TRY(c *= b);  TEST_EQUAL(c, t.mul);   errors += int(c != t.mul);

        if (errors)
            std::cout << "... [" << t.line << "] " << t.lhs << " " << t.rhs << "\n";

    }

}
