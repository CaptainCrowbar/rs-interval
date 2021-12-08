#include "rs-interval.hpp"
#include "test/unit-test.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace RS;
using namespace RS::Intervals;

using IntervalType = Interval<double>;
using SetType = IntervalSet<double>;

void test_rs_continuous_interval_basic_properties() {

    IntervalType in;

    TEST_TYPE(IntervalType::value_type, double);
    TEST_EQUAL(IntervalType::category, IntervalCategory::continuous);
    TEST(in.empty());
    TEST_EQUAL(in.size(), 0u);
    TEST(! in);
    TEST_EQUAL(in.str(), "{}");
    TEST_EQUAL(in.match(0), IntervalMatch::empty);
    TEST_EQUAL(in.match(123.456), IntervalMatch::empty);
    TEST(! in(0));
    TEST(! in(123.456));

}

void test_rs_continuous_interval_construction() {

    IntervalType in;
    std::string str;

    TRY(in = IntervalType());                                                      TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = IntervalType(1.0));                                                   TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = IntervalType(1.0, IntervalBound::closed, IntervalBound::closed));     TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = IntervalType(1.0, IntervalBound::closed, IntervalBound::open));       TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = IntervalType(1.0, IntervalBound::open, IntervalBound::closed));       TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = IntervalType(1.0, IntervalBound::closed, IntervalBound::unbound));    TRY(str = in.str());  TEST_EQUAL(str, ">=1");
    TRY(in = IntervalType(1.0, IntervalBound::unbound, IntervalBound::closed));    TRY(str = in.str());  TEST_EQUAL(str, "<=1");
    TRY(in = IntervalType(1.0, IntervalBound::open, IntervalBound::unbound));      TRY(str = in.str());  TEST_EQUAL(str, ">1");
    TRY(in = IntervalType(1.0, IntervalBound::unbound, IntervalBound::open));      TRY(str = in.str());  TEST_EQUAL(str, "<1");
    TRY(in = IntervalType(1.0, IntervalBound::unbound, IntervalBound::unbound));   TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = IntervalType(1.0, 2.0));                                              TRY(str = in.str());  TEST_EQUAL(str, "[1,2]");
    TRY(in = IntervalType(1.0, 2.0, IntervalBound::closed));                       TRY(str = in.str());  TEST_EQUAL(str, "[1,2]");
    TRY(in = IntervalType(1.0, 2.0, IntervalBound::open));                         TRY(str = in.str());  TEST_EQUAL(str, "(1,2)");
    TRY(in = IntervalType(1.0, 2.0, IntervalBound::closed, IntervalBound::open));  TRY(str = in.str());  TEST_EQUAL(str, "[1,2)");
    TRY(in = IntervalType(1.0, 2.0, IntervalBound::open, IntervalBound::closed));  TRY(str = in.str());  TEST_EQUAL(str, "(1,2]");
    TRY(in = IntervalType(1.0, 2.0, "[]"));                                        TRY(str = in.str());  TEST_EQUAL(str, "[1,2]");
    TRY(in = IntervalType(1.0, 2.0, "()"));                                        TRY(str = in.str());  TEST_EQUAL(str, "(1,2)");
    TRY(in = IntervalType(1.0, 2.0, "[)"));                                        TRY(str = in.str());  TEST_EQUAL(str, "[1,2)");
    TRY(in = IntervalType(1.0, 2.0, "(]"));                                        TRY(str = in.str());  TEST_EQUAL(str, "(1,2]");
    TRY(in = IntervalType(1.0, 2.0, "<"));                                         TRY(str = in.str());  TEST_EQUAL(str, "<2");
    TRY(in = IntervalType(1.0, 2.0, "<="));                                        TRY(str = in.str());  TEST_EQUAL(str, "<=2");
    TRY(in = IntervalType(1.0, 2.0, ">"));                                         TRY(str = in.str());  TEST_EQUAL(str, ">1");
    TRY(in = IntervalType(1.0, 2.0, ">="));                                        TRY(str = in.str());  TEST_EQUAL(str, ">=1");
    TRY(in = IntervalType(1.0, 2.0, "*"));                                         TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = IntervalType(2.0, 1.0));                                              TRY(str = in.str());  TEST_EQUAL(str, "{}");

    TRY(in = make_interval(1.0));                                                   TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = make_interval(1.0, IntervalBound::closed, IntervalBound::closed));     TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = make_interval(1.0, IntervalBound::closed, IntervalBound::open));       TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = make_interval(1.0, IntervalBound::open, IntervalBound::closed));       TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = make_interval(1.0, IntervalBound::closed, IntervalBound::unbound));    TRY(str = in.str());  TEST_EQUAL(str, ">=1");
    TRY(in = make_interval(1.0, IntervalBound::unbound, IntervalBound::closed));    TRY(str = in.str());  TEST_EQUAL(str, "<=1");
    TRY(in = make_interval(1.0, IntervalBound::open, IntervalBound::unbound));      TRY(str = in.str());  TEST_EQUAL(str, ">1");
    TRY(in = make_interval(1.0, IntervalBound::unbound, IntervalBound::open));      TRY(str = in.str());  TEST_EQUAL(str, "<1");
    TRY(in = make_interval(1.0, IntervalBound::unbound, IntervalBound::unbound));   TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = make_interval(1.0, 2.0));                                              TRY(str = in.str());  TEST_EQUAL(str, "[1,2]");
    TRY(in = make_interval(1.0, 2.0, IntervalBound::closed));                       TRY(str = in.str());  TEST_EQUAL(str, "[1,2]");
    TRY(in = make_interval(1.0, 2.0, IntervalBound::open));                         TRY(str = in.str());  TEST_EQUAL(str, "(1,2)");
    TRY(in = make_interval(1.0, 2.0, IntervalBound::closed, IntervalBound::open));  TRY(str = in.str());  TEST_EQUAL(str, "[1,2)");
    TRY(in = make_interval(1.0, 2.0, IntervalBound::open, IntervalBound::closed));  TRY(str = in.str());  TEST_EQUAL(str, "(1,2]");
    TRY(in = make_interval(1.0, 2.0, "[]"));                                        TRY(str = in.str());  TEST_EQUAL(str, "[1,2]");
    TRY(in = make_interval(1.0, 2.0, "()"));                                        TRY(str = in.str());  TEST_EQUAL(str, "(1,2)");
    TRY(in = make_interval(1.0, 2.0, "[)"));                                        TRY(str = in.str());  TEST_EQUAL(str, "[1,2)");
    TRY(in = make_interval(1.0, 2.0, "(]"));                                        TRY(str = in.str());  TEST_EQUAL(str, "(1,2]");
    TRY(in = make_interval(1.0, 2.0, "<"));                                         TRY(str = in.str());  TEST_EQUAL(str, "<2");
    TRY(in = make_interval(1.0, 2.0, "<="));                                        TRY(str = in.str());  TEST_EQUAL(str, "<=2");
    TRY(in = make_interval(1.0, 2.0, ">"));                                         TRY(str = in.str());  TEST_EQUAL(str, ">1");
    TRY(in = make_interval(1.0, 2.0, ">="));                                        TRY(str = in.str());  TEST_EQUAL(str, ">=1");
    TRY(in = make_interval(1.0, 2.0, "*"));                                         TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = make_interval(2.0, 1.0));                                              TRY(str = in.str());  TEST_EQUAL(str, "{}");

    TRY(in = ordered_interval(2.0, 1.0));                                              TRY(str = in.str());  TEST_EQUAL(str, "[1,2]");
    TRY(in = ordered_interval(2.0, 1.0, IntervalBound::closed));                       TRY(str = in.str());  TEST_EQUAL(str, "[1,2]");
    TRY(in = ordered_interval(2.0, 1.0, IntervalBound::open));                         TRY(str = in.str());  TEST_EQUAL(str, "(1,2)");
    TRY(in = ordered_interval(2.0, 1.0, IntervalBound::closed, IntervalBound::open));  TRY(str = in.str());  TEST_EQUAL(str, "(1,2]");
    TRY(in = ordered_interval(2.0, 1.0, IntervalBound::open, IntervalBound::closed));  TRY(str = in.str());  TEST_EQUAL(str, "[1,2)");

}

void test_rs_continuous_interval_inverse() {

    IntervalType in;
    SetType set;
    std::string str;

    TRY((in = {}));                   TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{*}");
    TRY((in = IntervalType::all()));  TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{}");
    TRY((in = 42));                   TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<42,>42}");
    TRY((in = {42,42,"<"}));          TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{>=42}");
    TRY((in = {42,42,"<="}));         TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{>42}");
    TRY((in = {42,42,">"}));          TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<=42}");
    TRY((in = {42,42,">="}));         TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<42}");
    TRY((in = {86,99,"()"}));         TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<=86,>=99}");
    TRY((in = {86,99,"(]"}));         TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<=86,>99}");
    TRY((in = {86,99,"[)"}));         TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<86,>=99}");
    TRY((in = {86,99,"[]"}));         TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<86,>99}");

}

void test_rs_continuous_interval_binary_operations() {

    struct test_info {
        int line;
        IntervalType lhs;
        IntervalType rhs;
        int compare;
        bool includes;
        bool overlaps;
        bool touches;
        std::string envelope;
        std::string set_intersection;
        std::string set_union;
        std::string set_difference;
        std::string set_symmetric_difference;
    };

    static const std::vector<test_info> tests = {

        // line      lhs          rhs          cmp  incl    over    touch   envel     inter     union             diff              symm              --
        { __LINE__,  {},          {},          0,   false,  false,  false,  "{}",     "{}",     "{}",             "{}",             "{}",             },
        { __LINE__,  {0,0,"*"},   {},          1,   false,  false,  false,  "*",      "{}",     "{*}",            "{*}",            "{*}",            },
        { __LINE__,  {0,0,"*"},   {0,0,"*"},   0,   true,   true,   true,   "*",      "*",      "{*}",            "{}",             "{}",             },
        { __LINE__,  {2},         {},          1,   false,  false,  false,  "2",      "{}",     "{2}",            "{2}",            "{2}",            },
        { __LINE__,  {2},         {0,0,"*"},   1,   false,  true,   true,   "*",      "2",      "{*}",            "{}",             "{<2,>2}",        },
        { __LINE__,  {2},         {1},         1,   false,  false,  false,  "[1,2]",  "{}",     "{1,2}",          "{2}",            "{1,2}",          },
        { __LINE__,  {2},         {2},         0,   true,   true,   true,   "2",      "2",      "{2}",            "{}",             "{}",             },
        { __LINE__,  {2},         {3},         -1,  false,  false,  false,  "[2,3]",  "{}",     "{2,3}",          "{2}",            "{2,3}",          },
        { __LINE__,  {2,2,"<"},   {},          1,   false,  false,  false,  "<2",     "{}",     "{<2}",           "{<2}",           "{<2}",           },
        { __LINE__,  {2,2,"<"},   {0,0,"*"},   -1,  false,  true,   true,   "*",      "<2",     "{*}",            "{}",             "{>=2}",          },
        { __LINE__,  {2,2,"<"},   {1},         -1,  true,   true,   true,   "<2",     "1",      "{<2}",           "{<1,(1,2)}",     "{<1,(1,2)}",     },
        { __LINE__,  {2,2,"<"},   {2},         -1,  false,  false,  true,   "<=2",    "{}",     "{<=2}",          "{<2}",           "{<=2}",          },
        { __LINE__,  {2,2,"<"},   {3},         -1,  false,  false,  false,  "<=3",    "{}",     "{<2,3}",         "{<2}",           "{<2,3}",         },
        { __LINE__,  {2,2,"<"},   {1,1,"<"},   1,   true,   true,   true,   "<2",     "<1",     "{<2}",           "{[1,2)}",        "{[1,2)}",        },
        { __LINE__,  {2,2,"<"},   {2,2,"<"},   0,   true,   true,   true,   "<2",     "<2",     "{<2}",           "{}",             "{}",             },
        { __LINE__,  {2,2,"<"},   {3,3,"<"},   -1,  false,  true,   true,   "<3",     "<2",     "{<3}",           "{}",             "{[2,3)}",        },
        { __LINE__,  {2,2,"<="},  {},          1,   false,  false,  false,  "<=2",    "{}",     "{<=2}",          "{<=2}",          "{<=2}",          },
        { __LINE__,  {2,2,"<="},  {0,0,"*"},   -1,  false,  true,   true,   "*",      "<=2",    "{*}",            "{}",             "{>2}",           },
        { __LINE__,  {2,2,"<="},  {1},         -1,  true,   true,   true,   "<=2",    "1",      "{<=2}",          "{<1,(1,2]}",     "{<1,(1,2]}",     },
        { __LINE__,  {2,2,"<="},  {2},         -1,  true,   true,   true,   "<=2",    "2",      "{<=2}",          "{<2}",           "{<2}",           },
        { __LINE__,  {2,2,"<="},  {3},         -1,  false,  false,  false,  "<=3",    "{}",     "{<=2,3}",        "{<=2}",          "{<=2,3}",        },
        { __LINE__,  {2,2,"<="},  {1,1,"<"},   1,   true,   true,   true,   "<=2",    "<1",     "{<=2}",          "{[1,2]}",        "{[1,2]}",        },
        { __LINE__,  {2,2,"<="},  {2,2,"<"},   1,   true,   true,   true,   "<=2",    "<2",     "{<=2}",          "{2}",            "{2}",            },
        { __LINE__,  {2,2,"<="},  {3,3,"<"},   -1,  false,  true,   true,   "<3",     "<=2",    "{<3}",           "{}",             "{(2,3)}",        },
        { __LINE__,  {2,2,"<="},  {1,1,"<="},  1,   true,   true,   true,   "<=2",    "<=1",    "{<=2}",          "{(1,2]}",        "{(1,2]}",        },
        { __LINE__,  {2,2,"<="},  {2,2,"<="},  0,   true,   true,   true,   "<=2",    "<=2",    "{<=2}",          "{}",             "{}",             },
        { __LINE__,  {2,2,"<="},  {3,3,"<="},  -1,  false,  true,   true,   "<=3",    "<=2",    "{<=3}",          "{}",             "{(2,3]}",        },
        { __LINE__,  {2,2,">"},   {},          1,   false,  false,  false,  ">2",     "{}",     "{>2}",           "{>2}",           "{>2}",           },
        { __LINE__,  {2,2,">"},   {0,0,"*"},   1,   false,  true,   true,   "*",      ">2",     "{*}",            "{}",             "{<=2}",          },
        { __LINE__,  {2,2,">"},   {1},         1,   false,  false,  false,  ">=1",    "{}",     "{1,>2}",         "{>2}",           "{1,>2}",         },
        { __LINE__,  {2,2,">"},   {2},         1,   false,  false,  true,   ">=2",    "{}",     "{>=2}",          "{>2}",           "{>=2}",          },
        { __LINE__,  {2,2,">"},   {3},         -1,  true,   true,   true,   ">2",     "3",      "{>2}",           "{(2,3),>3}",     "{(2,3),>3}",     },
        { __LINE__,  {2,2,">"},   {1,1,"<"},   1,   false,  false,  false,  "*",      "{}",     "{<1,>2}",        "{>2}",           "{<1,>2}",        },
        { __LINE__,  {2,2,">"},   {2,2,"<"},   1,   false,  false,  false,  "*",      "{}",     "{<2,>2}",        "{>2}",           "{<2,>2}",        },
        { __LINE__,  {2,2,">"},   {3,3,"<"},   1,   false,  true,   true,   "*",      "(2,3)",  "{*}",            "{>=3}",          "{<=2,>=3}",      },
        { __LINE__,  {2,2,">"},   {1,1,"<="},  1,   false,  false,  false,  "*",      "{}",     "{<=1,>2}",       "{>2}",           "{<=1,>2}",       },
        { __LINE__,  {2,2,">"},   {2,2,"<="},  1,   false,  false,  true,   "*",      "{}",     "{*}",            "{>2}",           "{*}",            },
        { __LINE__,  {2,2,">"},   {3,3,"<="},  1,   false,  true,   true,   "*",      "(2,3]",  "{*}",            "{>3}",           "{<=2,>3}",       },
        { __LINE__,  {2,2,">"},   {1,1,">"},   1,   false,  true,   true,   ">1",     ">2",     "{>1}",           "{}",             "{(1,2]}",        },
        { __LINE__,  {2,2,">"},   {2,2,">"},   0,   true,   true,   true,   ">2",     ">2",     "{>2}",           "{}",             "{}",             },
        { __LINE__,  {2,2,">"},   {3,3,">"},   -1,  true,   true,   true,   ">2",     ">3",     "{>2}",           "{(2,3]}",        "{(2,3]}",        },
        { __LINE__,  {2,2,">="},  {},          1,   false,  false,  false,  ">=2",    "{}",     "{>=2}",          "{>=2}",          "{>=2}",          },
        { __LINE__,  {2,2,">="},  {0,0,"*"},   1,   false,  true,   true,   "*",      ">=2",    "{*}",            "{}",             "{<2}",           },
        { __LINE__,  {2,2,">="},  {1},         1,   false,  false,  false,  ">=1",    "{}",     "{1,>=2}",        "{>=2}",          "{1,>=2}",        },
        { __LINE__,  {2,2,">="},  {2},         1,   true,   true,   true,   ">=2",    "2",      "{>=2}",          "{>2}",           "{>2}",           },
        { __LINE__,  {2,2,">="},  {3},         -1,  true,   true,   true,   ">=2",    "3",      "{>=2}",          "{[2,3),>3}",     "{[2,3),>3}",     },
        { __LINE__,  {2,2,">="},  {1,1,"<"},   1,   false,  false,  false,  "*",      "{}",     "{<1,>=2}",       "{>=2}",          "{<1,>=2}",       },
        { __LINE__,  {2,2,">="},  {2,2,"<"},   1,   false,  false,  true,   "*",      "{}",     "{*}",            "{>=2}",          "{*}",            },
        { __LINE__,  {2,2,">="},  {3,3,"<"},   1,   false,  true,   true,   "*",      "[2,3)",  "{*}",            "{>=3}",          "{<2,>=3}",       },
        { __LINE__,  {2,2,">="},  {1,1,"<="},  1,   false,  false,  false,  "*",      "{}",     "{<=1,>=2}",      "{>=2}",          "{<=1,>=2}",      },
        { __LINE__,  {2,2,">="},  {2,2,"<="},  1,   false,  true,   true,   "*",      "2",      "{*}",            "{>2}",           "{<2,>2}",        },
        { __LINE__,  {2,2,">="},  {3,3,"<="},  1,   false,  true,   true,   "*",      "[2,3]",  "{*}",            "{>3}",           "{<2,>3}",        },
        { __LINE__,  {2,2,">="},  {1,1,">"},   1,   false,  true,   true,   ">1",     ">=2",    "{>1}",           "{}",             "{(1,2)}",        },
        { __LINE__,  {2,2,">="},  {2,2,">"},   -1,  true,   true,   true,   ">=2",    ">2",     "{>=2}",          "{2}",            "{2}",            },
        { __LINE__,  {2,2,">="},  {3,3,">"},   -1,  true,   true,   true,   ">=2",    ">3",     "{>=2}",          "{[2,3]}",        "{[2,3]}",        },
        { __LINE__,  {2,2,">="},  {1,1,">="},  1,   false,  true,   true,   ">=1",    ">=2",    "{>=1}",          "{}",             "{[1,2)}",        },
        { __LINE__,  {2,2,">="},  {2,2,">="},  0,   true,   true,   true,   ">=2",    ">=2",    "{>=2}",          "{}",             "{}",             },
        { __LINE__,  {2,2,">="},  {3,3,">="},  -1,  true,   true,   true,   ">=2",    ">=3",    "{>=2}",          "{[2,3)}",        "{[2,3)}",        },
        { __LINE__,  {2,4,"()"},  {},          1,   false,  false,  false,  "(2,4)",  "{}",     "{(2,4)}",        "{(2,4)}",        "{(2,4)}",        },
        { __LINE__,  {2,4,"()"},  {0,0,"*"},   1,   false,  true,   true,   "*",      "(2,4)",  "{*}",            "{}",             "{<=2,>=4}",      },
        { __LINE__,  {2,4,"()"},  {1},         1,   false,  false,  false,  "[1,4)",  "{}",     "{1,(2,4)}",      "{(2,4)}",        "{1,(2,4)}",      },
        { __LINE__,  {2,4,"()"},  {2},         1,   false,  false,  true,   "[2,4)",  "{}",     "{[2,4)}",        "{(2,4)}",        "{[2,4)}",        },
        { __LINE__,  {2,4,"()"},  {3},         -1,  true,   true,   true,   "(2,4)",  "3",      "{(2,4)}",        "{(2,3),(3,4)}",  "{(2,3),(3,4)}",  },
        { __LINE__,  {2,4,"()"},  {4},         -1,  false,  false,  true,   "(2,4]",  "{}",     "{(2,4]}",        "{(2,4)}",        "{(2,4]}",        },
        { __LINE__,  {2,4,"()"},  {5},         -1,  false,  false,  false,  "(2,5]",  "{}",     "{(2,4),5}",      "{(2,4)}",        "{(2,4),5}",      },
        { __LINE__,  {2,4,"()"},  {1,1,"<"},   1,   false,  false,  false,  "<4",     "{}",     "{<1,(2,4)}",     "{(2,4)}",        "{<1,(2,4)}",     },
        { __LINE__,  {2,4,"()"},  {2,2,"<"},   1,   false,  false,  false,  "<4",     "{}",     "{<2,(2,4)}",     "{(2,4)}",        "{<2,(2,4)}",     },
        { __LINE__,  {2,4,"()"},  {3,3,"<"},   1,   false,  true,   true,   "<4",     "(2,3)",  "{<4}",           "{[3,4)}",        "{<=2,[3,4)}",    },
        { __LINE__,  {2,4,"()"},  {4,4,"<"},   1,   false,  true,   true,   "<4",     "(2,4)",  "{<4}",           "{}",             "{<=2}",          },
        { __LINE__,  {2,4,"()"},  {5,5,"<"},   1,   false,  true,   true,   "<5",     "(2,4)",  "{<5}",           "{}",             "{<=2,[4,5)}",    },
        { __LINE__,  {2,4,"()"},  {1,1,"<="},  1,   false,  false,  false,  "<4",     "{}",     "{<=1,(2,4)}",    "{(2,4)}",        "{<=1,(2,4)}",    },
        { __LINE__,  {2,4,"()"},  {2,2,"<="},  1,   false,  false,  true,   "<4",     "{}",     "{<4}",           "{(2,4)}",        "{<4}",           },
        { __LINE__,  {2,4,"()"},  {3,3,"<="},  1,   false,  true,   true,   "<4",     "(2,3]",  "{<4}",           "{(3,4)}",        "{<=2,(3,4)}",    },
        { __LINE__,  {2,4,"()"},  {4,4,"<="},  1,   false,  true,   true,   "<=4",    "(2,4)",  "{<=4}",          "{}",             "{<=2,4}",        },
        { __LINE__,  {2,4,"()"},  {5,5,"<="},  1,   false,  true,   true,   "<=5",    "(2,4)",  "{<=5}",          "{}",             "{<=2,[4,5]}",    },
        { __LINE__,  {2,4,"()"},  {1,1,">"},   1,   false,  true,   true,   ">1",     "(2,4)",  "{>1}",           "{}",             "{(1,2],>=4}",    },
        { __LINE__,  {2,4,"()"},  {2,2,">"},   -1,  false,  true,   true,   ">2",     "(2,4)",  "{>2}",           "{}",             "{>=4}",          },
        { __LINE__,  {2,4,"()"},  {3,3,">"},   -1,  false,  true,   true,   ">2",     "(3,4)",  "{>2}",           "{(2,3]}",        "{(2,3],>=4}",    },
        { __LINE__,  {2,4,"()"},  {4,4,">"},   -1,  false,  false,  false,  ">2",     "{}",     "{(2,4),>4}",     "{(2,4)}",        "{(2,4),>4}",     },
        { __LINE__,  {2,4,"()"},  {5,5,">"},   -1,  false,  false,  false,  ">2",     "{}",     "{(2,4),>5}",     "{(2,4)}",        "{(2,4),>5}",     },
        { __LINE__,  {2,4,"()"},  {1,1,">="},  1,   false,  true,   true,   ">=1",    "(2,4)",  "{>=1}",          "{}",             "{[1,2],>=4}",    },
        { __LINE__,  {2,4,"()"},  {2,2,">="},  1,   false,  true,   true,   ">=2",    "(2,4)",  "{>=2}",          "{}",             "{2,>=4}",        },
        { __LINE__,  {2,4,"()"},  {3,3,">="},  -1,  false,  true,   true,   ">2",     "[3,4)",  "{>2}",           "{(2,3)}",        "{(2,3),>=4}",    },
        { __LINE__,  {2,4,"()"},  {4,4,">="},  -1,  false,  false,  true,   ">2",     "{}",     "{>2}",           "{(2,4)}",        "{>2}",           },
        { __LINE__,  {2,4,"()"},  {5,5,">="},  -1,  false,  false,  false,  ">2",     "{}",     "{(2,4),>=5}",    "{(2,4)}",        "{(2,4),>=5}",    },
        { __LINE__,  {2,4,"()"},  {1,2,"()"},  1,   false,  false,  false,  "(1,4)",  "{}",     "{(1,2),(2,4)}",  "{(2,4)}",        "{(1,2),(2,4)}",  },
        { __LINE__,  {2,4,"()"},  {1,3,"()"},  1,   false,  true,   true,   "(1,4)",  "(2,3)",  "{(1,4)}",        "{[3,4)}",        "{(1,2],[3,4)}",  },
        { __LINE__,  {2,4,"()"},  {1,4,"()"},  1,   false,  true,   true,   "(1,4)",  "(2,4)",  "{(1,4)}",        "{}",             "{(1,2]}",        },
        { __LINE__,  {2,4,"()"},  {1,5,"()"},  1,   false,  true,   true,   "(1,5)",  "(2,4)",  "{(1,5)}",        "{}",             "{(1,2],[4,5)}",  },
        { __LINE__,  {2,4,"()"},  {2,3,"()"},  1,   true,   true,   true,   "(2,4)",  "(2,3)",  "{(2,4)}",        "{[3,4)}",        "{[3,4)}",        },
        { __LINE__,  {2,4,"()"},  {2,4,"()"},  0,   true,   true,   true,   "(2,4)",  "(2,4)",  "{(2,4)}",        "{}",             "{}",             },
        { __LINE__,  {2,4,"()"},  {2,5,"()"},  -1,  false,  true,   true,   "(2,5)",  "(2,4)",  "{(2,5)}",        "{}",             "{[4,5)}",        },
        { __LINE__,  {2,4,"()"},  {3,4,"()"},  -1,  true,   true,   true,   "(2,4)",  "(3,4)",  "{(2,4)}",        "{(2,3]}",        "{(2,3]}",        },
        { __LINE__,  {2,4,"()"},  {3,5,"()"},  -1,  false,  true,   true,   "(2,5)",  "(3,4)",  "{(2,5)}",        "{(2,3]}",        "{(2,3],[4,5)}",  },
        { __LINE__,  {2,4,"()"},  {4,5,"()"},  -1,  false,  false,  false,  "(2,5)",  "{}",     "{(2,4),(4,5)}",  "{(2,4)}",        "{(2,4),(4,5)}",  },
        { __LINE__,  {2,4,"[)"},  {},          1,   false,  false,  false,  "[2,4)",  "{}",     "{[2,4)}",        "{[2,4)}",        "{[2,4)}",        },
        { __LINE__,  {2,4,"[)"},  {0,0,"*"},   1,   false,  true,   true,   "*",      "[2,4)",  "{*}",            "{}",             "{<2,>=4}",       },
        { __LINE__,  {2,4,"[)"},  {1},         1,   false,  false,  false,  "[1,4)",  "{}",     "{1,[2,4)}",      "{[2,4)}",        "{1,[2,4)}",      },
        { __LINE__,  {2,4,"[)"},  {2},         1,   true,   true,   true,   "[2,4)",  "2",      "{[2,4)}",        "{(2,4)}",        "{(2,4)}",        },
        { __LINE__,  {2,4,"[)"},  {3},         -1,  true,   true,   true,   "[2,4)",  "3",      "{[2,4)}",        "{[2,3),(3,4)}",  "{[2,3),(3,4)}",  },
        { __LINE__,  {2,4,"[)"},  {4},         -1,  false,  false,  true,   "[2,4]",  "{}",     "{[2,4]}",        "{[2,4)}",        "{[2,4]}",        },
        { __LINE__,  {2,4,"[)"},  {5},         -1,  false,  false,  false,  "[2,5]",  "{}",     "{[2,4),5}",      "{[2,4)}",        "{[2,4),5}",      },
        { __LINE__,  {2,4,"[)"},  {1,1,"<"},   1,   false,  false,  false,  "<4",     "{}",     "{<1,[2,4)}",     "{[2,4)}",        "{<1,[2,4)}",     },
        { __LINE__,  {2,4,"[)"},  {2,2,"<"},   1,   false,  false,  true,   "<4",     "{}",     "{<4}",           "{[2,4)}",        "{<4}",           },
        { __LINE__,  {2,4,"[)"},  {3,3,"<"},   1,   false,  true,   true,   "<4",     "[2,3)",  "{<4}",           "{[3,4)}",        "{<2,[3,4)}",     },
        { __LINE__,  {2,4,"[)"},  {4,4,"<"},   1,   false,  true,   true,   "<4",     "[2,4)",  "{<4}",           "{}",             "{<2}",           },
        { __LINE__,  {2,4,"[)"},  {5,5,"<"},   1,   false,  true,   true,   "<5",     "[2,4)",  "{<5}",           "{}",             "{<2,[4,5)}",     },
        { __LINE__,  {2,4,"[)"},  {1,1,"<="},  1,   false,  false,  false,  "<4",     "{}",     "{<=1,[2,4)}",    "{[2,4)}",        "{<=1,[2,4)}",    },
        { __LINE__,  {2,4,"[)"},  {2,2,"<="},  1,   false,  true,   true,   "<4",     "2",      "{<4}",           "{(2,4)}",        "{<2,(2,4)}",     },
        { __LINE__,  {2,4,"[)"},  {3,3,"<="},  1,   false,  true,   true,   "<4",     "[2,3]",  "{<4}",           "{(3,4)}",        "{<2,(3,4)}",     },
        { __LINE__,  {2,4,"[)"},  {4,4,"<="},  1,   false,  true,   true,   "<=4",    "[2,4)",  "{<=4}",          "{}",             "{<2,4}",         },
        { __LINE__,  {2,4,"[)"},  {5,5,"<="},  1,   false,  true,   true,   "<=5",    "[2,4)",  "{<=5}",          "{}",             "{<2,[4,5]}",     },
        { __LINE__,  {2,4,"[)"},  {1,1,">"},   1,   false,  true,   true,   ">1",     "[2,4)",  "{>1}",           "{}",             "{(1,2),>=4}",    },
        { __LINE__,  {2,4,"[)"},  {2,2,">"},   -1,  false,  true,   true,   ">=2",    "(2,4)",  "{>=2}",          "{2}",            "{2,>=4}",        },
        { __LINE__,  {2,4,"[)"},  {3,3,">"},   -1,  false,  true,   true,   ">=2",    "(3,4)",  "{>=2}",          "{[2,3]}",        "{[2,3],>=4}",    },
        { __LINE__,  {2,4,"[)"},  {4,4,">"},   -1,  false,  false,  false,  ">=2",    "{}",     "{[2,4),>4}",     "{[2,4)}",        "{[2,4),>4}",     },
        { __LINE__,  {2,4,"[)"},  {5,5,">"},   -1,  false,  false,  false,  ">=2",    "{}",     "{[2,4),>5}",     "{[2,4)}",        "{[2,4),>5}",     },
        { __LINE__,  {2,4,"[)"},  {1,1,">="},  1,   false,  true,   true,   ">=1",    "[2,4)",  "{>=1}",          "{}",             "{[1,2),>=4}",    },
        { __LINE__,  {2,4,"[)"},  {2,2,">="},  -1,  false,  true,   true,   ">=2",    "[2,4)",  "{>=2}",          "{}",             "{>=4}",          },
        { __LINE__,  {2,4,"[)"},  {3,3,">="},  -1,  false,  true,   true,   ">=2",    "[3,4)",  "{>=2}",          "{[2,3)}",        "{[2,3),>=4}",    },
        { __LINE__,  {2,4,"[)"},  {4,4,">="},  -1,  false,  false,  true,   ">=2",    "{}",     "{>=2}",          "{[2,4)}",        "{>=2}",          },
        { __LINE__,  {2,4,"[)"},  {5,5,">="},  -1,  false,  false,  false,  ">=2",    "{}",     "{[2,4),>=5}",    "{[2,4)}",        "{[2,4),>=5}",    },
        { __LINE__,  {2,4,"[)"},  {1,2,"()"},  1,   false,  false,  true,   "(1,4)",  "{}",     "{(1,4)}",        "{[2,4)}",        "{(1,4)}",        },
        { __LINE__,  {2,4,"[)"},  {1,3,"()"},  1,   false,  true,   true,   "(1,4)",  "[2,3)",  "{(1,4)}",        "{[3,4)}",        "{(1,2),[3,4)}",  },
        { __LINE__,  {2,4,"[)"},  {1,4,"()"},  1,   false,  true,   true,   "(1,4)",  "[2,4)",  "{(1,4)}",        "{}",             "{(1,2)}",        },
        { __LINE__,  {2,4,"[)"},  {1,5,"()"},  1,   false,  true,   true,   "(1,5)",  "[2,4)",  "{(1,5)}",        "{}",             "{(1,2),[4,5)}",  },
        { __LINE__,  {2,4,"[)"},  {2,3,"()"},  -1,  true,   true,   true,   "[2,4)",  "(2,3)",  "{[2,4)}",        "{2,[3,4)}",      "{2,[3,4)}",      },
        { __LINE__,  {2,4,"[)"},  {2,4,"()"},  -1,  true,   true,   true,   "[2,4)",  "(2,4)",  "{[2,4)}",        "{2}",            "{2}",            },
        { __LINE__,  {2,4,"[)"},  {2,5,"()"},  -1,  false,  true,   true,   "[2,5)",  "(2,4)",  "{[2,5)}",        "{2}",            "{2,[4,5)}",      },
        { __LINE__,  {2,4,"[)"},  {3,4,"()"},  -1,  true,   true,   true,   "[2,4)",  "(3,4)",  "{[2,4)}",        "{[2,3]}",        "{[2,3]}",        },
        { __LINE__,  {2,4,"[)"},  {3,5,"()"},  -1,  false,  true,   true,   "[2,5)",  "(3,4)",  "{[2,5)}",        "{[2,3]}",        "{[2,3],[4,5)}",  },
        { __LINE__,  {2,4,"[)"},  {4,5,"()"},  -1,  false,  false,  false,  "[2,5)",  "{}",     "{[2,4),(4,5)}",  "{[2,4)}",        "{[2,4),(4,5)}",  },
        { __LINE__,  {2,4,"[)"},  {1,2,"[)"},  1,   false,  false,  true,   "[1,4)",  "{}",     "{[1,4)}",        "{[2,4)}",        "{[1,4)}",        },
        { __LINE__,  {2,4,"[)"},  {1,3,"[)"},  1,   false,  true,   true,   "[1,4)",  "[2,3)",  "{[1,4)}",        "{[3,4)}",        "{[1,2),[3,4)}",  },
        { __LINE__,  {2,4,"[)"},  {1,4,"[)"},  1,   false,  true,   true,   "[1,4)",  "[2,4)",  "{[1,4)}",        "{}",             "{[1,2)}",        },
        { __LINE__,  {2,4,"[)"},  {1,5,"[)"},  1,   false,  true,   true,   "[1,5)",  "[2,4)",  "{[1,5)}",        "{}",             "{[1,2),[4,5)}",  },
        { __LINE__,  {2,4,"[)"},  {2,3,"[)"},  1,   true,   true,   true,   "[2,4)",  "[2,3)",  "{[2,4)}",        "{[3,4)}",        "{[3,4)}",        },
        { __LINE__,  {2,4,"[)"},  {2,4,"[)"},  0,   true,   true,   true,   "[2,4)",  "[2,4)",  "{[2,4)}",        "{}",             "{}",             },
        { __LINE__,  {2,4,"[)"},  {2,5,"[)"},  -1,  false,  true,   true,   "[2,5)",  "[2,4)",  "{[2,5)}",        "{}",             "{[4,5)}",        },
        { __LINE__,  {2,4,"[)"},  {3,4,"[)"},  -1,  true,   true,   true,   "[2,4)",  "[3,4)",  "{[2,4)}",        "{[2,3)}",        "{[2,3)}",        },
        { __LINE__,  {2,4,"[)"},  {3,5,"[)"},  -1,  false,  true,   true,   "[2,5)",  "[3,4)",  "{[2,5)}",        "{[2,3)}",        "{[2,3),[4,5)}",  },
        { __LINE__,  {2,4,"[)"},  {4,5,"[)"},  -1,  false,  false,  true,   "[2,5)",  "{}",     "{[2,5)}",        "{[2,4)}",        "{[2,5)}",        },
        { __LINE__,  {2,4,"(]"},  {},          1,   false,  false,  false,  "(2,4]",  "{}",     "{(2,4]}",        "{(2,4]}",        "{(2,4]}",        },
        { __LINE__,  {2,4,"(]"},  {0,0,"*"},   1,   false,  true,   true,   "*",      "(2,4]",  "{*}",            "{}",             "{<=2,>4}",       },
        { __LINE__,  {2,4,"(]"},  {1},         1,   false,  false,  false,  "[1,4]",  "{}",     "{1,(2,4]}",      "{(2,4]}",        "{1,(2,4]}",      },
        { __LINE__,  {2,4,"(]"},  {2},         1,   false,  false,  true,   "[2,4]",  "{}",     "{[2,4]}",        "{(2,4]}",        "{[2,4]}",        },
        { __LINE__,  {2,4,"(]"},  {3},         -1,  true,   true,   true,   "(2,4]",  "3",      "{(2,4]}",        "{(2,3),(3,4]}",  "{(2,3),(3,4]}",  },
        { __LINE__,  {2,4,"(]"},  {4},         -1,  true,   true,   true,   "(2,4]",  "4",      "{(2,4]}",        "{(2,4)}",        "{(2,4)}",        },
        { __LINE__,  {2,4,"(]"},  {5},         -1,  false,  false,  false,  "(2,5]",  "{}",     "{(2,4],5}",      "{(2,4]}",        "{(2,4],5}",      },
        { __LINE__,  {2,4,"(]"},  {1,1,"<"},   1,   false,  false,  false,  "<=4",    "{}",     "{<1,(2,4]}",     "{(2,4]}",        "{<1,(2,4]}",     },
        { __LINE__,  {2,4,"(]"},  {2,2,"<"},   1,   false,  false,  false,  "<=4",    "{}",     "{<2,(2,4]}",     "{(2,4]}",        "{<2,(2,4]}",     },
        { __LINE__,  {2,4,"(]"},  {3,3,"<"},   1,   false,  true,   true,   "<=4",    "(2,3)",  "{<=4}",          "{[3,4]}",        "{<=2,[3,4]}",    },
        { __LINE__,  {2,4,"(]"},  {4,4,"<"},   1,   false,  true,   true,   "<=4",    "(2,4)",  "{<=4}",          "{4}",            "{<=2,4}",        },
        { __LINE__,  {2,4,"(]"},  {5,5,"<"},   1,   false,  true,   true,   "<5",     "(2,4]",  "{<5}",           "{}",             "{<=2,(4,5)}",    },
        { __LINE__,  {2,4,"(]"},  {1,1,"<="},  1,   false,  false,  false,  "<=4",    "{}",     "{<=1,(2,4]}",    "{(2,4]}",        "{<=1,(2,4]}",    },
        { __LINE__,  {2,4,"(]"},  {2,2,"<="},  1,   false,  false,  true,   "<=4",    "{}",     "{<=4}",          "{(2,4]}",        "{<=4}",          },
        { __LINE__,  {2,4,"(]"},  {3,3,"<="},  1,   false,  true,   true,   "<=4",    "(2,3]",  "{<=4}",          "{(3,4]}",        "{<=2,(3,4]}",    },
        { __LINE__,  {2,4,"(]"},  {4,4,"<="},  1,   false,  true,   true,   "<=4",    "(2,4]",  "{<=4}",          "{}",             "{<=2}",          },
        { __LINE__,  {2,4,"(]"},  {5,5,"<="},  1,   false,  true,   true,   "<=5",    "(2,4]",  "{<=5}",          "{}",             "{<=2,(4,5]}",    },
        { __LINE__,  {2,4,"(]"},  {1,1,">"},   1,   false,  true,   true,   ">1",     "(2,4]",  "{>1}",           "{}",             "{(1,2],>4}",     },
        { __LINE__,  {2,4,"(]"},  {2,2,">"},   -1,  false,  true,   true,   ">2",     "(2,4]",  "{>2}",           "{}",             "{>4}",           },
        { __LINE__,  {2,4,"(]"},  {3,3,">"},   -1,  false,  true,   true,   ">2",     "(3,4]",  "{>2}",           "{(2,3]}",        "{(2,3],>4}",     },
        { __LINE__,  {2,4,"(]"},  {4,4,">"},   -1,  false,  false,  true,   ">2",     "{}",     "{>2}",           "{(2,4]}",        "{>2}",           },
        { __LINE__,  {2,4,"(]"},  {5,5,">"},   -1,  false,  false,  false,  ">2",     "{}",     "{(2,4],>5}",     "{(2,4]}",        "{(2,4],>5}",     },
        { __LINE__,  {2,4,"(]"},  {1,1,">="},  1,   false,  true,   true,   ">=1",    "(2,4]",  "{>=1}",          "{}",             "{[1,2],>4}",     },
        { __LINE__,  {2,4,"(]"},  {2,2,">="},  1,   false,  true,   true,   ">=2",    "(2,4]",  "{>=2}",          "{}",             "{2,>4}",         },
        { __LINE__,  {2,4,"(]"},  {3,3,">="},  -1,  false,  true,   true,   ">2",     "[3,4]",  "{>2}",           "{(2,3)}",        "{(2,3),>4}",     },
        { __LINE__,  {2,4,"(]"},  {4,4,">="},  -1,  false,  true,   true,   ">2",     "4",      "{>2}",           "{(2,4)}",        "{(2,4),>4}",     },
        { __LINE__,  {2,4,"(]"},  {5,5,">="},  -1,  false,  false,  false,  ">2",     "{}",     "{(2,4],>=5}",    "{(2,4]}",        "{(2,4],>=5}",    },
        { __LINE__,  {2,4,"(]"},  {1,2,"()"},  1,   false,  false,  false,  "(1,4]",  "{}",     "{(1,2),(2,4]}",  "{(2,4]}",        "{(1,2),(2,4]}",  },
        { __LINE__,  {2,4,"(]"},  {1,3,"()"},  1,   false,  true,   true,   "(1,4]",  "(2,3)",  "{(1,4]}",        "{[3,4]}",        "{(1,2],[3,4]}",  },
        { __LINE__,  {2,4,"(]"},  {1,4,"()"},  1,   false,  true,   true,   "(1,4]",  "(2,4)",  "{(1,4]}",        "{4}",            "{(1,2],4}",      },
        { __LINE__,  {2,4,"(]"},  {1,5,"()"},  1,   false,  true,   true,   "(1,5)",  "(2,4]",  "{(1,5)}",        "{}",             "{(1,2],(4,5)}",  },
        { __LINE__,  {2,4,"(]"},  {2,3,"()"},  1,   true,   true,   true,   "(2,4]",  "(2,3)",  "{(2,4]}",        "{[3,4]}",        "{[3,4]}",        },
        { __LINE__,  {2,4,"(]"},  {2,4,"()"},  1,   true,   true,   true,   "(2,4]",  "(2,4)",  "{(2,4]}",        "{4}",            "{4}",            },
        { __LINE__,  {2,4,"(]"},  {2,5,"()"},  -1,  false,  true,   true,   "(2,5)",  "(2,4]",  "{(2,5)}",        "{}",             "{(4,5)}",        },
        { __LINE__,  {2,4,"(]"},  {3,4,"()"},  -1,  true,   true,   true,   "(2,4]",  "(3,4)",  "{(2,4]}",        "{(2,3],4}",      "{(2,3],4}",      },
        { __LINE__,  {2,4,"(]"},  {3,5,"()"},  -1,  false,  true,   true,   "(2,5)",  "(3,4]",  "{(2,5)}",        "{(2,3]}",        "{(2,3],(4,5)}",  },
        { __LINE__,  {2,4,"(]"},  {4,5,"()"},  -1,  false,  false,  true,   "(2,5)",  "{}",     "{(2,5)}",        "{(2,4]}",        "{(2,5)}",        },
        { __LINE__,  {2,4,"(]"},  {1,2,"[)"},  1,   false,  false,  false,  "[1,4]",  "{}",     "{[1,2),(2,4]}",  "{(2,4]}",        "{[1,2),(2,4]}",  },
        { __LINE__,  {2,4,"(]"},  {1,3,"[)"},  1,   false,  true,   true,   "[1,4]",  "(2,3)",  "{[1,4]}",        "{[3,4]}",        "{[1,2],[3,4]}",  },
        { __LINE__,  {2,4,"(]"},  {1,4,"[)"},  1,   false,  true,   true,   "[1,4]",  "(2,4)",  "{[1,4]}",        "{4}",            "{[1,2],4}",      },
        { __LINE__,  {2,4,"(]"},  {1,5,"[)"},  1,   false,  true,   true,   "[1,5)",  "(2,4]",  "{[1,5)}",        "{}",             "{[1,2],(4,5)}",  },
        { __LINE__,  {2,4,"(]"},  {2,3,"[)"},  1,   false,  true,   true,   "[2,4]",  "(2,3)",  "{[2,4]}",        "{[3,4]}",        "{2,[3,4]}",      },
        { __LINE__,  {2,4,"(]"},  {2,4,"[)"},  1,   false,  true,   true,   "[2,4]",  "(2,4)",  "{[2,4]}",        "{4}",            "{2,4}",          },
        { __LINE__,  {2,4,"(]"},  {2,5,"[)"},  1,   false,  true,   true,   "[2,5)",  "(2,4]",  "{[2,5)}",        "{}",             "{2,(4,5)}",      },
        { __LINE__,  {2,4,"(]"},  {3,4,"[)"},  -1,  true,   true,   true,   "(2,4]",  "[3,4)",  "{(2,4]}",        "{(2,3),4}",      "{(2,3),4}",      },
        { __LINE__,  {2,4,"(]"},  {3,5,"[)"},  -1,  false,  true,   true,   "(2,5)",  "[3,4]",  "{(2,5)}",        "{(2,3)}",        "{(2,3),(4,5)}",  },
        { __LINE__,  {2,4,"(]"},  {4,5,"[)"},  -1,  false,  true,   true,   "(2,5)",  "4",      "{(2,5)}",        "{(2,4)}",        "{(2,4),(4,5)}",  },
        { __LINE__,  {2,4,"(]"},  {1,2,"(]"},  1,   false,  false,  true,   "(1,4]",  "{}",     "{(1,4]}",        "{(2,4]}",        "{(1,4]}",        },
        { __LINE__,  {2,4,"(]"},  {1,3,"(]"},  1,   false,  true,   true,   "(1,4]",  "(2,3]",  "{(1,4]}",        "{(3,4]}",        "{(1,2],(3,4]}",  },
        { __LINE__,  {2,4,"(]"},  {1,4,"(]"},  1,   false,  true,   true,   "(1,4]",  "(2,4]",  "{(1,4]}",        "{}",             "{(1,2]}",        },
        { __LINE__,  {2,4,"(]"},  {1,5,"(]"},  1,   false,  true,   true,   "(1,5]",  "(2,4]",  "{(1,5]}",        "{}",             "{(1,2],(4,5]}",  },
        { __LINE__,  {2,4,"(]"},  {2,3,"(]"},  1,   true,   true,   true,   "(2,4]",  "(2,3]",  "{(2,4]}",        "{(3,4]}",        "{(3,4]}",        },
        { __LINE__,  {2,4,"(]"},  {2,4,"(]"},  0,   true,   true,   true,   "(2,4]",  "(2,4]",  "{(2,4]}",        "{}",             "{}",             },
        { __LINE__,  {2,4,"(]"},  {2,5,"(]"},  -1,  false,  true,   true,   "(2,5]",  "(2,4]",  "{(2,5]}",        "{}",             "{(4,5]}",        },
        { __LINE__,  {2,4,"(]"},  {3,4,"(]"},  -1,  true,   true,   true,   "(2,4]",  "(3,4]",  "{(2,4]}",        "{(2,3]}",        "{(2,3]}",        },
        { __LINE__,  {2,4,"(]"},  {3,5,"(]"},  -1,  false,  true,   true,   "(2,5]",  "(3,4]",  "{(2,5]}",        "{(2,3]}",        "{(2,3],(4,5]}",  },
        { __LINE__,  {2,4,"(]"},  {4,5,"(]"},  -1,  false,  false,  true,   "(2,5]",  "{}",     "{(2,5]}",        "{(2,4]}",        "{(2,5]}",        },
        { __LINE__,  {2,4,"[]"},  {},          1,   false,  false,  false,  "[2,4]",  "{}",     "{[2,4]}",        "{[2,4]}",        "{[2,4]}",        },
        { __LINE__,  {2,4,"[]"},  {0,0,"*"},   1,   false,  true,   true,   "*",      "[2,4]",  "{*}",            "{}",             "{<2,>4}",        },
        { __LINE__,  {2,4,"[]"},  {1},         1,   false,  false,  false,  "[1,4]",  "{}",     "{1,[2,4]}",      "{[2,4]}",        "{1,[2,4]}",      },
        { __LINE__,  {2,4,"[]"},  {2},         1,   true,   true,   true,   "[2,4]",  "2",      "{[2,4]}",        "{(2,4]}",        "{(2,4]}",        },
        { __LINE__,  {2,4,"[]"},  {3},         -1,  true,   true,   true,   "[2,4]",  "3",      "{[2,4]}",        "{[2,3),(3,4]}",  "{[2,3),(3,4]}",  },
        { __LINE__,  {2,4,"[]"},  {4},         -1,  true,   true,   true,   "[2,4]",  "4",      "{[2,4]}",        "{[2,4)}",        "{[2,4)}",        },
        { __LINE__,  {2,4,"[]"},  {5},         -1,  false,  false,  false,  "[2,5]",  "{}",     "{[2,4],5}",      "{[2,4]}",        "{[2,4],5}",      },
        { __LINE__,  {2,4,"[]"},  {1,1,"<"},   1,   false,  false,  false,  "<=4",    "{}",     "{<1,[2,4]}",     "{[2,4]}",        "{<1,[2,4]}",     },
        { __LINE__,  {2,4,"[]"},  {2,2,"<"},   1,   false,  false,  true,   "<=4",    "{}",     "{<=4}",          "{[2,4]}",        "{<=4}",          },
        { __LINE__,  {2,4,"[]"},  {3,3,"<"},   1,   false,  true,   true,   "<=4",    "[2,3)",  "{<=4}",          "{[3,4]}",        "{<2,[3,4]}",     },
        { __LINE__,  {2,4,"[]"},  {4,4,"<"},   1,   false,  true,   true,   "<=4",    "[2,4)",  "{<=4}",          "{4}",            "{<2,4}",         },
        { __LINE__,  {2,4,"[]"},  {5,5,"<"},   1,   false,  true,   true,   "<5",     "[2,4]",  "{<5}",           "{}",             "{<2,(4,5)}",     },
        { __LINE__,  {2,4,"[]"},  {1,1,"<="},  1,   false,  false,  false,  "<=4",    "{}",     "{<=1,[2,4]}",    "{[2,4]}",        "{<=1,[2,4]}",    },
        { __LINE__,  {2,4,"[]"},  {2,2,"<="},  1,   false,  true,   true,   "<=4",    "2",      "{<=4}",          "{(2,4]}",        "{<2,(2,4]}",     },
        { __LINE__,  {2,4,"[]"},  {3,3,"<="},  1,   false,  true,   true,   "<=4",    "[2,3]",  "{<=4}",          "{(3,4]}",        "{<2,(3,4]}",     },
        { __LINE__,  {2,4,"[]"},  {4,4,"<="},  1,   false,  true,   true,   "<=4",    "[2,4]",  "{<=4}",          "{}",             "{<2}",           },
        { __LINE__,  {2,4,"[]"},  {5,5,"<="},  1,   false,  true,   true,   "<=5",    "[2,4]",  "{<=5}",          "{}",             "{<2,(4,5]}",     },
        { __LINE__,  {2,4,"[]"},  {1,1,">"},   1,   false,  true,   true,   ">1",     "[2,4]",  "{>1}",           "{}",             "{(1,2),>4}",     },
        { __LINE__,  {2,4,"[]"},  {2,2,">"},   -1,  false,  true,   true,   ">=2",    "(2,4]",  "{>=2}",          "{2}",            "{2,>4}",         },
        { __LINE__,  {2,4,"[]"},  {3,3,">"},   -1,  false,  true,   true,   ">=2",    "(3,4]",  "{>=2}",          "{[2,3]}",        "{[2,3],>4}",     },
        { __LINE__,  {2,4,"[]"},  {4,4,">"},   -1,  false,  false,  true,   ">=2",    "{}",     "{>=2}",          "{[2,4]}",        "{>=2}",          },
        { __LINE__,  {2,4,"[]"},  {5,5,">"},   -1,  false,  false,  false,  ">=2",    "{}",     "{[2,4],>5}",     "{[2,4]}",        "{[2,4],>5}",     },
        { __LINE__,  {2,4,"[]"},  {1,1,">="},  1,   false,  true,   true,   ">=1",    "[2,4]",  "{>=1}",          "{}",             "{[1,2),>4}",     },
        { __LINE__,  {2,4,"[]"},  {2,2,">="},  -1,  false,  true,   true,   ">=2",    "[2,4]",  "{>=2}",          "{}",             "{>4}",           },
        { __LINE__,  {2,4,"[]"},  {3,3,">="},  -1,  false,  true,   true,   ">=2",    "[3,4]",  "{>=2}",          "{[2,3)}",        "{[2,3),>4}",     },
        { __LINE__,  {2,4,"[]"},  {4,4,">="},  -1,  false,  true,   true,   ">=2",    "4",      "{>=2}",          "{[2,4)}",        "{[2,4),>4}",     },
        { __LINE__,  {2,4,"[]"},  {5,5,">="},  -1,  false,  false,  false,  ">=2",    "{}",     "{[2,4],>=5}",    "{[2,4]}",        "{[2,4],>=5}",    },
        { __LINE__,  {2,4,"[]"},  {1,2,"()"},  1,   false,  false,  true,   "(1,4]",  "{}",     "{(1,4]}",        "{[2,4]}",        "{(1,4]}",        },
        { __LINE__,  {2,4,"[]"},  {1,3,"()"},  1,   false,  true,   true,   "(1,4]",  "[2,3)",  "{(1,4]}",        "{[3,4]}",        "{(1,2),[3,4]}",  },
        { __LINE__,  {2,4,"[]"},  {1,4,"()"},  1,   false,  true,   true,   "(1,4]",  "[2,4)",  "{(1,4]}",        "{4}",            "{(1,2),4}",      },
        { __LINE__,  {2,4,"[]"},  {1,5,"()"},  1,   false,  true,   true,   "(1,5)",  "[2,4]",  "{(1,5)}",        "{}",             "{(1,2),(4,5)}",  },
        { __LINE__,  {2,4,"[]"},  {2,3,"()"},  -1,  true,   true,   true,   "[2,4]",  "(2,3)",  "{[2,4]}",        "{2,[3,4]}",      "{2,[3,4]}",      },
        { __LINE__,  {2,4,"[]"},  {2,4,"()"},  -1,  true,   true,   true,   "[2,4]",  "(2,4)",  "{[2,4]}",        "{2,4}",          "{2,4}",          },
        { __LINE__,  {2,4,"[]"},  {2,5,"()"},  -1,  false,  true,   true,   "[2,5)",  "(2,4]",  "{[2,5)}",        "{2}",            "{2,(4,5)}",      },
        { __LINE__,  {2,4,"[]"},  {3,4,"()"},  -1,  true,   true,   true,   "[2,4]",  "(3,4)",  "{[2,4]}",        "{[2,3],4}",      "{[2,3],4}",      },
        { __LINE__,  {2,4,"[]"},  {3,5,"()"},  -1,  false,  true,   true,   "[2,5)",  "(3,4]",  "{[2,5)}",        "{[2,3]}",        "{[2,3],(4,5)}",  },
        { __LINE__,  {2,4,"[]"},  {4,5,"()"},  -1,  false,  false,  true,   "[2,5)",  "{}",     "{[2,5)}",        "{[2,4]}",        "{[2,5)}",        },
        { __LINE__,  {2,4,"[]"},  {1,2,"[)"},  1,   false,  false,  true,   "[1,4]",  "{}",     "{[1,4]}",        "{[2,4]}",        "{[1,4]}",        },
        { __LINE__,  {2,4,"[]"},  {1,3,"[)"},  1,   false,  true,   true,   "[1,4]",  "[2,3)",  "{[1,4]}",        "{[3,4]}",        "{[1,2),[3,4]}",  },
        { __LINE__,  {2,4,"[]"},  {1,4,"[)"},  1,   false,  true,   true,   "[1,4]",  "[2,4)",  "{[1,4]}",        "{4}",            "{[1,2),4}",      },
        { __LINE__,  {2,4,"[]"},  {1,5,"[)"},  1,   false,  true,   true,   "[1,5)",  "[2,4]",  "{[1,5)}",        "{}",             "{[1,2),(4,5)}",  },
        { __LINE__,  {2,4,"[]"},  {2,3,"[)"},  1,   true,   true,   true,   "[2,4]",  "[2,3)",  "{[2,4]}",        "{[3,4]}",        "{[3,4]}",        },
        { __LINE__,  {2,4,"[]"},  {2,4,"[)"},  1,   true,   true,   true,   "[2,4]",  "[2,4)",  "{[2,4]}",        "{4}",            "{4}",            },
        { __LINE__,  {2,4,"[]"},  {2,5,"[)"},  -1,  false,  true,   true,   "[2,5)",  "[2,4]",  "{[2,5)}",        "{}",             "{(4,5)}",        },
        { __LINE__,  {2,4,"[]"},  {3,4,"[)"},  -1,  true,   true,   true,   "[2,4]",  "[3,4)",  "{[2,4]}",        "{[2,3),4}",      "{[2,3),4}",      },
        { __LINE__,  {2,4,"[]"},  {3,5,"[)"},  -1,  false,  true,   true,   "[2,5)",  "[3,4]",  "{[2,5)}",        "{[2,3)}",        "{[2,3),(4,5)}",  },
        { __LINE__,  {2,4,"[]"},  {4,5,"[)"},  -1,  false,  true,   true,   "[2,5)",  "4",      "{[2,5)}",        "{[2,4)}",        "{[2,4),(4,5)}",  },
        { __LINE__,  {2,4,"[]"},  {1,2,"(]"},  1,   false,  true,   true,   "(1,4]",  "2",      "{(1,4]}",        "{(2,4]}",        "{(1,2),(2,4]}",  },
        { __LINE__,  {2,4,"[]"},  {1,3,"(]"},  1,   false,  true,   true,   "(1,4]",  "[2,3]",  "{(1,4]}",        "{(3,4]}",        "{(1,2),(3,4]}",  },
        { __LINE__,  {2,4,"[]"},  {1,4,"(]"},  1,   false,  true,   true,   "(1,4]",  "[2,4]",  "{(1,4]}",        "{}",             "{(1,2)}",        },
        { __LINE__,  {2,4,"[]"},  {1,5,"(]"},  1,   false,  true,   true,   "(1,5]",  "[2,4]",  "{(1,5]}",        "{}",             "{(1,2),(4,5]}",  },
        { __LINE__,  {2,4,"[]"},  {2,3,"(]"},  -1,  true,   true,   true,   "[2,4]",  "(2,3]",  "{[2,4]}",        "{2,(3,4]}",      "{2,(3,4]}",      },
        { __LINE__,  {2,4,"[]"},  {2,4,"(]"},  -1,  true,   true,   true,   "[2,4]",  "(2,4]",  "{[2,4]}",        "{2}",            "{2}",            },
        { __LINE__,  {2,4,"[]"},  {2,5,"(]"},  -1,  false,  true,   true,   "[2,5]",  "(2,4]",  "{[2,5]}",        "{2}",            "{2,(4,5]}",      },
        { __LINE__,  {2,4,"[]"},  {3,4,"(]"},  -1,  true,   true,   true,   "[2,4]",  "(3,4]",  "{[2,4]}",        "{[2,3]}",        "{[2,3]}",        },
        { __LINE__,  {2,4,"[]"},  {3,5,"(]"},  -1,  false,  true,   true,   "[2,5]",  "(3,4]",  "{[2,5]}",        "{[2,3]}",        "{[2,3],(4,5]}",  },
        { __LINE__,  {2,4,"[]"},  {4,5,"(]"},  -1,  false,  false,  true,   "[2,5]",  "{}",     "{[2,5]}",        "{[2,4]}",        "{[2,5]}",        },
        { __LINE__,  {2,4,"[]"},  {1,1,"[]"},  1,   false,  false,  false,  "[1,4]",  "{}",     "{1,[2,4]}",      "{[2,4]}",        "{1,[2,4]}",      },
        { __LINE__,  {2,4,"[]"},  {1,2,"[]"},  1,   false,  true,   true,   "[1,4]",  "2",      "{[1,4]}",        "{(2,4]}",        "{[1,2),(2,4]}",  },
        { __LINE__,  {2,4,"[]"},  {1,3,"[]"},  1,   false,  true,   true,   "[1,4]",  "[2,3]",  "{[1,4]}",        "{(3,4]}",        "{[1,2),(3,4]}",  },
        { __LINE__,  {2,4,"[]"},  {1,4,"[]"},  1,   false,  true,   true,   "[1,4]",  "[2,4]",  "{[1,4]}",        "{}",             "{[1,2)}",        },
        { __LINE__,  {2,4,"[]"},  {1,5,"[]"},  1,   false,  true,   true,   "[1,5]",  "[2,4]",  "{[1,5]}",        "{}",             "{[1,2),(4,5]}",  },
        { __LINE__,  {2,4,"[]"},  {2,3,"[]"},  1,   true,   true,   true,   "[2,4]",  "[2,3]",  "{[2,4]}",        "{(3,4]}",        "{(3,4]}",        },
        { __LINE__,  {2,4,"[]"},  {2,4,"[]"},  0,   true,   true,   true,   "[2,4]",  "[2,4]",  "{[2,4]}",        "{}",             "{}",             },
        { __LINE__,  {2,4,"[]"},  {2,5,"[]"},  -1,  false,  true,   true,   "[2,5]",  "[2,4]",  "{[2,5]}",        "{}",             "{(4,5]}",        },
        { __LINE__,  {2,4,"[]"},  {3,4,"[]"},  -1,  true,   true,   true,   "[2,4]",  "[3,4]",  "{[2,4]}",        "{[2,3)}",        "{[2,3)}",        },
        { __LINE__,  {2,4,"[]"},  {3,5,"[]"},  -1,  false,  true,   true,   "[2,5]",  "[3,4]",  "{[2,5]}",        "{[2,3)}",        "{[2,3),(4,5]}",  },
        { __LINE__,  {2,4,"[]"},  {4,5,"[]"},  -1,  false,  true,   true,   "[2,5]",  "4",      "{[2,5]}",        "{[2,4)}",        "{[2,4),(4,5]}",  },

    };

    int cmp = 0;
    bool flag = false;
    std::string str;

    for (auto& t: tests) {

        int errors = 0;

        TRY(cmp = t.lhs.compare(t.rhs));                         TEST_EQUAL(cmp, t.compare);                   errors += int(cmp != t.compare);
        TRY(flag = t.lhs.includes(t.rhs));                       TEST_EQUAL(flag, t.includes);                 errors += int(flag != t.includes);
        TRY(flag = t.lhs.overlaps(t.rhs));                       TEST_EQUAL(flag, t.overlaps);                 errors += int(flag != t.overlaps);
        TRY(flag = t.lhs.touches(t.rhs));                        TEST_EQUAL(flag, t.touches);                  errors += int(flag != t.touches);
        TRY(str = t.lhs.envelope(t.rhs).str());                  TEST_EQUAL(str, t.envelope);                  errors += int(str != t.envelope);
        TRY(str = t.lhs.set_intersection(t.rhs).str());          TEST_EQUAL(str, t.set_intersection);          errors += int(str != t.set_intersection);
        TRY(str = t.lhs.set_union(t.rhs).str());                 TEST_EQUAL(str, t.set_union);                 errors += int(str != t.set_union);
        TRY(str = t.lhs.set_difference(t.rhs).str());            TEST_EQUAL(str, t.set_difference);            errors += int(str != t.set_difference);
        TRY(str = t.lhs.set_symmetric_difference(t.rhs).str());  TEST_EQUAL(str, t.set_symmetric_difference);  errors += int(str != t.set_symmetric_difference);

        if (errors)
            std::cout << "... [" << t.line << "] " << t.lhs << " " << t.rhs << "\n";

    }

}
