#include "rs-interval/interval.hpp"
#include "rs-interval/set.hpp"
#include "rs-interval/types.hpp"
#include "test/unit-test.hpp"
#include <format>
#include <print>
#include <string>
#include <vector>

using namespace RS::Interval;

using Itv = Interval<double>;
using Set = IntervalSet<double>;

void test_rs_interval_continuous_contains_zero() {

    using namespace RS::Interval::Detail;

    Itv in;

    TRY(in = Itv());            TEST(! contains_zero(in));
    TRY(in = Itv::all());       TEST(contains_zero(in));
    TRY(in = Itv(-1));          TEST(! contains_zero(in));
    TRY(in = Itv(0));           TEST(contains_zero(in));
    TRY(in = Itv(1));           TEST(! contains_zero(in));
    TRY(in = Itv(-1,-1,"<="));  TEST(! contains_zero(in));
    TRY(in = Itv(0,0,"<="));    TEST(contains_zero(in));
    TRY(in = Itv(1,1,"<="));    TEST(contains_zero(in));
    TRY(in = Itv(-1,-1,"<"));   TEST(! contains_zero(in));
    TRY(in = Itv(0,0,"<"));     TEST(! contains_zero(in));
    TRY(in = Itv(1,1,"<"));     TEST(contains_zero(in));
    TRY(in = Itv(-1,-1,">="));  TEST(contains_zero(in));
    TRY(in = Itv(0,0,">="));    TEST(contains_zero(in));
    TRY(in = Itv(1,1,">="));    TEST(! contains_zero(in));
    TRY(in = Itv(-1,-1,">"));   TEST(contains_zero(in));
    TRY(in = Itv(0,0,">"));     TEST(! contains_zero(in));
    TRY(in = Itv(1,1,">"));     TEST(! contains_zero(in));
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

void test_rs_interval_continuous_interval_reciprocal() {

    using namespace RS::Interval::Detail;

    Itv in;
    Set rec;
    std::string str;

    TRY(in = Itv());            TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 0u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{}");
    TRY(in = Itv::all());       TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 2u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{<0,>0}");
    TRY(in = Itv(-2));          TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{-0.5}");
    TRY(in = Itv(0));           TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 0u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{}");
    TRY(in = Itv(2));           TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{0.5}");
    TRY(in = Itv(-2,-2,"<="));  TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{[-0.5,0)}");
    TRY(in = Itv(-2,-2,"<"));   TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{(-0.5,0)}");
    TRY(in = Itv(-2,-2,">="));  TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 2u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{<=-0.5,>0}");
    TRY(in = Itv(-2,-2,">"));   TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 2u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{<-0.5,>0}");
    TRY(in = Itv(0,0,"<="));    TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{<0}");
    TRY(in = Itv(0,0,"<"));     TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{<0}");
    TRY(in = Itv(0,0,">="));    TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{>0}");
    TRY(in = Itv(0,0,">"));     TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{>0}");
    TRY(in = Itv(2,2,"<="));    TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 2u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{<0,>=0.5}");
    TRY(in = Itv(2,2,"<"));     TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 2u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{<0,>0.5}");
    TRY(in = Itv(2,2,">="));    TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{(0,0.5]}");
    TRY(in = Itv(2,2,">"));     TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{(0,0.5)}");
    TRY(in = Itv(-4,-2,"[]"));  TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{[-0.5,-0.25]}");
    TRY(in = Itv(-4,-2,"()"));  TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{(-0.5,-0.25)}");
    TRY(in = Itv(-4,-2,"[)"));  TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{(-0.5,-0.25]}");
    TRY(in = Itv(-4,-2,"(]"));  TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{[-0.5,-0.25)}");
    TRY(in = Itv(-2,0,"[]"));   TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{<=-0.5}");
    TRY(in = Itv(-2,0,"()"));   TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{<-0.5}");
    TRY(in = Itv(-2,0,"[)"));   TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{<=-0.5}");
    TRY(in = Itv(-2,0,"(]"));   TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{<-0.5}");
    TRY(in = Itv(-2,2,"[]"));   TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 2u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{<=-0.5,>=0.5}");
    TRY(in = Itv(-2,2,"()"));   TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 2u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{<-0.5,>0.5}");
    TRY(in = Itv(-2,2,"[)"));   TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 2u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{<=-0.5,>0.5}");
    TRY(in = Itv(-2,2,"(]"));   TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 2u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{<-0.5,>=0.5}");
    TRY(in = Itv(0,2,"[]"));    TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{>=0.5}");
    TRY(in = Itv(0,2,"()"));    TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{>0.5}");
    TRY(in = Itv(0,2,"[)"));    TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{>0.5}");
    TRY(in = Itv(0,2,"(]"));    TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{>=0.5}");
    TRY(in = Itv(2,4,"[]"));    TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{[0.25,0.5]}");
    TRY(in = Itv(2,4,"()"));    TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{(0.25,0.5)}");
    TRY(in = Itv(2,4,"[)"));    TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{(0.25,0.5]}");
    TRY(in = Itv(2,4,"(]"));    TRY(rec = reciprocal_set(in));  TEST_EQUAL(rec.size(), 1u);  TRY(str = std::format("{}", rec));  TEST_EQUAL(str, "{[0.25,0.5)}");

}

void test_rs_interval_continuous_interval_arithmetic() {

    Itv in;
    std::string str;

    TRY(in = + Itv());          TRY(str = std::format("{}", in));  TEST_EQUAL(str, "{}");
    TRY(in = + Itv::all());     TRY(str = std::format("{}", in));  TEST_EQUAL(str, "*");
    TRY(in = + Itv(1));         TRY(str = std::format("{}", in));  TEST_EQUAL(str, "1");
    TRY(in = - Itv());          TRY(str = std::format("{}", in));  TEST_EQUAL(str, "{}");
    TRY(in = - Itv::all());     TRY(str = std::format("{}", in));  TEST_EQUAL(str, "*");
    TRY(in = - Itv(1));         TRY(str = std::format("{}", in));  TEST_EQUAL(str, "-1");
    TRY(in = - Itv(1,5,"[]"));  TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[-5,-1]");
    TRY(in = - Itv(1,5,"()"));  TRY(str = std::format("{}", in));  TEST_EQUAL(str, "(-5,-1)");
    TRY(in = - Itv(1,5,"[)"));  TRY(str = std::format("{}", in));  TEST_EQUAL(str, "(-5,-1]");
    TRY(in = - Itv(1,5,"(]"));  TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[-5,-1)");
    TRY(in = - Itv(0,5,"<"));   TRY(str = std::format("{}", in));  TEST_EQUAL(str, ">-5");
    TRY(in = - Itv(0,5,"<="));  TRY(str = std::format("{}", in));  TEST_EQUAL(str, ">=-5");
    TRY(in = - Itv(5,0,">"));   TRY(str = std::format("{}", in));  TEST_EQUAL(str, "<-5");
    TRY(in = - Itv(5,0,">="));  TRY(str = std::format("{}", in));  TEST_EQUAL(str, "<=-5");

    TRY(in = Itv() + 42.0);           TRY(str = std::format("{}", in));  TEST_EQUAL(str, "{}");
    TRY(in = Itv::all() + 42.0);      TRY(str = std::format("{}", in));  TEST_EQUAL(str, "*");
    TRY(in = Itv(5) + 42.0);          TRY(str = std::format("{}", in));  TEST_EQUAL(str, "47");
    TRY(in = Itv(5,10,"[]") + 42.0);  TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[47,52]");
    TRY(in = Itv(5,10,"()") + 42.0);  TRY(str = std::format("{}", in));  TEST_EQUAL(str, "(47,52)");
    TRY(in = Itv(5,10,"[)") + 42.0);  TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[47,52)");
    TRY(in = Itv(5,10,"(]") + 42.0);  TRY(str = std::format("{}", in));  TEST_EQUAL(str, "(47,52]");
    TRY(in = Itv(5,10,"<") + 42.0);   TRY(str = std::format("{}", in));  TEST_EQUAL(str, "<52");
    TRY(in = Itv(5,10,"<=") + 42.0);  TRY(str = std::format("{}", in));  TEST_EQUAL(str, "<=52");
    TRY(in = Itv(5,10,">") + 42.0);   TRY(str = std::format("{}", in));  TEST_EQUAL(str, ">47");
    TRY(in = Itv(5,10,">=") + 42.0);  TRY(str = std::format("{}", in));  TEST_EQUAL(str, ">=47");

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
        { __LINE__,  {10,20,"[)"},  {},             {},              {},              {},              {},                 },
        { __LINE__,  {10,20,"(]"},  {},             {},              {},              {},              {},                 },
        { __LINE__,  {10,20,"()"},  {},             {},              {},              {},              {},                 },
        { __LINE__,  {10,10,"<"},   {},             {},              {},              {},              {},                 },
        { __LINE__,  {10,10,"<="},  {},             {},              {},              {},              {},                 },
        { __LINE__,  {10,10,">"},   {},             {},              {},              {},              {},                 },
        { __LINE__,  {10,10,">="},  {},             {},              {},              {},              {},                 },
        { __LINE__,  {0,0,"*"},     {0,0,"*"},      {0,0,"*"},       {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10},          {0,0,"*"},      {0,0,"*"},       {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10,20,"[]"},  {0,0,"*"},      {0,0,"*"},       {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10,20,"[)"},  {0,0,"*"},      {0,0,"*"},       {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10,20,"(]"},  {0,0,"*"},      {0,0,"*"},       {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10,20,"()"},  {0,0,"*"},      {0,0,"*"},       {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10,10,"<"},   {0,0,"*"},      {0,0,"*"},       {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10,10,"<="},  {0,0,"*"},      {0,0,"*"},       {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10,10,">"},   {0,0,"*"},      {0,0,"*"},       {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10,10,">="},  {0,0,"*"},      {0,0,"*"},       {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10},          {42},           {52},            {-32},           {32},            {420},              },
        { __LINE__,  {10,20,"[]"},  {42},           {52,62,"[]"},    {-32,-22,"[]"},  {22,32,"[]"},    {420,840,"[]"},     },
        { __LINE__,  {10,20,"[)"},  {42},           {52,62,"[)"},    {-32,-22,"[)"},  {22,32,"(]"},    {420,840,"[)"},     },
        { __LINE__,  {10,20,"(]"},  {42},           {52,62,"(]"},    {-32,-22,"(]"},  {22,32,"[)"},    {420,840,"(]"},     },
        { __LINE__,  {10,20,"()"},  {42},           {52,62,"()"},    {-32,-22,"()"},  {22,32,"()"},    {420,840,"()"},     },
        { __LINE__,  {10,10,"<"},   {42},           {52,52,"<"},     {-32,-32,"<"},   {32,32,">"},     {420,420,"<"},      },
        { __LINE__,  {10,10,"<="},  {42},           {52,52,"<="},    {-32,-32,"<="},  {32,32,">="},    {420,420,"<="},     },
        { __LINE__,  {10,10,">"},   {42},           {52,52,">"},     {-32,-32,">"},   {32,32,"<"},     {420,420,">"},      },
        { __LINE__,  {10,10,">="},  {42},           {52,52,">="},    {-32,-32,">="},  {32,32,"<="},    {420,420,">="},     },
        { __LINE__,  {10,20,"[]"},  {-86,99,"[]"},  {-76,119,"[]"},  {-89,106,"[]"},  {-106,89,"[]"},  {-1720,1980,"[]"},  },
        { __LINE__,  {10,20,"[)"},  {-86,99,"[]"},  {-76,119,"[)"},  {-89,106,"[)"},  {-106,89,"(]"},  {-1720,1980,"()"},  },
        { __LINE__,  {10,20,"(]"},  {-86,99,"[]"},  {-76,119,"(]"},  {-89,106,"(]"},  {-106,89,"[)"},  {-1720,1980,"[]"},  },
        { __LINE__,  {10,20,"()"},  {-86,99,"[]"},  {-76,119,"()"},  {-89,106,"()"},  {-106,89,"()"},  {-1720,1980,"()"},  },
        { __LINE__,  {10,10,"<"},   {-86,99,"[]"},  {109,109,"<"},   {96,96,"<"},     {-96,-96,">"},   {0,0,"*"},          },
        { __LINE__,  {10,10,"<="},  {-86,99,"[]"},  {109,109,"<="},  {96,96,"<="},    {-96,-96,">="},  {0,0,"*"},          },
        { __LINE__,  {10,10,">"},   {-86,99,"[]"},  {-76,-76,">"},   {-89,-89,">"},   {89,89,"<"},     {0,0,"*"},          },
        { __LINE__,  {10,10,">="},  {-86,99,"[]"},  {-76,-76,">="},  {-89,-89,">="},  {89,89,"<="},    {0,0,"*"},          },
        { __LINE__,  {10,20,"[]"},  {-86,99,"[)"},  {-76,119,"[)"},  {-89,106,"(]"},  {-106,89,"[)"},  {-1720,1980,"[)"},  },
        { __LINE__,  {10,20,"[)"},  {-86,99,"[)"},  {-76,119,"[)"},  {-89,106,"()"},  {-106,89,"()"},  {-1720,1980,"()"},  },
        { __LINE__,  {10,20,"(]"},  {-86,99,"[)"},  {-76,119,"()"},  {-89,106,"(]"},  {-106,89,"[)"},  {-1720,1980,"[)"},  },
        { __LINE__,  {10,20,"()"},  {-86,99,"[)"},  {-76,119,"()"},  {-89,106,"()"},  {-106,89,"()"},  {-1720,1980,"()"},  },
        { __LINE__,  {10,10,"<"},   {-86,99,"[)"},  {109,109,"<"},   {96,96,"<"},     {-96,-96,">"},   {0,0,"*"},          },
        { __LINE__,  {10,10,"<="},  {-86,99,"[)"},  {109,109,"<"},   {96,96,"<="},    {-96,-96,">="},  {0,0,"*"},          },
        { __LINE__,  {10,10,">"},   {-86,99,"[)"},  {-76,-76,">"},   {-89,-89,">"},   {89,89,"<"},     {0,0,"*"},          },
        { __LINE__,  {10,10,">="},  {-86,99,"[)"},  {-76,-76,">="},  {-89,-89,">"},   {89,89,"<"},     {0,0,"*"},          },
        { __LINE__,  {10,20,"[]"},  {-86,99,"(]"},  {-76,119,"(]"},  {-89,106,"[)"},  {-106,89,"(]"},  {-1720,1980,"(]"},  },
        { __LINE__,  {10,20,"[)"},  {-86,99,"(]"},  {-76,119,"()"},  {-89,106,"[)"},  {-106,89,"(]"},  {-1720,1980,"()"},  },
        { __LINE__,  {10,20,"(]"},  {-86,99,"(]"},  {-76,119,"(]"},  {-89,106,"()"},  {-106,89,"()"},  {-1720,1980,"(]"},  },
        { __LINE__,  {10,20,"()"},  {-86,99,"(]"},  {-76,119,"()"},  {-89,106,"()"},  {-106,89,"()"},  {-1720,1980,"()"},  },
        { __LINE__,  {10,10,"<"},   {-86,99,"(]"},  {109,109,"<"},   {96,96,"<"},     {-96,-96,">"},   {0,0,"*"},          },
        { __LINE__,  {10,10,"<="},  {-86,99,"(]"},  {109,109,"<="},  {96,96,"<"},     {-96,-96,">"},   {0,0,"*"},          },
        { __LINE__,  {10,10,">"},   {-86,99,"(]"},  {-76,-76,">"},   {-89,-89,">"},   {89,89,"<"},     {0,0,"*"},          },
        { __LINE__,  {10,10,">="},  {-86,99,"(]"},  {-76,-76,">"},   {-89,-89,">="},  {89,89,"<="},    {0,0,"*"},          },
        { __LINE__,  {10,20,"[]"},  {-86,99,"()"},  {-76,119,"()"},  {-89,106,"()"},  {-106,89,"()"},  {-1720,1980,"()"},  },
        { __LINE__,  {10,20,"[)"},  {-86,99,"()"},  {-76,119,"()"},  {-89,106,"()"},  {-106,89,"()"},  {-1720,1980,"()"},  },
        { __LINE__,  {10,20,"(]"},  {-86,99,"()"},  {-76,119,"()"},  {-89,106,"()"},  {-106,89,"()"},  {-1720,1980,"()"},  },
        { __LINE__,  {10,20,"()"},  {-86,99,"()"},  {-76,119,"()"},  {-89,106,"()"},  {-106,89,"()"},  {-1720,1980,"()"},  },
        { __LINE__,  {10,10,"<"},   {-86,99,"()"},  {109,109,"<"},   {96,96,"<"},     {-96,-96,">"},   {0,0,"*"},          },
        { __LINE__,  {10,10,"<="},  {-86,99,"()"},  {109,109,"<"},   {96,96,"<"},     {-96,-96,">"},   {0,0,"*"},          },
        { __LINE__,  {10,10,">"},   {-86,99,"()"},  {-76,-76,">"},   {-89,-89,">"},   {89,89,"<"},     {0,0,"*"},          },
        { __LINE__,  {10,10,">="},  {-86,99,"()"},  {-76,-76,">"},   {-89,-89,">"},   {89,89,"<"},     {0,0,"*"},          },
        { __LINE__,  {10,10,"<"},   {42,42,"<"},    {52,52,"<"},     {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10,10,"<="},  {42,42,"<"},    {52,52,"<"},     {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10,10,">"},   {42,42,"<"},    {0,0,"*"},       {-32,-32,">"},   {32,32,"<"},     {0,0,"*"},          },
        { __LINE__,  {10,10,">="},  {42,42,"<"},    {0,0,"*"},       {-32,-32,">"},   {32,32,"<"},     {0,0,"*"},          },
        { __LINE__,  {10,10,"<="},  {42,42,"<="},   {52,52,"<="},    {0,0,"*"},       {0,0,"*"},       {0,0,"*"},          },
        { __LINE__,  {10,10,">"},   {42,42,"<="},   {0,0,"*"},       {-32,-32,">"},   {32,32,"<"},     {0,0,"*"},          },
        { __LINE__,  {10,10,">="},  {42,42,"<="},   {0,0,"*"},       {-32,-32,">="},  {32,32,"<="},    {0,0,"*"},          },
        { __LINE__,  {10,10,">"},   {42,42,">"},    {52,52,">"},     {0,0,"*"},       {0,0,"*"},       {420,420,">"},      },
        { __LINE__,  {10,10,">="},  {42,42,">"},    {52,52,">"},     {0,0,"*"},       {0,0,"*"},       {420,420,">"},      },
        { __LINE__,  {10,10,">="},  {42,42,">="},   {52,52,">="},    {0,0,"*"},       {0,0,"*"},       {420,420,">="},     },

    };

    Itv a, b, c;

    for (const auto& t: tests) {

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

        if (errors > 0) {
            std::println("... [{}] {} {}", t.line, t.lhs, t.rhs);
        }

    }

}
