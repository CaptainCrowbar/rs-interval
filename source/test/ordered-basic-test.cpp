#include "rs-interval.hpp"
#include "test/unit-test.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace RS;
using namespace RS::Intervals;
using namespace std::literals;

using IntervalType = Interval<std::string>;
using SetType = IntervalSet<std::string>;

void test_rs_ordered_interval_basic_properties() {

    IntervalType in;

    TEST_TYPE(IntervalType::value_type, std::string);
    TEST_EQUAL(IntervalType::category, IntervalCategory::ordered);
    TEST(in.empty());
    TEST(! in);
    TEST_EQUAL(in.str(), "{}");
    TEST_EQUAL(in.match(""), IntervalMatch::empty);
    TEST_EQUAL(in.match("hello"), IntervalMatch::empty);
    TEST(! in(""));
    TEST(! in("hello"));

}

void test_rs_ordered_interval_construction() {

    IntervalType in;
    std::string str;

    TRY(in = IntervalType());                                                      TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = IntervalType("a"));                                                   TRY(str = in.str());  TEST_EQUAL(str, "a");
    TRY(in = IntervalType("a", IntervalBound::closed, IntervalBound::closed));     TRY(str = in.str());  TEST_EQUAL(str, "a");
    TRY(in = IntervalType("a", IntervalBound::closed, IntervalBound::open));       TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = IntervalType("a", IntervalBound::open, IntervalBound::closed));       TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = IntervalType("a", IntervalBound::closed, IntervalBound::unbound));    TRY(str = in.str());  TEST_EQUAL(str, ">=a");
    TRY(in = IntervalType("a", IntervalBound::unbound, IntervalBound::closed));    TRY(str = in.str());  TEST_EQUAL(str, "<=a");
    TRY(in = IntervalType("a", IntervalBound::open, IntervalBound::unbound));      TRY(str = in.str());  TEST_EQUAL(str, ">a");
    TRY(in = IntervalType("a", IntervalBound::unbound, IntervalBound::open));      TRY(str = in.str());  TEST_EQUAL(str, "<a");
    TRY(in = IntervalType("a", IntervalBound::unbound, IntervalBound::unbound));   TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = IntervalType("a", "z"));                                              TRY(str = in.str());  TEST_EQUAL(str, "[a,z]");
    TRY(in = IntervalType("a", "z", IntervalBound::closed));                       TRY(str = in.str());  TEST_EQUAL(str, "[a,z]");
    TRY(in = IntervalType("a", "z", IntervalBound::open));                         TRY(str = in.str());  TEST_EQUAL(str, "(a,z)");
    TRY(in = IntervalType("a", "z", IntervalBound::closed, IntervalBound::open));  TRY(str = in.str());  TEST_EQUAL(str, "[a,z)");
    TRY(in = IntervalType("a", "z", IntervalBound::open, IntervalBound::closed));  TRY(str = in.str());  TEST_EQUAL(str, "(a,z]");
    TRY(in = IntervalType("a", "z", "[]"));                                        TRY(str = in.str());  TEST_EQUAL(str, "[a,z]");
    TRY(in = IntervalType("a", "z", "()"));                                        TRY(str = in.str());  TEST_EQUAL(str, "(a,z)");
    TRY(in = IntervalType("a", "z", "[)"));                                        TRY(str = in.str());  TEST_EQUAL(str, "[a,z)");
    TRY(in = IntervalType("a", "z", "(]"));                                        TRY(str = in.str());  TEST_EQUAL(str, "(a,z]");
    TRY(in = IntervalType("a", "z", "<"));                                         TRY(str = in.str());  TEST_EQUAL(str, "<z");
    TRY(in = IntervalType("a", "z", "<="));                                        TRY(str = in.str());  TEST_EQUAL(str, "<=z");
    TRY(in = IntervalType("a", "z", ">"));                                         TRY(str = in.str());  TEST_EQUAL(str, ">a");
    TRY(in = IntervalType("a", "z", ">="));                                        TRY(str = in.str());  TEST_EQUAL(str, ">=a");
    TRY(in = IntervalType("a", "z", "*"));                                         TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = IntervalType("z", "a"));                                              TRY(str = in.str());  TEST_EQUAL(str, "{}");

    TRY(in = make_interval("a"s));                                                    TRY(str = in.str());  TEST_EQUAL(str, "a");
    TRY(in = make_interval("a"s, IntervalBound::closed, IntervalBound::closed));      TRY(str = in.str());  TEST_EQUAL(str, "a");
    TRY(in = make_interval("a"s, IntervalBound::closed, IntervalBound::open));        TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = make_interval("a"s, IntervalBound::open, IntervalBound::closed));        TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = make_interval("a"s, IntervalBound::closed, IntervalBound::unbound));     TRY(str = in.str());  TEST_EQUAL(str, ">=a");
    TRY(in = make_interval("a"s, IntervalBound::unbound, IntervalBound::closed));     TRY(str = in.str());  TEST_EQUAL(str, "<=a");
    TRY(in = make_interval("a"s, IntervalBound::open, IntervalBound::unbound));       TRY(str = in.str());  TEST_EQUAL(str, ">a");
    TRY(in = make_interval("a"s, IntervalBound::unbound, IntervalBound::open));       TRY(str = in.str());  TEST_EQUAL(str, "<a");
    TRY(in = make_interval("a"s, IntervalBound::unbound, IntervalBound::unbound));    TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = make_interval("a"s, "z"s));                                              TRY(str = in.str());  TEST_EQUAL(str, "[a,z]");
    TRY(in = make_interval("a"s, "z"s, IntervalBound::closed));                       TRY(str = in.str());  TEST_EQUAL(str, "[a,z]");
    TRY(in = make_interval("a"s, "z"s, IntervalBound::open));                         TRY(str = in.str());  TEST_EQUAL(str, "(a,z)");
    TRY(in = make_interval("a"s, "z"s, IntervalBound::closed, IntervalBound::open));  TRY(str = in.str());  TEST_EQUAL(str, "[a,z)");
    TRY(in = make_interval("a"s, "z"s, IntervalBound::open, IntervalBound::closed));  TRY(str = in.str());  TEST_EQUAL(str, "(a,z]");
    TRY(in = make_interval("a"s, "z"s, "[]"));                                        TRY(str = in.str());  TEST_EQUAL(str, "[a,z]");
    TRY(in = make_interval("a"s, "z"s, "()"));                                        TRY(str = in.str());  TEST_EQUAL(str, "(a,z)");
    TRY(in = make_interval("a"s, "z"s, "[)"));                                        TRY(str = in.str());  TEST_EQUAL(str, "[a,z)");
    TRY(in = make_interval("a"s, "z"s, "(]"));                                        TRY(str = in.str());  TEST_EQUAL(str, "(a,z]");
    TRY(in = make_interval("a"s, "z"s, "<"));                                         TRY(str = in.str());  TEST_EQUAL(str, "<z");
    TRY(in = make_interval("a"s, "z"s, "<="));                                        TRY(str = in.str());  TEST_EQUAL(str, "<=z");
    TRY(in = make_interval("a"s, "z"s, ">"));                                         TRY(str = in.str());  TEST_EQUAL(str, ">a");
    TRY(in = make_interval("a"s, "z"s, ">="));                                        TRY(str = in.str());  TEST_EQUAL(str, ">=a");
    TRY(in = make_interval("a"s, "z"s, "*"));                                         TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = make_interval("z"s, "a"s));                                              TRY(str = in.str());  TEST_EQUAL(str, "{}");

    TRY(in = ordered_interval("z"s, "a"s, IntervalBound::closed));                       TRY(str = in.str());  TEST_EQUAL(str, "[a,z]");
    TRY(in = ordered_interval("z"s, "a"s, IntervalBound::open));                         TRY(str = in.str());  TEST_EQUAL(str, "(a,z)");
    TRY(in = ordered_interval("z"s, "a"s, IntervalBound::closed, IntervalBound::open));  TRY(str = in.str());  TEST_EQUAL(str, "(a,z]");
    TRY(in = ordered_interval("z"s, "a"s, IntervalBound::open, IntervalBound::closed));  TRY(str = in.str());  TEST_EQUAL(str, "[a,z)");

}

void test_rs_ordered_interval_inverse() {

    IntervalType in;
    SetType set;
    std::string str;

    TRY((in = {}));                      TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{*}");
    TRY((in = IntervalType::all()));     TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{}");
    TRY((in = {"hello"}));               TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<hello,>hello}");
    TRY((in = {"hello","hello","<"}));   TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{>=hello}");
    TRY((in = {"hello","hello","<="}));  TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{>hello}");
    TRY((in = {"hello","hello",">"}));   TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<=hello}");
    TRY((in = {"hello","hello",">="}));  TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<hello}");
    TRY((in = {"hello","world","()"}));  TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<=hello,>=world}");
    TRY((in = {"hello","world","(]"}));  TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<=hello,>world}");
    TRY((in = {"hello","world","[)"}));  TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<hello,>=world}");
    TRY((in = {"hello","world","[]"}));  TRY(set = in.inverse());  TRY(str = set.str());  TEST_EQUAL(str, "{<hello,>world}");

}

void test_rs_ordered_interval_binary_operations() {

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

        // line      lhs              rhs              cmp  incl    over    touch   envel     inter     union             diff              symm              --
        { __LINE__,  {},              {},              0,   false,  false,  false,  "{}",     "{}",     "{}",             "{}",             "{}",             },
        { __LINE__,  {" "," ","*"},   {},              1,   false,  false,  false,  "*",      "{}",     "{*}",            "{*}",            "{*}",            },
        { __LINE__,  {" "," ","*"},   {" "," ","*"},   0,   true,   true,   true,   "*",      "*",      "{*}",            "{}",             "{}",             },
        { __LINE__,  {"b"},           {},              1,   false,  false,  false,  "b",      "{}",     "{b}",            "{b}",            "{b}",            },
        { __LINE__,  {"b"},           {" "," ","*"},   1,   false,  true,   true,   "*",      "b",      "{*}",            "{}",             "{<b,>b}",        },
        { __LINE__,  {"b"},           {"a"},           1,   false,  false,  false,  "[a,b]",  "{}",     "{a,b}",          "{b}",            "{a,b}",          },
        { __LINE__,  {"b"},           {"b"},           0,   true,   true,   true,   "b",      "b",      "{b}",            "{}",             "{}",             },
        { __LINE__,  {"b"},           {"c"},           -1,  false,  false,  false,  "[b,c]",  "{}",     "{b,c}",          "{b}",            "{b,c}",          },
        { __LINE__,  {"b","b","<"},   {},              1,   false,  false,  false,  "<b",     "{}",     "{<b}",           "{<b}",           "{<b}",           },
        { __LINE__,  {"b","b","<"},   {" "," ","*"},   -1,  false,  true,   true,   "*",      "<b",     "{*}",            "{}",             "{>=b}",          },
        { __LINE__,  {"b","b","<"},   {"a"},           -1,  true,   true,   true,   "<b",     "a",      "{<b}",           "{<a,(a,b)}",     "{<a,(a,b)}",     },
        { __LINE__,  {"b","b","<"},   {"b"},           -1,  false,  false,  true,   "<=b",    "{}",     "{<=b}",          "{<b}",           "{<=b}",          },
        { __LINE__,  {"b","b","<"},   {"c"},           -1,  false,  false,  false,  "<=c",    "{}",     "{<b,c}",         "{<b}",           "{<b,c}",         },
        { __LINE__,  {"b","b","<"},   {"a","a","<"},   1,   true,   true,   true,   "<b",     "<a",     "{<b}",           "{[a,b)}",        "{[a,b)}",        },
        { __LINE__,  {"b","b","<"},   {"b","b","<"},   0,   true,   true,   true,   "<b",     "<b",     "{<b}",           "{}",             "{}",             },
        { __LINE__,  {"b","b","<"},   {"c","c","<"},   -1,  false,  true,   true,   "<c",     "<b",     "{<c}",           "{}",             "{[b,c)}",        },
        { __LINE__,  {"b","b","<="},  {},              1,   false,  false,  false,  "<=b",    "{}",     "{<=b}",          "{<=b}",          "{<=b}",          },
        { __LINE__,  {"b","b","<="},  {" "," ","*"},   -1,  false,  true,   true,   "*",      "<=b",    "{*}",            "{}",             "{>b}",           },
        { __LINE__,  {"b","b","<="},  {"a"},           -1,  true,   true,   true,   "<=b",    "a",      "{<=b}",          "{<a,(a,b]}",     "{<a,(a,b]}",     },
        { __LINE__,  {"b","b","<="},  {"b"},           -1,  true,   true,   true,   "<=b",    "b",      "{<=b}",          "{<b}",           "{<b}",           },
        { __LINE__,  {"b","b","<="},  {"c"},           -1,  false,  false,  false,  "<=c",    "{}",     "{<=b,c}",        "{<=b}",          "{<=b,c}",        },
        { __LINE__,  {"b","b","<="},  {"a","a","<"},   1,   true,   true,   true,   "<=b",    "<a",     "{<=b}",          "{[a,b]}",        "{[a,b]}",        },
        { __LINE__,  {"b","b","<="},  {"b","b","<"},   1,   true,   true,   true,   "<=b",    "<b",     "{<=b}",          "{b}",            "{b}",            },
        { __LINE__,  {"b","b","<="},  {"c","c","<"},   -1,  false,  true,   true,   "<c",     "<=b",    "{<c}",           "{}",             "{(b,c)}",        },
        { __LINE__,  {"b","b","<="},  {"a","a","<="},  1,   true,   true,   true,   "<=b",    "<=a",    "{<=b}",          "{(a,b]}",        "{(a,b]}",        },
        { __LINE__,  {"b","b","<="},  {"b","b","<="},  0,   true,   true,   true,   "<=b",    "<=b",    "{<=b}",          "{}",             "{}",             },
        { __LINE__,  {"b","b","<="},  {"c","c","<="},  -1,  false,  true,   true,   "<=c",    "<=b",    "{<=c}",          "{}",             "{(b,c]}",        },
        { __LINE__,  {"b","b",">"},   {},              1,   false,  false,  false,  ">b",     "{}",     "{>b}",           "{>b}",           "{>b}",           },
        { __LINE__,  {"b","b",">"},   {" "," ","*"},   1,   false,  true,   true,   "*",      ">b",     "{*}",            "{}",             "{<=b}",          },
        { __LINE__,  {"b","b",">"},   {"a"},           1,   false,  false,  false,  ">=a",    "{}",     "{a,>b}",         "{>b}",           "{a,>b}",         },
        { __LINE__,  {"b","b",">"},   {"b"},           1,   false,  false,  true,   ">=b",    "{}",     "{>=b}",          "{>b}",           "{>=b}",          },
        { __LINE__,  {"b","b",">"},   {"c"},           -1,  true,   true,   true,   ">b",     "c",      "{>b}",           "{(b,c),>c}",     "{(b,c),>c}",     },
        { __LINE__,  {"b","b",">"},   {"a","a","<"},   1,   false,  false,  false,  "*",      "{}",     "{<a,>b}",        "{>b}",           "{<a,>b}",        },
        { __LINE__,  {"b","b",">"},   {"b","b","<"},   1,   false,  false,  false,  "*",      "{}",     "{<b,>b}",        "{>b}",           "{<b,>b}",        },
        { __LINE__,  {"b","b",">"},   {"c","c","<"},   1,   false,  true,   true,   "*",      "(b,c)",  "{*}",            "{>=c}",          "{<=b,>=c}",      },
        { __LINE__,  {"b","b",">"},   {"a","a","<="},  1,   false,  false,  false,  "*",      "{}",     "{<=a,>b}",       "{>b}",           "{<=a,>b}",       },
        { __LINE__,  {"b","b",">"},   {"b","b","<="},  1,   false,  false,  true,   "*",      "{}",     "{*}",            "{>b}",           "{*}",            },
        { __LINE__,  {"b","b",">"},   {"c","c","<="},  1,   false,  true,   true,   "*",      "(b,c]",  "{*}",            "{>c}",           "{<=b,>c}",       },
        { __LINE__,  {"b","b",">"},   {"a","a",">"},   1,   false,  true,   true,   ">a",     ">b",     "{>a}",           "{}",             "{(a,b]}",        },
        { __LINE__,  {"b","b",">"},   {"b","b",">"},   0,   true,   true,   true,   ">b",     ">b",     "{>b}",           "{}",             "{}",             },
        { __LINE__,  {"b","b",">"},   {"c","c",">"},   -1,  true,   true,   true,   ">b",     ">c",     "{>b}",           "{(b,c]}",        "{(b,c]}",        },
        { __LINE__,  {"b","b",">="},  {},              1,   false,  false,  false,  ">=b",    "{}",     "{>=b}",          "{>=b}",          "{>=b}",          },
        { __LINE__,  {"b","b",">="},  {" "," ","*"},   1,   false,  true,   true,   "*",      ">=b",    "{*}",            "{}",             "{<b}",           },
        { __LINE__,  {"b","b",">="},  {"a"},           1,   false,  false,  false,  ">=a",    "{}",     "{a,>=b}",        "{>=b}",          "{a,>=b}",        },
        { __LINE__,  {"b","b",">="},  {"b"},           1,   true,   true,   true,   ">=b",    "b",      "{>=b}",          "{>b}",           "{>b}",           },
        { __LINE__,  {"b","b",">="},  {"c"},           -1,  true,   true,   true,   ">=b",    "c",      "{>=b}",          "{[b,c),>c}",     "{[b,c),>c}",     },
        { __LINE__,  {"b","b",">="},  {"a","a","<"},   1,   false,  false,  false,  "*",      "{}",     "{<a,>=b}",       "{>=b}",          "{<a,>=b}",       },
        { __LINE__,  {"b","b",">="},  {"b","b","<"},   1,   false,  false,  true,   "*",      "{}",     "{*}",            "{>=b}",          "{*}",            },
        { __LINE__,  {"b","b",">="},  {"c","c","<"},   1,   false,  true,   true,   "*",      "[b,c)",  "{*}",            "{>=c}",          "{<b,>=c}",       },
        { __LINE__,  {"b","b",">="},  {"a","a","<="},  1,   false,  false,  false,  "*",      "{}",     "{<=a,>=b}",      "{>=b}",          "{<=a,>=b}",      },
        { __LINE__,  {"b","b",">="},  {"b","b","<="},  1,   false,  true,   true,   "*",      "b",      "{*}",            "{>b}",           "{<b,>b}",        },
        { __LINE__,  {"b","b",">="},  {"c","c","<="},  1,   false,  true,   true,   "*",      "[b,c]",  "{*}",            "{>c}",           "{<b,>c}",        },
        { __LINE__,  {"b","b",">="},  {"a","a",">"},   1,   false,  true,   true,   ">a",     ">=b",    "{>a}",           "{}",             "{(a,b)}",        },
        { __LINE__,  {"b","b",">="},  {"b","b",">"},   -1,  true,   true,   true,   ">=b",    ">b",     "{>=b}",          "{b}",            "{b}",            },
        { __LINE__,  {"b","b",">="},  {"c","c",">"},   -1,  true,   true,   true,   ">=b",    ">c",     "{>=b}",          "{[b,c]}",        "{[b,c]}",        },
        { __LINE__,  {"b","b",">="},  {"a","a",">="},  1,   false,  true,   true,   ">=a",    ">=b",    "{>=a}",          "{}",             "{[a,b)}",        },
        { __LINE__,  {"b","b",">="},  {"b","b",">="},  0,   true,   true,   true,   ">=b",    ">=b",    "{>=b}",          "{}",             "{}",             },
        { __LINE__,  {"b","b",">="},  {"c","c",">="},  -1,  true,   true,   true,   ">=b",    ">=c",    "{>=b}",          "{[b,c)}",        "{[b,c)}",        },
        { __LINE__,  {"b","d","()"},  {},              1,   false,  false,  false,  "(b,d)",  "{}",     "{(b,d)}",        "{(b,d)}",        "{(b,d)}",        },
        { __LINE__,  {"b","d","()"},  {" "," ","*"},   1,   false,  true,   true,   "*",      "(b,d)",  "{*}",            "{}",             "{<=b,>=d}",      },
        { __LINE__,  {"b","d","()"},  {"a"},           1,   false,  false,  false,  "[a,d)",  "{}",     "{a,(b,d)}",      "{(b,d)}",        "{a,(b,d)}",      },
        { __LINE__,  {"b","d","()"},  {"b"},           1,   false,  false,  true,   "[b,d)",  "{}",     "{[b,d)}",        "{(b,d)}",        "{[b,d)}",        },
        { __LINE__,  {"b","d","()"},  {"c"},           -1,  true,   true,   true,   "(b,d)",  "c",      "{(b,d)}",        "{(b,c),(c,d)}",  "{(b,c),(c,d)}",  },
        { __LINE__,  {"b","d","()"},  {"d"},           -1,  false,  false,  true,   "(b,d]",  "{}",     "{(b,d]}",        "{(b,d)}",        "{(b,d]}",        },
        { __LINE__,  {"b","d","()"},  {"e"},           -1,  false,  false,  false,  "(b,e]",  "{}",     "{(b,d),e}",      "{(b,d)}",        "{(b,d),e}",      },
        { __LINE__,  {"b","d","()"},  {"a","a","<"},   1,   false,  false,  false,  "<d",     "{}",     "{<a,(b,d)}",     "{(b,d)}",        "{<a,(b,d)}",     },
        { __LINE__,  {"b","d","()"},  {"b","b","<"},   1,   false,  false,  false,  "<d",     "{}",     "{<b,(b,d)}",     "{(b,d)}",        "{<b,(b,d)}",     },
        { __LINE__,  {"b","d","()"},  {"c","c","<"},   1,   false,  true,   true,   "<d",     "(b,c)",  "{<d}",           "{[c,d)}",        "{<=b,[c,d)}",    },
        { __LINE__,  {"b","d","()"},  {"d","d","<"},   1,   false,  true,   true,   "<d",     "(b,d)",  "{<d}",           "{}",             "{<=b}",          },
        { __LINE__,  {"b","d","()"},  {"e","e","<"},   1,   false,  true,   true,   "<e",     "(b,d)",  "{<e}",           "{}",             "{<=b,[d,e)}",    },
        { __LINE__,  {"b","d","()"},  {"a","a","<="},  1,   false,  false,  false,  "<d",     "{}",     "{<=a,(b,d)}",    "{(b,d)}",        "{<=a,(b,d)}",    },
        { __LINE__,  {"b","d","()"},  {"b","b","<="},  1,   false,  false,  true,   "<d",     "{}",     "{<d}",           "{(b,d)}",        "{<d}",           },
        { __LINE__,  {"b","d","()"},  {"c","c","<="},  1,   false,  true,   true,   "<d",     "(b,c]",  "{<d}",           "{(c,d)}",        "{<=b,(c,d)}",    },
        { __LINE__,  {"b","d","()"},  {"d","d","<="},  1,   false,  true,   true,   "<=d",    "(b,d)",  "{<=d}",          "{}",             "{<=b,d}",        },
        { __LINE__,  {"b","d","()"},  {"e","e","<="},  1,   false,  true,   true,   "<=e",    "(b,d)",  "{<=e}",          "{}",             "{<=b,[d,e]}",    },
        { __LINE__,  {"b","d","()"},  {"a","a",">"},   1,   false,  true,   true,   ">a",     "(b,d)",  "{>a}",           "{}",             "{(a,b],>=d}",    },
        { __LINE__,  {"b","d","()"},  {"b","b",">"},   -1,  false,  true,   true,   ">b",     "(b,d)",  "{>b}",           "{}",             "{>=d}",          },
        { __LINE__,  {"b","d","()"},  {"c","c",">"},   -1,  false,  true,   true,   ">b",     "(c,d)",  "{>b}",           "{(b,c]}",        "{(b,c],>=d}",    },
        { __LINE__,  {"b","d","()"},  {"d","d",">"},   -1,  false,  false,  false,  ">b",     "{}",     "{(b,d),>d}",     "{(b,d)}",        "{(b,d),>d}",     },
        { __LINE__,  {"b","d","()"},  {"e","e",">"},   -1,  false,  false,  false,  ">b",     "{}",     "{(b,d),>e}",     "{(b,d)}",        "{(b,d),>e}",     },
        { __LINE__,  {"b","d","()"},  {"a","a",">="},  1,   false,  true,   true,   ">=a",    "(b,d)",  "{>=a}",          "{}",             "{[a,b],>=d}",    },
        { __LINE__,  {"b","d","()"},  {"b","b",">="},  1,   false,  true,   true,   ">=b",    "(b,d)",  "{>=b}",          "{}",             "{b,>=d}",        },
        { __LINE__,  {"b","d","()"},  {"c","c",">="},  -1,  false,  true,   true,   ">b",     "[c,d)",  "{>b}",           "{(b,c)}",        "{(b,c),>=d}",    },
        { __LINE__,  {"b","d","()"},  {"d","d",">="},  -1,  false,  false,  true,   ">b",     "{}",     "{>b}",           "{(b,d)}",        "{>b}",           },
        { __LINE__,  {"b","d","()"},  {"e","e",">="},  -1,  false,  false,  false,  ">b",     "{}",     "{(b,d),>=e}",    "{(b,d)}",        "{(b,d),>=e}",    },
        { __LINE__,  {"b","d","()"},  {"a","b","()"},  1,   false,  false,  false,  "(a,d)",  "{}",     "{(a,b),(b,d)}",  "{(b,d)}",        "{(a,b),(b,d)}",  },
        { __LINE__,  {"b","d","()"},  {"a","c","()"},  1,   false,  true,   true,   "(a,d)",  "(b,c)",  "{(a,d)}",        "{[c,d)}",        "{(a,b],[c,d)}",  },
        { __LINE__,  {"b","d","()"},  {"a","d","()"},  1,   false,  true,   true,   "(a,d)",  "(b,d)",  "{(a,d)}",        "{}",             "{(a,b]}",        },
        { __LINE__,  {"b","d","()"},  {"a","e","()"},  1,   false,  true,   true,   "(a,e)",  "(b,d)",  "{(a,e)}",        "{}",             "{(a,b],[d,e)}",  },
        { __LINE__,  {"b","d","()"},  {"b","c","()"},  1,   true,   true,   true,   "(b,d)",  "(b,c)",  "{(b,d)}",        "{[c,d)}",        "{[c,d)}",        },
        { __LINE__,  {"b","d","()"},  {"b","d","()"},  0,   true,   true,   true,   "(b,d)",  "(b,d)",  "{(b,d)}",        "{}",             "{}",             },
        { __LINE__,  {"b","d","()"},  {"b","e","()"},  -1,  false,  true,   true,   "(b,e)",  "(b,d)",  "{(b,e)}",        "{}",             "{[d,e)}",        },
        { __LINE__,  {"b","d","()"},  {"c","d","()"},  -1,  true,   true,   true,   "(b,d)",  "(c,d)",  "{(b,d)}",        "{(b,c]}",        "{(b,c]}",        },
        { __LINE__,  {"b","d","()"},  {"c","e","()"},  -1,  false,  true,   true,   "(b,e)",  "(c,d)",  "{(b,e)}",        "{(b,c]}",        "{(b,c],[d,e)}",  },
        { __LINE__,  {"b","d","()"},  {"d","e","()"},  -1,  false,  false,  false,  "(b,e)",  "{}",     "{(b,d),(d,e)}",  "{(b,d)}",        "{(b,d),(d,e)}",  },
        { __LINE__,  {"b","d","[)"},  {},              1,   false,  false,  false,  "[b,d)",  "{}",     "{[b,d)}",        "{[b,d)}",        "{[b,d)}",        },
        { __LINE__,  {"b","d","[)"},  {" "," ","*"},   1,   false,  true,   true,   "*",      "[b,d)",  "{*}",            "{}",             "{<b,>=d}",       },
        { __LINE__,  {"b","d","[)"},  {"a"},           1,   false,  false,  false,  "[a,d)",  "{}",     "{a,[b,d)}",      "{[b,d)}",        "{a,[b,d)}",      },
        { __LINE__,  {"b","d","[)"},  {"b"},           1,   true,   true,   true,   "[b,d)",  "b",      "{[b,d)}",        "{(b,d)}",        "{(b,d)}",        },
        { __LINE__,  {"b","d","[)"},  {"c"},           -1,  true,   true,   true,   "[b,d)",  "c",      "{[b,d)}",        "{[b,c),(c,d)}",  "{[b,c),(c,d)}",  },
        { __LINE__,  {"b","d","[)"},  {"d"},           -1,  false,  false,  true,   "[b,d]",  "{}",     "{[b,d]}",        "{[b,d)}",        "{[b,d]}",        },
        { __LINE__,  {"b","d","[)"},  {"e"},           -1,  false,  false,  false,  "[b,e]",  "{}",     "{[b,d),e}",      "{[b,d)}",        "{[b,d),e}",      },
        { __LINE__,  {"b","d","[)"},  {"a","a","<"},   1,   false,  false,  false,  "<d",     "{}",     "{<a,[b,d)}",     "{[b,d)}",        "{<a,[b,d)}",     },
        { __LINE__,  {"b","d","[)"},  {"b","b","<"},   1,   false,  false,  true,   "<d",     "{}",     "{<d}",           "{[b,d)}",        "{<d}",           },
        { __LINE__,  {"b","d","[)"},  {"c","c","<"},   1,   false,  true,   true,   "<d",     "[b,c)",  "{<d}",           "{[c,d)}",        "{<b,[c,d)}",     },
        { __LINE__,  {"b","d","[)"},  {"d","d","<"},   1,   false,  true,   true,   "<d",     "[b,d)",  "{<d}",           "{}",             "{<b}",           },
        { __LINE__,  {"b","d","[)"},  {"e","e","<"},   1,   false,  true,   true,   "<e",     "[b,d)",  "{<e}",           "{}",             "{<b,[d,e)}",     },
        { __LINE__,  {"b","d","[)"},  {"a","a","<="},  1,   false,  false,  false,  "<d",     "{}",     "{<=a,[b,d)}",    "{[b,d)}",        "{<=a,[b,d)}",    },
        { __LINE__,  {"b","d","[)"},  {"b","b","<="},  1,   false,  true,   true,   "<d",     "b",      "{<d}",           "{(b,d)}",        "{<b,(b,d)}",     },
        { __LINE__,  {"b","d","[)"},  {"c","c","<="},  1,   false,  true,   true,   "<d",     "[b,c]",  "{<d}",           "{(c,d)}",        "{<b,(c,d)}",     },
        { __LINE__,  {"b","d","[)"},  {"d","d","<="},  1,   false,  true,   true,   "<=d",    "[b,d)",  "{<=d}",          "{}",             "{<b,d}",         },
        { __LINE__,  {"b","d","[)"},  {"e","e","<="},  1,   false,  true,   true,   "<=e",    "[b,d)",  "{<=e}",          "{}",             "{<b,[d,e]}",     },
        { __LINE__,  {"b","d","[)"},  {"a","a",">"},   1,   false,  true,   true,   ">a",     "[b,d)",  "{>a}",           "{}",             "{(a,b),>=d}",    },
        { __LINE__,  {"b","d","[)"},  {"b","b",">"},   -1,  false,  true,   true,   ">=b",    "(b,d)",  "{>=b}",          "{b}",            "{b,>=d}",        },
        { __LINE__,  {"b","d","[)"},  {"c","c",">"},   -1,  false,  true,   true,   ">=b",    "(c,d)",  "{>=b}",          "{[b,c]}",        "{[b,c],>=d}",    },
        { __LINE__,  {"b","d","[)"},  {"d","d",">"},   -1,  false,  false,  false,  ">=b",    "{}",     "{[b,d),>d}",     "{[b,d)}",        "{[b,d),>d}",     },
        { __LINE__,  {"b","d","[)"},  {"e","e",">"},   -1,  false,  false,  false,  ">=b",    "{}",     "{[b,d),>e}",     "{[b,d)}",        "{[b,d),>e}",     },
        { __LINE__,  {"b","d","[)"},  {"a","a",">="},  1,   false,  true,   true,   ">=a",    "[b,d)",  "{>=a}",          "{}",             "{[a,b),>=d}",    },
        { __LINE__,  {"b","d","[)"},  {"b","b",">="},  -1,  false,  true,   true,   ">=b",    "[b,d)",  "{>=b}",          "{}",             "{>=d}",          },
        { __LINE__,  {"b","d","[)"},  {"c","c",">="},  -1,  false,  true,   true,   ">=b",    "[c,d)",  "{>=b}",          "{[b,c)}",        "{[b,c),>=d}",    },
        { __LINE__,  {"b","d","[)"},  {"d","d",">="},  -1,  false,  false,  true,   ">=b",    "{}",     "{>=b}",          "{[b,d)}",        "{>=b}",          },
        { __LINE__,  {"b","d","[)"},  {"e","e",">="},  -1,  false,  false,  false,  ">=b",    "{}",     "{[b,d),>=e}",    "{[b,d)}",        "{[b,d),>=e}",    },
        { __LINE__,  {"b","d","[)"},  {"a","b","()"},  1,   false,  false,  true,   "(a,d)",  "{}",     "{(a,d)}",        "{[b,d)}",        "{(a,d)}",        },
        { __LINE__,  {"b","d","[)"},  {"a","c","()"},  1,   false,  true,   true,   "(a,d)",  "[b,c)",  "{(a,d)}",        "{[c,d)}",        "{(a,b),[c,d)}",  },
        { __LINE__,  {"b","d","[)"},  {"a","d","()"},  1,   false,  true,   true,   "(a,d)",  "[b,d)",  "{(a,d)}",        "{}",             "{(a,b)}",        },
        { __LINE__,  {"b","d","[)"},  {"a","e","()"},  1,   false,  true,   true,   "(a,e)",  "[b,d)",  "{(a,e)}",        "{}",             "{(a,b),[d,e)}",  },
        { __LINE__,  {"b","d","[)"},  {"b","c","()"},  -1,  true,   true,   true,   "[b,d)",  "(b,c)",  "{[b,d)}",        "{b,[c,d)}",      "{b,[c,d)}",      },
        { __LINE__,  {"b","d","[)"},  {"b","d","()"},  -1,  true,   true,   true,   "[b,d)",  "(b,d)",  "{[b,d)}",        "{b}",            "{b}",            },
        { __LINE__,  {"b","d","[)"},  {"b","e","()"},  -1,  false,  true,   true,   "[b,e)",  "(b,d)",  "{[b,e)}",        "{b}",            "{b,[d,e)}",      },
        { __LINE__,  {"b","d","[)"},  {"c","d","()"},  -1,  true,   true,   true,   "[b,d)",  "(c,d)",  "{[b,d)}",        "{[b,c]}",        "{[b,c]}",        },
        { __LINE__,  {"b","d","[)"},  {"c","e","()"},  -1,  false,  true,   true,   "[b,e)",  "(c,d)",  "{[b,e)}",        "{[b,c]}",        "{[b,c],[d,e)}",  },
        { __LINE__,  {"b","d","[)"},  {"d","e","()"},  -1,  false,  false,  false,  "[b,e)",  "{}",     "{[b,d),(d,e)}",  "{[b,d)}",        "{[b,d),(d,e)}",  },
        { __LINE__,  {"b","d","[)"},  {"a","b","[)"},  1,   false,  false,  true,   "[a,d)",  "{}",     "{[a,d)}",        "{[b,d)}",        "{[a,d)}",        },
        { __LINE__,  {"b","d","[)"},  {"a","c","[)"},  1,   false,  true,   true,   "[a,d)",  "[b,c)",  "{[a,d)}",        "{[c,d)}",        "{[a,b),[c,d)}",  },
        { __LINE__,  {"b","d","[)"},  {"a","d","[)"},  1,   false,  true,   true,   "[a,d)",  "[b,d)",  "{[a,d)}",        "{}",             "{[a,b)}",        },
        { __LINE__,  {"b","d","[)"},  {"a","e","[)"},  1,   false,  true,   true,   "[a,e)",  "[b,d)",  "{[a,e)}",        "{}",             "{[a,b),[d,e)}",  },
        { __LINE__,  {"b","d","[)"},  {"b","c","[)"},  1,   true,   true,   true,   "[b,d)",  "[b,c)",  "{[b,d)}",        "{[c,d)}",        "{[c,d)}",        },
        { __LINE__,  {"b","d","[)"},  {"b","d","[)"},  0,   true,   true,   true,   "[b,d)",  "[b,d)",  "{[b,d)}",        "{}",             "{}",             },
        { __LINE__,  {"b","d","[)"},  {"b","e","[)"},  -1,  false,  true,   true,   "[b,e)",  "[b,d)",  "{[b,e)}",        "{}",             "{[d,e)}",        },
        { __LINE__,  {"b","d","[)"},  {"c","d","[)"},  -1,  true,   true,   true,   "[b,d)",  "[c,d)",  "{[b,d)}",        "{[b,c)}",        "{[b,c)}",        },
        { __LINE__,  {"b","d","[)"},  {"c","e","[)"},  -1,  false,  true,   true,   "[b,e)",  "[c,d)",  "{[b,e)}",        "{[b,c)}",        "{[b,c),[d,e)}",  },
        { __LINE__,  {"b","d","[)"},  {"d","e","[)"},  -1,  false,  false,  true,   "[b,e)",  "{}",     "{[b,e)}",        "{[b,d)}",        "{[b,e)}",        },
        { __LINE__,  {"b","d","(]"},  {},              1,   false,  false,  false,  "(b,d]",  "{}",     "{(b,d]}",        "{(b,d]}",        "{(b,d]}",        },
        { __LINE__,  {"b","d","(]"},  {" "," ","*"},   1,   false,  true,   true,   "*",      "(b,d]",  "{*}",            "{}",             "{<=b,>d}",       },
        { __LINE__,  {"b","d","(]"},  {"a"},           1,   false,  false,  false,  "[a,d]",  "{}",     "{a,(b,d]}",      "{(b,d]}",        "{a,(b,d]}",      },
        { __LINE__,  {"b","d","(]"},  {"b"},           1,   false,  false,  true,   "[b,d]",  "{}",     "{[b,d]}",        "{(b,d]}",        "{[b,d]}",        },
        { __LINE__,  {"b","d","(]"},  {"c"},           -1,  true,   true,   true,   "(b,d]",  "c",      "{(b,d]}",        "{(b,c),(c,d]}",  "{(b,c),(c,d]}",  },
        { __LINE__,  {"b","d","(]"},  {"d"},           -1,  true,   true,   true,   "(b,d]",  "d",      "{(b,d]}",        "{(b,d)}",        "{(b,d)}",        },
        { __LINE__,  {"b","d","(]"},  {"e"},           -1,  false,  false,  false,  "(b,e]",  "{}",     "{(b,d],e}",      "{(b,d]}",        "{(b,d],e}",      },
        { __LINE__,  {"b","d","(]"},  {"a","a","<"},   1,   false,  false,  false,  "<=d",    "{}",     "{<a,(b,d]}",     "{(b,d]}",        "{<a,(b,d]}",     },
        { __LINE__,  {"b","d","(]"},  {"b","b","<"},   1,   false,  false,  false,  "<=d",    "{}",     "{<b,(b,d]}",     "{(b,d]}",        "{<b,(b,d]}",     },
        { __LINE__,  {"b","d","(]"},  {"c","c","<"},   1,   false,  true,   true,   "<=d",    "(b,c)",  "{<=d}",          "{[c,d]}",        "{<=b,[c,d]}",    },
        { __LINE__,  {"b","d","(]"},  {"d","d","<"},   1,   false,  true,   true,   "<=d",    "(b,d)",  "{<=d}",          "{d}",            "{<=b,d}",        },
        { __LINE__,  {"b","d","(]"},  {"e","e","<"},   1,   false,  true,   true,   "<e",     "(b,d]",  "{<e}",           "{}",             "{<=b,(d,e)}",    },
        { __LINE__,  {"b","d","(]"},  {"a","a","<="},  1,   false,  false,  false,  "<=d",    "{}",     "{<=a,(b,d]}",    "{(b,d]}",        "{<=a,(b,d]}",    },
        { __LINE__,  {"b","d","(]"},  {"b","b","<="},  1,   false,  false,  true,   "<=d",    "{}",     "{<=d}",          "{(b,d]}",        "{<=d}",          },
        { __LINE__,  {"b","d","(]"},  {"c","c","<="},  1,   false,  true,   true,   "<=d",    "(b,c]",  "{<=d}",          "{(c,d]}",        "{<=b,(c,d]}",    },
        { __LINE__,  {"b","d","(]"},  {"d","d","<="},  1,   false,  true,   true,   "<=d",    "(b,d]",  "{<=d}",          "{}",             "{<=b}",          },
        { __LINE__,  {"b","d","(]"},  {"e","e","<="},  1,   false,  true,   true,   "<=e",    "(b,d]",  "{<=e}",          "{}",             "{<=b,(d,e]}",    },
        { __LINE__,  {"b","d","(]"},  {"a","a",">"},   1,   false,  true,   true,   ">a",     "(b,d]",  "{>a}",           "{}",             "{(a,b],>d}",     },
        { __LINE__,  {"b","d","(]"},  {"b","b",">"},   -1,  false,  true,   true,   ">b",     "(b,d]",  "{>b}",           "{}",             "{>d}",           },
        { __LINE__,  {"b","d","(]"},  {"c","c",">"},   -1,  false,  true,   true,   ">b",     "(c,d]",  "{>b}",           "{(b,c]}",        "{(b,c],>d}",     },
        { __LINE__,  {"b","d","(]"},  {"d","d",">"},   -1,  false,  false,  true,   ">b",     "{}",     "{>b}",           "{(b,d]}",        "{>b}",           },
        { __LINE__,  {"b","d","(]"},  {"e","e",">"},   -1,  false,  false,  false,  ">b",     "{}",     "{(b,d],>e}",     "{(b,d]}",        "{(b,d],>e}",     },
        { __LINE__,  {"b","d","(]"},  {"a","a",">="},  1,   false,  true,   true,   ">=a",    "(b,d]",  "{>=a}",          "{}",             "{[a,b],>d}",     },
        { __LINE__,  {"b","d","(]"},  {"b","b",">="},  1,   false,  true,   true,   ">=b",    "(b,d]",  "{>=b}",          "{}",             "{b,>d}",         },
        { __LINE__,  {"b","d","(]"},  {"c","c",">="},  -1,  false,  true,   true,   ">b",     "[c,d]",  "{>b}",           "{(b,c)}",        "{(b,c),>d}",     },
        { __LINE__,  {"b","d","(]"},  {"d","d",">="},  -1,  false,  true,   true,   ">b",     "d",      "{>b}",           "{(b,d)}",        "{(b,d),>d}",     },
        { __LINE__,  {"b","d","(]"},  {"e","e",">="},  -1,  false,  false,  false,  ">b",     "{}",     "{(b,d],>=e}",    "{(b,d]}",        "{(b,d],>=e}",    },
        { __LINE__,  {"b","d","(]"},  {"a","b","()"},  1,   false,  false,  false,  "(a,d]",  "{}",     "{(a,b),(b,d]}",  "{(b,d]}",        "{(a,b),(b,d]}",  },
        { __LINE__,  {"b","d","(]"},  {"a","c","()"},  1,   false,  true,   true,   "(a,d]",  "(b,c)",  "{(a,d]}",        "{[c,d]}",        "{(a,b],[c,d]}",  },
        { __LINE__,  {"b","d","(]"},  {"a","d","()"},  1,   false,  true,   true,   "(a,d]",  "(b,d)",  "{(a,d]}",        "{d}",            "{(a,b],d}",      },
        { __LINE__,  {"b","d","(]"},  {"a","e","()"},  1,   false,  true,   true,   "(a,e)",  "(b,d]",  "{(a,e)}",        "{}",             "{(a,b],(d,e)}",  },
        { __LINE__,  {"b","d","(]"},  {"b","c","()"},  1,   true,   true,   true,   "(b,d]",  "(b,c)",  "{(b,d]}",        "{[c,d]}",        "{[c,d]}",        },
        { __LINE__,  {"b","d","(]"},  {"b","d","()"},  1,   true,   true,   true,   "(b,d]",  "(b,d)",  "{(b,d]}",        "{d}",            "{d}",            },
        { __LINE__,  {"b","d","(]"},  {"b","e","()"},  -1,  false,  true,   true,   "(b,e)",  "(b,d]",  "{(b,e)}",        "{}",             "{(d,e)}",        },
        { __LINE__,  {"b","d","(]"},  {"c","d","()"},  -1,  true,   true,   true,   "(b,d]",  "(c,d)",  "{(b,d]}",        "{(b,c],d}",      "{(b,c],d}",      },
        { __LINE__,  {"b","d","(]"},  {"c","e","()"},  -1,  false,  true,   true,   "(b,e)",  "(c,d]",  "{(b,e)}",        "{(b,c]}",        "{(b,c],(d,e)}",  },
        { __LINE__,  {"b","d","(]"},  {"d","e","()"},  -1,  false,  false,  true,   "(b,e)",  "{}",     "{(b,e)}",        "{(b,d]}",        "{(b,e)}",        },
        { __LINE__,  {"b","d","(]"},  {"a","b","[)"},  1,   false,  false,  false,  "[a,d]",  "{}",     "{[a,b),(b,d]}",  "{(b,d]}",        "{[a,b),(b,d]}",  },
        { __LINE__,  {"b","d","(]"},  {"a","c","[)"},  1,   false,  true,   true,   "[a,d]",  "(b,c)",  "{[a,d]}",        "{[c,d]}",        "{[a,b],[c,d]}",  },
        { __LINE__,  {"b","d","(]"},  {"a","d","[)"},  1,   false,  true,   true,   "[a,d]",  "(b,d)",  "{[a,d]}",        "{d}",            "{[a,b],d}",      },
        { __LINE__,  {"b","d","(]"},  {"a","e","[)"},  1,   false,  true,   true,   "[a,e)",  "(b,d]",  "{[a,e)}",        "{}",             "{[a,b],(d,e)}",  },
        { __LINE__,  {"b","d","(]"},  {"b","c","[)"},  1,   false,  true,   true,   "[b,d]",  "(b,c)",  "{[b,d]}",        "{[c,d]}",        "{b,[c,d]}",      },
        { __LINE__,  {"b","d","(]"},  {"b","d","[)"},  1,   false,  true,   true,   "[b,d]",  "(b,d)",  "{[b,d]}",        "{d}",            "{b,d}",          },
        { __LINE__,  {"b","d","(]"},  {"b","e","[)"},  1,   false,  true,   true,   "[b,e)",  "(b,d]",  "{[b,e)}",        "{}",             "{b,(d,e)}",      },
        { __LINE__,  {"b","d","(]"},  {"c","d","[)"},  -1,  true,   true,   true,   "(b,d]",  "[c,d)",  "{(b,d]}",        "{(b,c),d}",      "{(b,c),d}",      },
        { __LINE__,  {"b","d","(]"},  {"c","e","[)"},  -1,  false,  true,   true,   "(b,e)",  "[c,d]",  "{(b,e)}",        "{(b,c)}",        "{(b,c),(d,e)}",  },
        { __LINE__,  {"b","d","(]"},  {"d","e","[)"},  -1,  false,  true,   true,   "(b,e)",  "d",      "{(b,e)}",        "{(b,d)}",        "{(b,d),(d,e)}",  },
        { __LINE__,  {"b","d","(]"},  {"a","b","(]"},  1,   false,  false,  true,   "(a,d]",  "{}",     "{(a,d]}",        "{(b,d]}",        "{(a,d]}",        },
        { __LINE__,  {"b","d","(]"},  {"a","c","(]"},  1,   false,  true,   true,   "(a,d]",  "(b,c]",  "{(a,d]}",        "{(c,d]}",        "{(a,b],(c,d]}",  },
        { __LINE__,  {"b","d","(]"},  {"a","d","(]"},  1,   false,  true,   true,   "(a,d]",  "(b,d]",  "{(a,d]}",        "{}",             "{(a,b]}",        },
        { __LINE__,  {"b","d","(]"},  {"a","e","(]"},  1,   false,  true,   true,   "(a,e]",  "(b,d]",  "{(a,e]}",        "{}",             "{(a,b],(d,e]}",  },
        { __LINE__,  {"b","d","(]"},  {"b","c","(]"},  1,   true,   true,   true,   "(b,d]",  "(b,c]",  "{(b,d]}",        "{(c,d]}",        "{(c,d]}",        },
        { __LINE__,  {"b","d","(]"},  {"b","d","(]"},  0,   true,   true,   true,   "(b,d]",  "(b,d]",  "{(b,d]}",        "{}",             "{}",             },
        { __LINE__,  {"b","d","(]"},  {"b","e","(]"},  -1,  false,  true,   true,   "(b,e]",  "(b,d]",  "{(b,e]}",        "{}",             "{(d,e]}",        },
        { __LINE__,  {"b","d","(]"},  {"c","d","(]"},  -1,  true,   true,   true,   "(b,d]",  "(c,d]",  "{(b,d]}",        "{(b,c]}",        "{(b,c]}",        },
        { __LINE__,  {"b","d","(]"},  {"c","e","(]"},  -1,  false,  true,   true,   "(b,e]",  "(c,d]",  "{(b,e]}",        "{(b,c]}",        "{(b,c],(d,e]}",  },
        { __LINE__,  {"b","d","(]"},  {"d","e","(]"},  -1,  false,  false,  true,   "(b,e]",  "{}",     "{(b,e]}",        "{(b,d]}",        "{(b,e]}",        },
        { __LINE__,  {"b","d","[]"},  {},              1,   false,  false,  false,  "[b,d]",  "{}",     "{[b,d]}",        "{[b,d]}",        "{[b,d]}",        },
        { __LINE__,  {"b","d","[]"},  {" "," ","*"},   1,   false,  true,   true,   "*",      "[b,d]",  "{*}",            "{}",             "{<b,>d}",        },
        { __LINE__,  {"b","d","[]"},  {"a"},           1,   false,  false,  false,  "[a,d]",  "{}",     "{a,[b,d]}",      "{[b,d]}",        "{a,[b,d]}",      },
        { __LINE__,  {"b","d","[]"},  {"b"},           1,   true,   true,   true,   "[b,d]",  "b",      "{[b,d]}",        "{(b,d]}",        "{(b,d]}",        },
        { __LINE__,  {"b","d","[]"},  {"c"},           -1,  true,   true,   true,   "[b,d]",  "c",      "{[b,d]}",        "{[b,c),(c,d]}",  "{[b,c),(c,d]}",  },
        { __LINE__,  {"b","d","[]"},  {"d"},           -1,  true,   true,   true,   "[b,d]",  "d",      "{[b,d]}",        "{[b,d)}",        "{[b,d)}",        },
        { __LINE__,  {"b","d","[]"},  {"e"},           -1,  false,  false,  false,  "[b,e]",  "{}",     "{[b,d],e}",      "{[b,d]}",        "{[b,d],e}",      },
        { __LINE__,  {"b","d","[]"},  {"a","a","<"},   1,   false,  false,  false,  "<=d",    "{}",     "{<a,[b,d]}",     "{[b,d]}",        "{<a,[b,d]}",     },
        { __LINE__,  {"b","d","[]"},  {"b","b","<"},   1,   false,  false,  true,   "<=d",    "{}",     "{<=d}",          "{[b,d]}",        "{<=d}",          },
        { __LINE__,  {"b","d","[]"},  {"c","c","<"},   1,   false,  true,   true,   "<=d",    "[b,c)",  "{<=d}",          "{[c,d]}",        "{<b,[c,d]}",     },
        { __LINE__,  {"b","d","[]"},  {"d","d","<"},   1,   false,  true,   true,   "<=d",    "[b,d)",  "{<=d}",          "{d}",            "{<b,d}",         },
        { __LINE__,  {"b","d","[]"},  {"e","e","<"},   1,   false,  true,   true,   "<e",     "[b,d]",  "{<e}",           "{}",             "{<b,(d,e)}",     },
        { __LINE__,  {"b","d","[]"},  {"a","a","<="},  1,   false,  false,  false,  "<=d",    "{}",     "{<=a,[b,d]}",    "{[b,d]}",        "{<=a,[b,d]}",    },
        { __LINE__,  {"b","d","[]"},  {"b","b","<="},  1,   false,  true,   true,   "<=d",    "b",      "{<=d}",          "{(b,d]}",        "{<b,(b,d]}",     },
        { __LINE__,  {"b","d","[]"},  {"c","c","<="},  1,   false,  true,   true,   "<=d",    "[b,c]",  "{<=d}",          "{(c,d]}",        "{<b,(c,d]}",     },
        { __LINE__,  {"b","d","[]"},  {"d","d","<="},  1,   false,  true,   true,   "<=d",    "[b,d]",  "{<=d}",          "{}",             "{<b}",           },
        { __LINE__,  {"b","d","[]"},  {"e","e","<="},  1,   false,  true,   true,   "<=e",    "[b,d]",  "{<=e}",          "{}",             "{<b,(d,e]}",     },
        { __LINE__,  {"b","d","[]"},  {"a","a",">"},   1,   false,  true,   true,   ">a",     "[b,d]",  "{>a}",           "{}",             "{(a,b),>d}",     },
        { __LINE__,  {"b","d","[]"},  {"b","b",">"},   -1,  false,  true,   true,   ">=b",    "(b,d]",  "{>=b}",          "{b}",            "{b,>d}",         },
        { __LINE__,  {"b","d","[]"},  {"c","c",">"},   -1,  false,  true,   true,   ">=b",    "(c,d]",  "{>=b}",          "{[b,c]}",        "{[b,c],>d}",     },
        { __LINE__,  {"b","d","[]"},  {"d","d",">"},   -1,  false,  false,  true,   ">=b",    "{}",     "{>=b}",          "{[b,d]}",        "{>=b}",          },
        { __LINE__,  {"b","d","[]"},  {"e","e",">"},   -1,  false,  false,  false,  ">=b",    "{}",     "{[b,d],>e}",     "{[b,d]}",        "{[b,d],>e}",     },
        { __LINE__,  {"b","d","[]"},  {"a","a",">="},  1,   false,  true,   true,   ">=a",    "[b,d]",  "{>=a}",          "{}",             "{[a,b),>d}",     },
        { __LINE__,  {"b","d","[]"},  {"b","b",">="},  -1,  false,  true,   true,   ">=b",    "[b,d]",  "{>=b}",          "{}",             "{>d}",           },
        { __LINE__,  {"b","d","[]"},  {"c","c",">="},  -1,  false,  true,   true,   ">=b",    "[c,d]",  "{>=b}",          "{[b,c)}",        "{[b,c),>d}",     },
        { __LINE__,  {"b","d","[]"},  {"d","d",">="},  -1,  false,  true,   true,   ">=b",    "d",      "{>=b}",          "{[b,d)}",        "{[b,d),>d}",     },
        { __LINE__,  {"b","d","[]"},  {"e","e",">="},  -1,  false,  false,  false,  ">=b",    "{}",     "{[b,d],>=e}",    "{[b,d]}",        "{[b,d],>=e}",    },
        { __LINE__,  {"b","d","[]"},  {"a","b","()"},  1,   false,  false,  true,   "(a,d]",  "{}",     "{(a,d]}",        "{[b,d]}",        "{(a,d]}",        },
        { __LINE__,  {"b","d","[]"},  {"a","c","()"},  1,   false,  true,   true,   "(a,d]",  "[b,c)",  "{(a,d]}",        "{[c,d]}",        "{(a,b),[c,d]}",  },
        { __LINE__,  {"b","d","[]"},  {"a","d","()"},  1,   false,  true,   true,   "(a,d]",  "[b,d)",  "{(a,d]}",        "{d}",            "{(a,b),d}",      },
        { __LINE__,  {"b","d","[]"},  {"a","e","()"},  1,   false,  true,   true,   "(a,e)",  "[b,d]",  "{(a,e)}",        "{}",             "{(a,b),(d,e)}",  },
        { __LINE__,  {"b","d","[]"},  {"b","c","()"},  -1,  true,   true,   true,   "[b,d]",  "(b,c)",  "{[b,d]}",        "{b,[c,d]}",      "{b,[c,d]}",      },
        { __LINE__,  {"b","d","[]"},  {"b","d","()"},  -1,  true,   true,   true,   "[b,d]",  "(b,d)",  "{[b,d]}",        "{b,d}",          "{b,d}",          },
        { __LINE__,  {"b","d","[]"},  {"b","e","()"},  -1,  false,  true,   true,   "[b,e)",  "(b,d]",  "{[b,e)}",        "{b}",            "{b,(d,e)}",      },
        { __LINE__,  {"b","d","[]"},  {"c","d","()"},  -1,  true,   true,   true,   "[b,d]",  "(c,d)",  "{[b,d]}",        "{[b,c],d}",      "{[b,c],d}",      },
        { __LINE__,  {"b","d","[]"},  {"c","e","()"},  -1,  false,  true,   true,   "[b,e)",  "(c,d]",  "{[b,e)}",        "{[b,c]}",        "{[b,c],(d,e)}",  },
        { __LINE__,  {"b","d","[]"},  {"d","e","()"},  -1,  false,  false,  true,   "[b,e)",  "{}",     "{[b,e)}",        "{[b,d]}",        "{[b,e)}",        },
        { __LINE__,  {"b","d","[]"},  {"a","b","[)"},  1,   false,  false,  true,   "[a,d]",  "{}",     "{[a,d]}",        "{[b,d]}",        "{[a,d]}",        },
        { __LINE__,  {"b","d","[]"},  {"a","c","[)"},  1,   false,  true,   true,   "[a,d]",  "[b,c)",  "{[a,d]}",        "{[c,d]}",        "{[a,b),[c,d]}",  },
        { __LINE__,  {"b","d","[]"},  {"a","d","[)"},  1,   false,  true,   true,   "[a,d]",  "[b,d)",  "{[a,d]}",        "{d}",            "{[a,b),d}",      },
        { __LINE__,  {"b","d","[]"},  {"a","e","[)"},  1,   false,  true,   true,   "[a,e)",  "[b,d]",  "{[a,e)}",        "{}",             "{[a,b),(d,e)}",  },
        { __LINE__,  {"b","d","[]"},  {"b","c","[)"},  1,   true,   true,   true,   "[b,d]",  "[b,c)",  "{[b,d]}",        "{[c,d]}",        "{[c,d]}",        },
        { __LINE__,  {"b","d","[]"},  {"b","d","[)"},  1,   true,   true,   true,   "[b,d]",  "[b,d)",  "{[b,d]}",        "{d}",            "{d}",            },
        { __LINE__,  {"b","d","[]"},  {"b","e","[)"},  -1,  false,  true,   true,   "[b,e)",  "[b,d]",  "{[b,e)}",        "{}",             "{(d,e)}",        },
        { __LINE__,  {"b","d","[]"},  {"c","d","[)"},  -1,  true,   true,   true,   "[b,d]",  "[c,d)",  "{[b,d]}",        "{[b,c),d}",      "{[b,c),d}",      },
        { __LINE__,  {"b","d","[]"},  {"c","e","[)"},  -1,  false,  true,   true,   "[b,e)",  "[c,d]",  "{[b,e)}",        "{[b,c)}",        "{[b,c),(d,e)}",  },
        { __LINE__,  {"b","d","[]"},  {"d","e","[)"},  -1,  false,  true,   true,   "[b,e)",  "d",      "{[b,e)}",        "{[b,d)}",        "{[b,d),(d,e)}",  },
        { __LINE__,  {"b","d","[]"},  {"a","b","(]"},  1,   false,  true,   true,   "(a,d]",  "b",      "{(a,d]}",        "{(b,d]}",        "{(a,b),(b,d]}",  },
        { __LINE__,  {"b","d","[]"},  {"a","c","(]"},  1,   false,  true,   true,   "(a,d]",  "[b,c]",  "{(a,d]}",        "{(c,d]}",        "{(a,b),(c,d]}",  },
        { __LINE__,  {"b","d","[]"},  {"a","d","(]"},  1,   false,  true,   true,   "(a,d]",  "[b,d]",  "{(a,d]}",        "{}",             "{(a,b)}",        },
        { __LINE__,  {"b","d","[]"},  {"a","e","(]"},  1,   false,  true,   true,   "(a,e]",  "[b,d]",  "{(a,e]}",        "{}",             "{(a,b),(d,e]}",  },
        { __LINE__,  {"b","d","[]"},  {"b","c","(]"},  -1,  true,   true,   true,   "[b,d]",  "(b,c]",  "{[b,d]}",        "{b,(c,d]}",      "{b,(c,d]}",      },
        { __LINE__,  {"b","d","[]"},  {"b","d","(]"},  -1,  true,   true,   true,   "[b,d]",  "(b,d]",  "{[b,d]}",        "{b}",            "{b}",            },
        { __LINE__,  {"b","d","[]"},  {"b","e","(]"},  -1,  false,  true,   true,   "[b,e]",  "(b,d]",  "{[b,e]}",        "{b}",            "{b,(d,e]}",      },
        { __LINE__,  {"b","d","[]"},  {"c","d","(]"},  -1,  true,   true,   true,   "[b,d]",  "(c,d]",  "{[b,d]}",        "{[b,c]}",        "{[b,c]}",        },
        { __LINE__,  {"b","d","[]"},  {"c","e","(]"},  -1,  false,  true,   true,   "[b,e]",  "(c,d]",  "{[b,e]}",        "{[b,c]}",        "{[b,c],(d,e]}",  },
        { __LINE__,  {"b","d","[]"},  {"d","e","(]"},  -1,  false,  false,  true,   "[b,e]",  "{}",     "{[b,e]}",        "{[b,d]}",        "{[b,e]}",        },
        { __LINE__,  {"b","d","[]"},  {"a","a","[]"},  1,   false,  false,  false,  "[a,d]",  "{}",     "{a,[b,d]}",      "{[b,d]}",        "{a,[b,d]}",      },
        { __LINE__,  {"b","d","[]"},  {"a","b","[]"},  1,   false,  true,   true,   "[a,d]",  "b",      "{[a,d]}",        "{(b,d]}",        "{[a,b),(b,d]}",  },
        { __LINE__,  {"b","d","[]"},  {"a","c","[]"},  1,   false,  true,   true,   "[a,d]",  "[b,c]",  "{[a,d]}",        "{(c,d]}",        "{[a,b),(c,d]}",  },
        { __LINE__,  {"b","d","[]"},  {"a","d","[]"},  1,   false,  true,   true,   "[a,d]",  "[b,d]",  "{[a,d]}",        "{}",             "{[a,b)}",        },
        { __LINE__,  {"b","d","[]"},  {"a","e","[]"},  1,   false,  true,   true,   "[a,e]",  "[b,d]",  "{[a,e]}",        "{}",             "{[a,b),(d,e]}",  },
        { __LINE__,  {"b","d","[]"},  {"b","c","[]"},  1,   true,   true,   true,   "[b,d]",  "[b,c]",  "{[b,d]}",        "{(c,d]}",        "{(c,d]}",        },
        { __LINE__,  {"b","d","[]"},  {"b","d","[]"},  0,   true,   true,   true,   "[b,d]",  "[b,d]",  "{[b,d]}",        "{}",             "{}",             },
        { __LINE__,  {"b","d","[]"},  {"b","e","[]"},  -1,  false,  true,   true,   "[b,e]",  "[b,d]",  "{[b,e]}",        "{}",             "{(d,e]}",        },
        { __LINE__,  {"b","d","[]"},  {"c","d","[]"},  -1,  true,   true,   true,   "[b,d]",  "[c,d]",  "{[b,d]}",        "{[b,c)}",        "{[b,c)}",        },
        { __LINE__,  {"b","d","[]"},  {"c","e","[]"},  -1,  false,  true,   true,   "[b,e]",  "[c,d]",  "{[b,e]}",        "{[b,c)}",        "{[b,c),(d,e]}",  },
        { __LINE__,  {"b","d","[]"},  {"d","e","[]"},  -1,  false,  true,   true,   "[b,e]",  "d",      "{[b,e]}",        "{[b,d)}",        "{[b,d),(d,e]}",  },

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
