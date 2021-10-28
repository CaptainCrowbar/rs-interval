#include "rs-interval/interval.hpp"
#include "test/unit-test.hpp"
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace RS;
using namespace RS::Intervals;

using IntervalType = Interval<int>;
using SetType = IntervalSet<int>;

void test_rs_interval_integral_basic_properties() {

    IntervalType in;

    TEST_TYPE(IntervalType::value_type, int);
    TEST_EQUAL(IntervalType::category, IntervalCategory::integral);
    TEST(in.empty());
    TEST_EQUAL(in.size(), 0u);
    TEST(! in);
    TEST_EQUAL(in.str(), "{}");
    TEST_EQUAL(in.match(0), IntervalMatch::empty);
    TEST_EQUAL(in.match(42), IntervalMatch::empty);
    TEST(! in(0));
    TEST(! in(42));

}

void test_rs_interval_integral_construction() {

    IntervalType in;
    std::string str;

    TRY(in = IntervalType());                                                   TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = IntervalType(1));                                                  TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = IntervalType(1, IntervalBound::closed, IntervalBound::closed));    TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = IntervalType(1, IntervalBound::closed, IntervalBound::open));      TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = IntervalType(1, IntervalBound::open, IntervalBound::closed));      TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = IntervalType(1, IntervalBound::closed, IntervalBound::unbound));   TRY(str = in.str());  TEST_EQUAL(str, ">=1");
    TRY(in = IntervalType(1, IntervalBound::unbound, IntervalBound::closed));   TRY(str = in.str());  TEST_EQUAL(str, "<=1");
    TRY(in = IntervalType(1, IntervalBound::open, IntervalBound::unbound));     TRY(str = in.str());  TEST_EQUAL(str, ">=2");
    TRY(in = IntervalType(1, IntervalBound::unbound, IntervalBound::open));     TRY(str = in.str());  TEST_EQUAL(str, "<=0");
    TRY(in = IntervalType(1, IntervalBound::unbound, IntervalBound::unbound));  TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = IntervalType(1, 5));                                               TRY(str = in.str());  TEST_EQUAL(str, "[1,5]");
    TRY(in = IntervalType(1, 5, IntervalBound::closed));                        TRY(str = in.str());  TEST_EQUAL(str, "[1,5]");
    TRY(in = IntervalType(1, 5, IntervalBound::open));                          TRY(str = in.str());  TEST_EQUAL(str, "[2,4]");
    TRY(in = IntervalType(1, 5, IntervalBound::closed, IntervalBound::open));   TRY(str = in.str());  TEST_EQUAL(str, "[1,4]");
    TRY(in = IntervalType(1, 5, IntervalBound::open, IntervalBound::closed));   TRY(str = in.str());  TEST_EQUAL(str, "[2,5]");
    TRY(in = IntervalType(1, 5, "[]"));                                         TRY(str = in.str());  TEST_EQUAL(str, "[1,5]");
    TRY(in = IntervalType(1, 5, "()"));                                         TRY(str = in.str());  TEST_EQUAL(str, "[2,4]");
    TRY(in = IntervalType(1, 5, "[)"));                                         TRY(str = in.str());  TEST_EQUAL(str, "[1,4]");
    TRY(in = IntervalType(1, 5, "(]"));                                         TRY(str = in.str());  TEST_EQUAL(str, "[2,5]");
    TRY(in = IntervalType(1, 5, "<"));                                          TRY(str = in.str());  TEST_EQUAL(str, "<=4");
    TRY(in = IntervalType(1, 5, "<="));                                         TRY(str = in.str());  TEST_EQUAL(str, "<=5");
    TRY(in = IntervalType(1, 5, ">"));                                          TRY(str = in.str());  TEST_EQUAL(str, ">=2");
    TRY(in = IntervalType(1, 5, ">="));                                         TRY(str = in.str());  TEST_EQUAL(str, ">=1");
    TRY(in = IntervalType(1, 5, "*"));                                          TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = IntervalType(5, 1));                                               TRY(str = in.str());  TEST_EQUAL(str, "{}");

    TRY(in = make_interval(1));                                                  TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = make_interval(1, IntervalBound::closed, IntervalBound::closed));    TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = make_interval(1, IntervalBound::closed, IntervalBound::open));      TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = make_interval(1, IntervalBound::open, IntervalBound::closed));      TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = make_interval(1, IntervalBound::closed, IntervalBound::unbound));   TRY(str = in.str());  TEST_EQUAL(str, ">=1");
    TRY(in = make_interval(1, IntervalBound::unbound, IntervalBound::closed));   TRY(str = in.str());  TEST_EQUAL(str, "<=1");
    TRY(in = make_interval(1, IntervalBound::open, IntervalBound::unbound));     TRY(str = in.str());  TEST_EQUAL(str, ">=2");
    TRY(in = make_interval(1, IntervalBound::unbound, IntervalBound::open));     TRY(str = in.str());  TEST_EQUAL(str, "<=0");
    TRY(in = make_interval(1, IntervalBound::unbound, IntervalBound::unbound));  TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = make_interval(1, 5));                                               TRY(str = in.str());  TEST_EQUAL(str, "[1,5]");
    TRY(in = make_interval(1, 5, IntervalBound::closed));                        TRY(str = in.str());  TEST_EQUAL(str, "[1,5]");
    TRY(in = make_interval(1, 5, IntervalBound::open));                          TRY(str = in.str());  TEST_EQUAL(str, "[2,4]");
    TRY(in = make_interval(1, 5, IntervalBound::closed, IntervalBound::open));   TRY(str = in.str());  TEST_EQUAL(str, "[1,4]");
    TRY(in = make_interval(1, 5, IntervalBound::open, IntervalBound::closed));   TRY(str = in.str());  TEST_EQUAL(str, "[2,5]");
    TRY(in = make_interval(1, 5, "[]"));                                         TRY(str = in.str());  TEST_EQUAL(str, "[1,5]");
    TRY(in = make_interval(1, 5, "()"));                                         TRY(str = in.str());  TEST_EQUAL(str, "[2,4]");
    TRY(in = make_interval(1, 5, "[)"));                                         TRY(str = in.str());  TEST_EQUAL(str, "[1,4]");
    TRY(in = make_interval(1, 5, "(]"));                                         TRY(str = in.str());  TEST_EQUAL(str, "[2,5]");
    TRY(in = make_interval(1, 5, "<"));                                          TRY(str = in.str());  TEST_EQUAL(str, "<=4");
    TRY(in = make_interval(1, 5, "<="));                                         TRY(str = in.str());  TEST_EQUAL(str, "<=5");
    TRY(in = make_interval(1, 5, ">"));                                          TRY(str = in.str());  TEST_EQUAL(str, ">=2");
    TRY(in = make_interval(1, 5, ">="));                                         TRY(str = in.str());  TEST_EQUAL(str, ">=1");
    TRY(in = make_interval(1, 5, "*"));                                          TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = make_interval(5, 1));                                               TRY(str = in.str());  TEST_EQUAL(str, "{}");

    TRY(in = ordered_interval(5, 1, IntervalBound::closed));                       TRY(str = in.str());  TEST_EQUAL(str, "[1,5]");
    TRY(in = ordered_interval(5, 1, IntervalBound::open));                         TRY(str = in.str());  TEST_EQUAL(str, "[2,4]");
    TRY(in = ordered_interval(5, 1, IntervalBound::closed, IntervalBound::open));  TRY(str = in.str());  TEST_EQUAL(str, "[2,5]");
    TRY(in = ordered_interval(5, 1, IntervalBound::open, IntervalBound::closed));  TRY(str = in.str());  TEST_EQUAL(str, "[1,4]");

}

void test_rs_interval_integral_iterators() {

    IntervalType in;
    IntervalType::iterator it1 = {}, it2 = {};
    ptrdiff_t diff = 0;

    TRY(in = {});           TEST_EQUAL(in.size(), 0u);  TRY(it1 = in.begin());  TRY(it2 = in.end());  TRY(diff = std::distance(it1, it2));  TEST_EQUAL(diff, 0);
    TRY(in = 1);            TEST_EQUAL(in.size(), 1u);  TRY(it1 = in.begin());  TRY(it2 = in.end());  TRY(diff = std::distance(it1, it2));  TEST_EQUAL(diff, 1);
    TRY(in = IntervalType(1, 5));  TEST_EQUAL(in.size(), 5u);  TRY(it1 = in.begin());  TRY(it2 = in.end());  TRY(diff = std::distance(it1, it2));  TEST_EQUAL(diff, 5);

    TEST_EQUAL(*it1, 1);  TRY(++it1);
    TEST_EQUAL(*it1, 2);  TRY(++it1);
    TEST_EQUAL(*it1, 3);  TRY(++it1);
    TEST_EQUAL(*it1, 4);  TRY(++it1);
    TEST_EQUAL(*it1, 5);  TRY(++it1);
    TEST(it1 == it2);

}

void test_rs_interval_integral_inverse() {

    IntervalType in;
    SetType set;
    std::string str;

    TRY((in = {}));                   TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{*}");
    TRY((in = IntervalType::all()));  TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{}");
    TRY((in = 42));                   TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<=41,>=43}");
    TRY((in = {42,42,"<"}));          TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{>=42}");
    TRY((in = {42,42,"<="}));         TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{>=43}");
    TRY((in = {42,42,">"}));          TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<=42}");
    TRY((in = {42,42,">="}));         TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<=41}");
    TRY((in = {86,99,"()"}));         TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<=86,>=99}");
    TRY((in = {86,99,"(]"}));         TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<=86,>=100}");
    TRY((in = {86,99,"[)"}));         TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<=85,>=99}");
    TRY((in = {86,99,"[]"}));         TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<=85,>=100}");

}

void test_rs_interval_integral_binary_operations() {

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

        // line      lhs          rhs          cmp  incl    over    touch   envel     inter     union           diff            symm              --
        { __LINE__,  {},          {},          0,   false,  false,  false,  "{}",     "{}",     "{}",           "{}",           "{}",             },
        { __LINE__,  {0,0,"*"},   {},          1,   false,  false,  false,  "*",      "{}",     "{*}",          "{*}",          "{*}",            },
        { __LINE__,  {0,0,"*"},   {0,0,"*"},   0,   true,   true,   true,   "*",      "*",      "{*}",          "{}",           "{}",             },
        { __LINE__,  {3},         {},          1,   false,  false,  false,  "3",      "{}",     "{3}",          "{3}",          "{3}",            },
        { __LINE__,  {3},         {0,0,"*"},   1,   false,  true,   true,   "*",      "3",      "{*}",          "{}",           "{<=2,>=4}",      },
        { __LINE__,  {3},         {1},         1,   false,  false,  false,  "[1,3]",  "{}",     "{1,3}",        "{3}",          "{1,3}",          },
        { __LINE__,  {3},         {2},         1,   false,  false,  true,   "[2,3]",  "{}",     "{[2,3]}",      "{3}",          "{[2,3]}",        },
        { __LINE__,  {3},         {3},         0,   true,   true,   true,   "3",      "3",      "{3}",          "{}",           "{}",             },
        { __LINE__,  {3},         {4},         -1,  false,  false,  true,   "[3,4]",  "{}",     "{[3,4]}",      "{3}",          "{[3,4]}",        },
        { __LINE__,  {3},         {5},         -1,  false,  false,  false,  "[3,5]",  "{}",     "{3,5}",        "{3}",          "{3,5}",          },
        { __LINE__,  {3,3,"<"},   {},          1,   false,  false,  false,  "<=2",    "{}",     "{<=2}",        "{<=2}",        "{<=2}",          },
        { __LINE__,  {3,3,"<"},   {0,0,"*"},   -1,  false,  true,   true,   "*",      "<=2",    "{*}",          "{}",           "{>=3}",          },
        { __LINE__,  {3,3,"<"},   {1},         -1,  true,   true,   true,   "<=2",    "1",      "{<=2}",        "{<=0,2}",      "{<=0,2}",        },
        { __LINE__,  {3,3,"<"},   {2},         -1,  true,   true,   true,   "<=2",    "2",      "{<=2}",        "{<=1}",        "{<=1}",          },
        { __LINE__,  {3,3,"<"},   {3},         -1,  false,  false,  true,   "<=3",    "{}",     "{<=3}",        "{<=2}",        "{<=3}",          },
        { __LINE__,  {3,3,"<"},   {4},         -1,  false,  false,  false,  "<=4",    "{}",     "{<=2,4}",      "{<=2}",        "{<=2,4}",        },
        { __LINE__,  {3,3,"<"},   {5},         -1,  false,  false,  false,  "<=5",    "{}",     "{<=2,5}",      "{<=2}",        "{<=2,5}",        },
        { __LINE__,  {3,3,"<"},   {1,1,"<"},   1,   true,   true,   true,   "<=2",    "<=0",    "{<=2}",        "{[1,2]}",      "{[1,2]}",        },
        { __LINE__,  {3,3,"<"},   {2,2,"<"},   1,   true,   true,   true,   "<=2",    "<=1",    "{<=2}",        "{2}",          "{2}",            },
        { __LINE__,  {3,3,"<"},   {3,3,"<"},   0,   true,   true,   true,   "<=2",    "<=2",    "{<=2}",        "{}",           "{}",             },
        { __LINE__,  {3,3,"<"},   {4,4,"<"},   -1,  false,  true,   true,   "<=3",    "<=2",    "{<=3}",        "{}",           "{3}",            },
        { __LINE__,  {3,3,"<"},   {5,5,"<"},   -1,  false,  true,   true,   "<=4",    "<=2",    "{<=4}",        "{}",           "{[3,4]}",        },
        { __LINE__,  {3,3,"<="},  {},          1,   false,  false,  false,  "<=3",    "{}",     "{<=3}",        "{<=3}",        "{<=3}",          },
        { __LINE__,  {3,3,"<="},  {0,0,"*"},   -1,  false,  true,   true,   "*",      "<=3",    "{*}",          "{}",           "{>=4}",          },
        { __LINE__,  {3,3,"<="},  {1},         -1,  true,   true,   true,   "<=3",    "1",      "{<=3}",        "{<=0,[2,3]}",  "{<=0,[2,3]}",    },
        { __LINE__,  {3,3,"<="},  {2},         -1,  true,   true,   true,   "<=3",    "2",      "{<=3}",        "{<=1,3}",      "{<=1,3}",        },
        { __LINE__,  {3,3,"<="},  {3},         -1,  true,   true,   true,   "<=3",    "3",      "{<=3}",        "{<=2}",        "{<=2}",          },
        { __LINE__,  {3,3,"<="},  {4},         -1,  false,  false,  true,   "<=4",    "{}",     "{<=4}",        "{<=3}",        "{<=4}",          },
        { __LINE__,  {3,3,"<="},  {5},         -1,  false,  false,  false,  "<=5",    "{}",     "{<=3,5}",      "{<=3}",        "{<=3,5}",        },
        { __LINE__,  {3,3,"<="},  {1,1,"<"},   1,   true,   true,   true,   "<=3",    "<=0",    "{<=3}",        "{[1,3]}",      "{[1,3]}",        },
        { __LINE__,  {3,3,"<="},  {2,2,"<"},   1,   true,   true,   true,   "<=3",    "<=1",    "{<=3}",        "{[2,3]}",      "{[2,3]}",        },
        { __LINE__,  {3,3,"<="},  {3,3,"<"},   1,   true,   true,   true,   "<=3",    "<=2",    "{<=3}",        "{3}",          "{3}",            },
        { __LINE__,  {3,3,"<="},  {4,4,"<"},   0,   true,   true,   true,   "<=3",    "<=3",    "{<=3}",        "{}",           "{}",             },
        { __LINE__,  {3,3,"<="},  {5,5,"<"},   -1,  false,  true,   true,   "<=4",    "<=3",    "{<=4}",        "{}",           "{4}",            },
        { __LINE__,  {3,3,"<="},  {1,1,"<="},  1,   true,   true,   true,   "<=3",    "<=1",    "{<=3}",        "{[2,3]}",      "{[2,3]}",        },
        { __LINE__,  {3,3,"<="},  {2,2,"<="},  1,   true,   true,   true,   "<=3",    "<=2",    "{<=3}",        "{3}",          "{3}",            },
        { __LINE__,  {3,3,"<="},  {3,3,"<="},  0,   true,   true,   true,   "<=3",    "<=3",    "{<=3}",        "{}",           "{}",             },
        { __LINE__,  {3,3,"<="},  {4,4,"<="},  -1,  false,  true,   true,   "<=4",    "<=3",    "{<=4}",        "{}",           "{4}",            },
        { __LINE__,  {3,3,"<="},  {5,5,"<="},  -1,  false,  true,   true,   "<=5",    "<=3",    "{<=5}",        "{}",           "{[4,5]}",        },
        { __LINE__,  {3,3,">"},   {},          1,   false,  false,  false,  ">=4",    "{}",     "{>=4}",        "{>=4}",        "{>=4}",          },
        { __LINE__,  {3,3,">"},   {0,0,"*"},   1,   false,  true,   true,   "*",      ">=4",    "{*}",          "{}",           "{<=3}",          },
        { __LINE__,  {3,3,">"},   {1},         1,   false,  false,  false,  ">=1",    "{}",     "{1,>=4}",      "{>=4}",        "{1,>=4}",        },
        { __LINE__,  {3,3,">"},   {2},         1,   false,  false,  false,  ">=2",    "{}",     "{2,>=4}",      "{>=4}",        "{2,>=4}",        },
        { __LINE__,  {3,3,">"},   {3},         1,   false,  false,  true,   ">=3",    "{}",     "{>=3}",        "{>=4}",        "{>=3}",          },
        { __LINE__,  {3,3,">"},   {4},         1,   true,   true,   true,   ">=4",    "4",      "{>=4}",        "{>=5}",        "{>=5}",          },
        { __LINE__,  {3,3,">"},   {5},         -1,  true,   true,   true,   ">=4",    "5",      "{>=4}",        "{4,>=6}",      "{4,>=6}",        },
        { __LINE__,  {3,3,">"},   {1,1,"<"},   1,   false,  false,  false,  "*",      "{}",     "{<=0,>=4}",    "{>=4}",        "{<=0,>=4}",      },
        { __LINE__,  {3,3,">"},   {2,2,"<"},   1,   false,  false,  false,  "*",      "{}",     "{<=1,>=4}",    "{>=4}",        "{<=1,>=4}",      },
        { __LINE__,  {3,3,">"},   {3,3,"<"},   1,   false,  false,  false,  "*",      "{}",     "{<=2,>=4}",    "{>=4}",        "{<=2,>=4}",      },
        { __LINE__,  {3,3,">"},   {4,4,"<"},   1,   false,  false,  true,   "*",      "{}",     "{*}",          "{>=4}",        "{*}",            },
        { __LINE__,  {3,3,">"},   {5,5,"<"},   1,   false,  true,   true,   "*",      "4",      "{*}",          "{>=5}",        "{<=3,>=5}",      },
        { __LINE__,  {3,3,">"},   {1,1,"<="},  1,   false,  false,  false,  "*",      "{}",     "{<=1,>=4}",    "{>=4}",        "{<=1,>=4}",      },
        { __LINE__,  {3,3,">"},   {2,2,"<="},  1,   false,  false,  false,  "*",      "{}",     "{<=2,>=4}",    "{>=4}",        "{<=2,>=4}",      },
        { __LINE__,  {3,3,">"},   {3,3,"<="},  1,   false,  false,  true,   "*",      "{}",     "{*}",          "{>=4}",        "{*}",            },
        { __LINE__,  {3,3,">"},   {4,4,"<="},  1,   false,  true,   true,   "*",      "4",      "{*}",          "{>=5}",        "{<=3,>=5}",      },
        { __LINE__,  {3,3,">"},   {5,5,"<="},  1,   false,  true,   true,   "*",      "[4,5]",  "{*}",          "{>=6}",        "{<=3,>=6}",      },
        { __LINE__,  {3,3,">"},   {1,1,">"},   1,   false,  true,   true,   ">=2",    ">=4",    "{>=2}",        "{}",           "{[2,3]}",        },
        { __LINE__,  {3,3,">"},   {2,2,">"},   1,   false,  true,   true,   ">=3",    ">=4",    "{>=3}",        "{}",           "{3}",            },
        { __LINE__,  {3,3,">"},   {3,3,">"},   0,   true,   true,   true,   ">=4",    ">=4",    "{>=4}",        "{}",           "{}",             },
        { __LINE__,  {3,3,">"},   {4,4,">"},   -1,  true,   true,   true,   ">=4",    ">=5",    "{>=4}",        "{4}",          "{4}",            },
        { __LINE__,  {3,3,">"},   {5,5,">"},   -1,  true,   true,   true,   ">=4",    ">=6",    "{>=4}",        "{[4,5]}",      "{[4,5]}",        },
        { __LINE__,  {3,3,">="},  {},          1,   false,  false,  false,  ">=3",    "{}",     "{>=3}",        "{>=3}",        "{>=3}",          },
        { __LINE__,  {3,3,">="},  {0,0,"*"},   1,   false,  true,   true,   "*",      ">=3",    "{*}",          "{}",           "{<=2}",          },
        { __LINE__,  {3,3,">="},  {1},         1,   false,  false,  false,  ">=1",    "{}",     "{1,>=3}",      "{>=3}",        "{1,>=3}",        },
        { __LINE__,  {3,3,">="},  {2},         1,   false,  false,  true,   ">=2",    "{}",     "{>=2}",        "{>=3}",        "{>=2}",          },
        { __LINE__,  {3,3,">="},  {3},         1,   true,   true,   true,   ">=3",    "3",      "{>=3}",        "{>=4}",        "{>=4}",          },
        { __LINE__,  {3,3,">="},  {4},         -1,  true,   true,   true,   ">=3",    "4",      "{>=3}",        "{3,>=5}",      "{3,>=5}",        },
        { __LINE__,  {3,3,">="},  {5},         -1,  true,   true,   true,   ">=3",    "5",      "{>=3}",        "{[3,4],>=6}",  "{[3,4],>=6}",    },
        { __LINE__,  {3,3,">="},  {1,1,"<"},   1,   false,  false,  false,  "*",      "{}",     "{<=0,>=3}",    "{>=3}",        "{<=0,>=3}",      },
        { __LINE__,  {3,3,">="},  {2,2,"<"},   1,   false,  false,  false,  "*",      "{}",     "{<=1,>=3}",    "{>=3}",        "{<=1,>=3}",      },
        { __LINE__,  {3,3,">="},  {3,3,"<"},   1,   false,  false,  true,   "*",      "{}",     "{*}",          "{>=3}",        "{*}",            },
        { __LINE__,  {3,3,">="},  {4,4,"<"},   1,   false,  true,   true,   "*",      "3",      "{*}",          "{>=4}",        "{<=2,>=4}",      },
        { __LINE__,  {3,3,">="},  {5,5,"<"},   1,   false,  true,   true,   "*",      "[3,4]",  "{*}",          "{>=5}",        "{<=2,>=5}",      },
        { __LINE__,  {3,3,">="},  {1,1,"<="},  1,   false,  false,  false,  "*",      "{}",     "{<=1,>=3}",    "{>=3}",        "{<=1,>=3}",      },
        { __LINE__,  {3,3,">="},  {2,2,"<="},  1,   false,  false,  true,   "*",      "{}",     "{*}",          "{>=3}",        "{*}",            },
        { __LINE__,  {3,3,">="},  {3,3,"<="},  1,   false,  true,   true,   "*",      "3",      "{*}",          "{>=4}",        "{<=2,>=4}",      },
        { __LINE__,  {3,3,">="},  {4,4,"<="},  1,   false,  true,   true,   "*",      "[3,4]",  "{*}",          "{>=5}",        "{<=2,>=5}",      },
        { __LINE__,  {3,3,">="},  {5,5,"<="},  1,   false,  true,   true,   "*",      "[3,5]",  "{*}",          "{>=6}",        "{<=2,>=6}",      },
        { __LINE__,  {3,3,">="},  {1,1,">"},   1,   false,  true,   true,   ">=2",    ">=3",    "{>=2}",        "{}",           "{2}",            },
        { __LINE__,  {3,3,">="},  {2,2,">"},   0,   true,   true,   true,   ">=3",    ">=3",    "{>=3}",        "{}",           "{}",             },
        { __LINE__,  {3,3,">="},  {3,3,">"},   -1,  true,   true,   true,   ">=3",    ">=4",    "{>=3}",        "{3}",          "{3}",            },
        { __LINE__,  {3,3,">="},  {4,4,">"},   -1,  true,   true,   true,   ">=3",    ">=5",    "{>=3}",        "{[3,4]}",      "{[3,4]}",        },
        { __LINE__,  {3,3,">="},  {5,5,">"},   -1,  true,   true,   true,   ">=3",    ">=6",    "{>=3}",        "{[3,5]}",      "{[3,5]}",        },
        { __LINE__,  {3,3,">="},  {1,1,">="},  1,   false,  true,   true,   ">=1",    ">=3",    "{>=1}",        "{}",           "{[1,2]}",        },
        { __LINE__,  {3,3,">="},  {2,2,">="},  1,   false,  true,   true,   ">=2",    ">=3",    "{>=2}",        "{}",           "{2}",            },
        { __LINE__,  {3,3,">="},  {3,3,">="},  0,   true,   true,   true,   ">=3",    ">=3",    "{>=3}",        "{}",           "{}",             },
        { __LINE__,  {3,3,">="},  {4,4,">="},  -1,  true,   true,   true,   ">=3",    ">=4",    "{>=3}",        "{3}",          "{3}",            },
        { __LINE__,  {3,3,">="},  {5,5,">="},  -1,  true,   true,   true,   ">=3",    ">=5",    "{>=3}",        "{[3,4]}",      "{[3,4]}",        },
        { __LINE__,  {3,6,"[]"},  {},          1,   false,  false,  false,  "[3,6]",  "{}",     "{[3,6]}",      "{[3,6]}",      "{[3,6]}",        },
        { __LINE__,  {3,6,"[]"},  {0,0,"*"},   1,   false,  true,   true,   "*",      "[3,6]",  "{*}",          "{}",           "{<=2,>=7}",      },
        { __LINE__,  {3,6,"[]"},  {1},         1,   false,  false,  false,  "[1,6]",  "{}",     "{1,[3,6]}",    "{[3,6]}",      "{1,[3,6]}",      },
        { __LINE__,  {3,6,"[]"},  {2},         1,   false,  false,  true,   "[2,6]",  "{}",     "{[2,6]}",      "{[3,6]}",      "{[2,6]}",        },
        { __LINE__,  {3,6,"[]"},  {3},         1,   true,   true,   true,   "[3,6]",  "3",      "{[3,6]}",      "{[4,6]}",      "{[4,6]}",        },
        { __LINE__,  {3,6,"[]"},  {4},         -1,  true,   true,   true,   "[3,6]",  "4",      "{[3,6]}",      "{3,[5,6]}",    "{3,[5,6]}",      },
        { __LINE__,  {3,6,"[]"},  {5},         -1,  true,   true,   true,   "[3,6]",  "5",      "{[3,6]}",      "{[3,4],6}",    "{[3,4],6}",      },
        { __LINE__,  {3,6,"[]"},  {6},         -1,  true,   true,   true,   "[3,6]",  "6",      "{[3,6]}",      "{[3,5]}",      "{[3,5]}",        },
        { __LINE__,  {3,6,"[]"},  {7},         -1,  false,  false,  true,   "[3,7]",  "{}",     "{[3,7]}",      "{[3,6]}",      "{[3,7]}",        },
        { __LINE__,  {3,6,"[]"},  {8},         -1,  false,  false,  false,  "[3,8]",  "{}",     "{[3,6],8}",    "{[3,6]}",      "{[3,6],8}",      },
        { __LINE__,  {3,6,"[]"},  {1,1,"<"},   1,   false,  false,  false,  "<=6",    "{}",     "{<=0,[3,6]}",  "{[3,6]}",      "{<=0,[3,6]}",    },
        { __LINE__,  {3,6,"[]"},  {2,2,"<"},   1,   false,  false,  false,  "<=6",    "{}",     "{<=1,[3,6]}",  "{[3,6]}",      "{<=1,[3,6]}",    },
        { __LINE__,  {3,6,"[]"},  {3,3,"<"},   1,   false,  false,  true,   "<=6",    "{}",     "{<=6}",        "{[3,6]}",      "{<=6}",          },
        { __LINE__,  {3,6,"[]"},  {4,4,"<"},   1,   false,  true,   true,   "<=6",    "3",      "{<=6}",        "{[4,6]}",      "{<=2,[4,6]}",    },
        { __LINE__,  {3,6,"[]"},  {5,5,"<"},   1,   false,  true,   true,   "<=6",    "[3,4]",  "{<=6}",        "{[5,6]}",      "{<=2,[5,6]}",    },
        { __LINE__,  {3,6,"[]"},  {6,6,"<"},   1,   false,  true,   true,   "<=6",    "[3,5]",  "{<=6}",        "{6}",          "{<=2,6}",        },
        { __LINE__,  {3,6,"[]"},  {7,7,"<"},   1,   false,  true,   true,   "<=6",    "[3,6]",  "{<=6}",        "{}",           "{<=2}",          },
        { __LINE__,  {3,6,"[]"},  {8,8,"<"},   1,   false,  true,   true,   "<=7",    "[3,6]",  "{<=7}",        "{}",           "{<=2,7}",        },
        { __LINE__,  {3,6,"[]"},  {1,1,"<="},  1,   false,  false,  false,  "<=6",    "{}",     "{<=1,[3,6]}",  "{[3,6]}",      "{<=1,[3,6]}",    },
        { __LINE__,  {3,6,"[]"},  {2,2,"<="},  1,   false,  false,  true,   "<=6",    "{}",     "{<=6}",        "{[3,6]}",      "{<=6}",          },
        { __LINE__,  {3,6,"[]"},  {3,3,"<="},  1,   false,  true,   true,   "<=6",    "3",      "{<=6}",        "{[4,6]}",      "{<=2,[4,6]}",    },
        { __LINE__,  {3,6,"[]"},  {4,4,"<="},  1,   false,  true,   true,   "<=6",    "[3,4]",  "{<=6}",        "{[5,6]}",      "{<=2,[5,6]}",    },
        { __LINE__,  {3,6,"[]"},  {5,5,"<="},  1,   false,  true,   true,   "<=6",    "[3,5]",  "{<=6}",        "{6}",          "{<=2,6}",        },
        { __LINE__,  {3,6,"[]"},  {6,6,"<="},  1,   false,  true,   true,   "<=6",    "[3,6]",  "{<=6}",        "{}",           "{<=2}",          },
        { __LINE__,  {3,6,"[]"},  {7,7,"<="},  1,   false,  true,   true,   "<=7",    "[3,6]",  "{<=7}",        "{}",           "{<=2,7}",        },
        { __LINE__,  {3,6,"[]"},  {8,8,"<="},  1,   false,  true,   true,   "<=8",    "[3,6]",  "{<=8}",        "{}",           "{<=2,[7,8]}",    },
        { __LINE__,  {3,6,"[]"},  {1,1,">"},   1,   false,  true,   true,   ">=2",    "[3,6]",  "{>=2}",        "{}",           "{2,>=7}",        },
        { __LINE__,  {3,6,"[]"},  {2,2,">"},   -1,  false,  true,   true,   ">=3",    "[3,6]",  "{>=3}",        "{}",           "{>=7}",          },
        { __LINE__,  {3,6,"[]"},  {3,3,">"},   -1,  false,  true,   true,   ">=3",    "[4,6]",  "{>=3}",        "{3}",          "{3,>=7}",        },
        { __LINE__,  {3,6,"[]"},  {4,4,">"},   -1,  false,  true,   true,   ">=3",    "[5,6]",  "{>=3}",        "{[3,4]}",      "{[3,4],>=7}",    },
        { __LINE__,  {3,6,"[]"},  {5,5,">"},   -1,  false,  true,   true,   ">=3",    "6",      "{>=3}",        "{[3,5]}",      "{[3,5],>=7}",    },
        { __LINE__,  {3,6,"[]"},  {6,6,">"},   -1,  false,  false,  true,   ">=3",    "{}",     "{>=3}",        "{[3,6]}",      "{>=3}",          },
        { __LINE__,  {3,6,"[]"},  {7,7,">"},   -1,  false,  false,  false,  ">=3",    "{}",     "{[3,6],>=8}",  "{[3,6]}",      "{[3,6],>=8}",    },
        { __LINE__,  {3,6,"[]"},  {8,8,">"},   -1,  false,  false,  false,  ">=3",    "{}",     "{[3,6],>=9}",  "{[3,6]}",      "{[3,6],>=9}",    },
        { __LINE__,  {3,6,"[]"},  {1,1,">="},  1,   false,  true,   true,   ">=1",    "[3,6]",  "{>=1}",        "{}",           "{[1,2],>=7}",    },
        { __LINE__,  {3,6,"[]"},  {2,2,">="},  1,   false,  true,   true,   ">=2",    "[3,6]",  "{>=2}",        "{}",           "{2,>=7}",        },
        { __LINE__,  {3,6,"[]"},  {3,3,">="},  -1,  false,  true,   true,   ">=3",    "[3,6]",  "{>=3}",        "{}",           "{>=7}",          },
        { __LINE__,  {3,6,"[]"},  {4,4,">="},  -1,  false,  true,   true,   ">=3",    "[4,6]",  "{>=3}",        "{3}",          "{3,>=7}",        },
        { __LINE__,  {3,6,"[]"},  {5,5,">="},  -1,  false,  true,   true,   ">=3",    "[5,6]",  "{>=3}",        "{[3,4]}",      "{[3,4],>=7}",    },
        { __LINE__,  {3,6,"[]"},  {6,6,">="},  -1,  false,  true,   true,   ">=3",    "6",      "{>=3}",        "{[3,5]}",      "{[3,5],>=7}",    },
        { __LINE__,  {3,6,"[]"},  {7,7,">="},  -1,  false,  false,  true,   ">=3",    "{}",     "{>=3}",        "{[3,6]}",      "{>=3}",          },
        { __LINE__,  {3,6,"[]"},  {8,8,">="},  -1,  false,  false,  false,  ">=3",    "{}",     "{[3,6],>=8}",  "{[3,6]}",      "{[3,6],>=8}",    },
        { __LINE__,  {3,6,"[]"},  {1,1,"[]"},  1,   false,  false,  false,  "[1,6]",  "{}",     "{1,[3,6]}",    "{[3,6]}",      "{1,[3,6]}",      },
        { __LINE__,  {3,6,"[]"},  {1,2,"[]"},  1,   false,  false,  true,   "[1,6]",  "{}",     "{[1,6]}",      "{[3,6]}",      "{[1,6]}",        },
        { __LINE__,  {3,6,"[]"},  {1,3,"[]"},  1,   false,  true,   true,   "[1,6]",  "3",      "{[1,6]}",      "{[4,6]}",      "{[1,2],[4,6]}",  },
        { __LINE__,  {3,6,"[]"},  {1,4,"[]"},  1,   false,  true,   true,   "[1,6]",  "[3,4]",  "{[1,6]}",      "{[5,6]}",      "{[1,2],[5,6]}",  },
        { __LINE__,  {3,6,"[]"},  {1,5,"[]"},  1,   false,  true,   true,   "[1,6]",  "[3,5]",  "{[1,6]}",      "{6}",          "{[1,2],6}",      },
        { __LINE__,  {3,6,"[]"},  {1,6,"[]"},  1,   false,  true,   true,   "[1,6]",  "[3,6]",  "{[1,6]}",      "{}",           "{[1,2]}",        },
        { __LINE__,  {3,6,"[]"},  {1,7,"[]"},  1,   false,  true,   true,   "[1,7]",  "[3,6]",  "{[1,7]}",      "{}",           "{[1,2],7}",      },
        { __LINE__,  {3,6,"[]"},  {1,8,"[]"},  1,   false,  true,   true,   "[1,8]",  "[3,6]",  "{[1,8]}",      "{}",           "{[1,2],[7,8]}",  },
        { __LINE__,  {3,6,"[]"},  {2,2,"[]"},  1,   false,  false,  true,   "[2,6]",  "{}",     "{[2,6]}",      "{[3,6]}",      "{[2,6]}",        },
        { __LINE__,  {3,6,"[]"},  {2,3,"[]"},  1,   false,  true,   true,   "[2,6]",  "3",      "{[2,6]}",      "{[4,6]}",      "{2,[4,6]}",      },
        { __LINE__,  {3,6,"[]"},  {2,4,"[]"},  1,   false,  true,   true,   "[2,6]",  "[3,4]",  "{[2,6]}",      "{[5,6]}",      "{2,[5,6]}",      },
        { __LINE__,  {3,6,"[]"},  {2,5,"[]"},  1,   false,  true,   true,   "[2,6]",  "[3,5]",  "{[2,6]}",      "{6}",          "{2,6}",          },
        { __LINE__,  {3,6,"[]"},  {2,6,"[]"},  1,   false,  true,   true,   "[2,6]",  "[3,6]",  "{[2,6]}",      "{}",           "{2}",            },
        { __LINE__,  {3,6,"[]"},  {2,7,"[]"},  1,   false,  true,   true,   "[2,7]",  "[3,6]",  "{[2,7]}",      "{}",           "{2,7}",          },
        { __LINE__,  {3,6,"[]"},  {2,8,"[]"},  1,   false,  true,   true,   "[2,8]",  "[3,6]",  "{[2,8]}",      "{}",           "{2,[7,8]}",      },
        { __LINE__,  {3,6,"[]"},  {3,3,"[]"},  1,   true,   true,   true,   "[3,6]",  "3",      "{[3,6]}",      "{[4,6]}",      "{[4,6]}",        },
        { __LINE__,  {3,6,"[]"},  {3,4,"[]"},  1,   true,   true,   true,   "[3,6]",  "[3,4]",  "{[3,6]}",      "{[5,6]}",      "{[5,6]}",        },
        { __LINE__,  {3,6,"[]"},  {3,5,"[]"},  1,   true,   true,   true,   "[3,6]",  "[3,5]",  "{[3,6]}",      "{6}",          "{6}",            },
        { __LINE__,  {3,6,"[]"},  {3,6,"[]"},  0,   true,   true,   true,   "[3,6]",  "[3,6]",  "{[3,6]}",      "{}",           "{}",             },
        { __LINE__,  {3,6,"[]"},  {3,7,"[]"},  -1,  false,  true,   true,   "[3,7]",  "[3,6]",  "{[3,7]}",      "{}",           "{7}",            },
        { __LINE__,  {3,6,"[]"},  {3,8,"[]"},  -1,  false,  true,   true,   "[3,8]",  "[3,6]",  "{[3,8]}",      "{}",           "{[7,8]}",        },
        { __LINE__,  {3,6,"[]"},  {4,4,"[]"},  -1,  true,   true,   true,   "[3,6]",  "4",      "{[3,6]}",      "{3,[5,6]}",    "{3,[5,6]}",      },
        { __LINE__,  {3,6,"[]"},  {4,5,"[]"},  -1,  true,   true,   true,   "[3,6]",  "[4,5]",  "{[3,6]}",      "{3,6}",        "{3,6}",          },
        { __LINE__,  {3,6,"[]"},  {4,6,"[]"},  -1,  true,   true,   true,   "[3,6]",  "[4,6]",  "{[3,6]}",      "{3}",          "{3}",            },
        { __LINE__,  {3,6,"[]"},  {4,7,"[]"},  -1,  false,  true,   true,   "[3,7]",  "[4,6]",  "{[3,7]}",      "{3}",          "{3,7}",          },
        { __LINE__,  {3,6,"[]"},  {4,8,"[]"},  -1,  false,  true,   true,   "[3,8]",  "[4,6]",  "{[3,8]}",      "{3}",          "{3,[7,8]}",      },
        { __LINE__,  {3,6,"[]"},  {5,5,"[]"},  -1,  true,   true,   true,   "[3,6]",  "5",      "{[3,6]}",      "{[3,4],6}",    "{[3,4],6}",      },
        { __LINE__,  {3,6,"[]"},  {5,6,"[]"},  -1,  true,   true,   true,   "[3,6]",  "[5,6]",  "{[3,6]}",      "{[3,4]}",      "{[3,4]}",        },
        { __LINE__,  {3,6,"[]"},  {5,7,"[]"},  -1,  false,  true,   true,   "[3,7]",  "[5,6]",  "{[3,7]}",      "{[3,4]}",      "{[3,4],7}",      },
        { __LINE__,  {3,6,"[]"},  {5,8,"[]"},  -1,  false,  true,   true,   "[3,8]",  "[5,6]",  "{[3,8]}",      "{[3,4]}",      "{[3,4],[7,8]}",  },
        { __LINE__,  {3,6,"[]"},  {6,6,"[]"},  -1,  true,   true,   true,   "[3,6]",  "6",      "{[3,6]}",      "{[3,5]}",      "{[3,5]}",        },
        { __LINE__,  {3,6,"[]"},  {6,7,"[]"},  -1,  false,  true,   true,   "[3,7]",  "6",      "{[3,7]}",      "{[3,5]}",      "{[3,5],7}",      },
        { __LINE__,  {3,6,"[]"},  {6,8,"[]"},  -1,  false,  true,   true,   "[3,8]",  "6",      "{[3,8]}",      "{[3,5]}",      "{[3,5],[7,8]}",  },
        { __LINE__,  {3,6,"[]"},  {7,7,"[]"},  -1,  false,  false,  true,   "[3,7]",  "{}",     "{[3,7]}",      "{[3,6]}",      "{[3,7]}",        },
        { __LINE__,  {3,6,"[]"},  {7,8,"[]"},  -1,  false,  false,  true,   "[3,8]",  "{}",     "{[3,8]}",      "{[3,6]}",      "{[3,8]}",        },
        { __LINE__,  {3,6,"[]"},  {8,8,"[]"},  -1,  false,  false,  false,  "[3,8]",  "{}",     "{[3,6],8}",    "{[3,6]}",      "{[3,6],8}",      },

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
