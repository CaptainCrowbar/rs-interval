#include "rs-intervals/interval.hpp"
#include "rs-intervals/set.hpp"
#include "rs-intervals/types.hpp"
#include "test/unit-test.hpp"
#include <compare>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace RS::Intervals;
using namespace std::literals;

using Itv = Interval<std::string>;
using Set = IntervalSet<std::string>;
using SO = std::strong_ordering;

void test_rs_interval_ordered_interval_basic_properties() {

    Itv in;

    TEST_TYPE(Itv::value_type, std::string);
    TEST_EQUAL(Itv::category, Category::ordered);
    TEST(in.empty());
    TEST(! in);
    TEST_EQUAL(in.str(), "{}");
    TEST_EQUAL(in.match(""), Match::empty);
    TEST_EQUAL(in.match("hello"), Match::empty);
    TEST(! in(""));
    TEST(! in("hello"));

}

void test_rs_interval_ordered_interval_construction() {

    Itv in;
    std::string str;

    TRY(in = Itv());                                      TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = Itv("a"));                                   TRY(str = in.str());  TEST_EQUAL(str, "a");
    TRY(in = Itv("a", Bound::closed, Bound::closed));     TRY(str = in.str());  TEST_EQUAL(str, "a");
    TRY(in = Itv("a", Bound::closed, Bound::open));       TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = Itv("a", Bound::open, Bound::closed));       TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = Itv("a", Bound::closed, Bound::unbound));    TRY(str = in.str());  TEST_EQUAL(str, ">=a");
    TRY(in = Itv("a", Bound::unbound, Bound::closed));    TRY(str = in.str());  TEST_EQUAL(str, "<=a");
    TRY(in = Itv("a", Bound::open, Bound::unbound));      TRY(str = in.str());  TEST_EQUAL(str, ">a");
    TRY(in = Itv("a", Bound::unbound, Bound::open));      TRY(str = in.str());  TEST_EQUAL(str, "<a");
    TRY(in = Itv("a", Bound::unbound, Bound::unbound));   TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = Itv("a", "z"));                              TRY(str = in.str());  TEST_EQUAL(str, "[a,z]");
    TRY(in = Itv("a", "z", Bound::closed));               TRY(str = in.str());  TEST_EQUAL(str, "[a,z]");
    TRY(in = Itv("a", "z", Bound::open));                 TRY(str = in.str());  TEST_EQUAL(str, "(a,z)");
    TRY(in = Itv("a", "z", Bound::closed, Bound::open));  TRY(str = in.str());  TEST_EQUAL(str, "[a,z)");
    TRY(in = Itv("a", "z", Bound::open, Bound::closed));  TRY(str = in.str());  TEST_EQUAL(str, "(a,z]");
    TRY(in = Itv("a", "z", "[]"));                        TRY(str = in.str());  TEST_EQUAL(str, "[a,z]");
    TRY(in = Itv("a", "z", "()"));                        TRY(str = in.str());  TEST_EQUAL(str, "(a,z)");
    TRY(in = Itv("a", "z", "[)"));                        TRY(str = in.str());  TEST_EQUAL(str, "[a,z)");
    TRY(in = Itv("a", "z", "(]"));                        TRY(str = in.str());  TEST_EQUAL(str, "(a,z]");
    TRY(in = Itv("a", "z", "<"));                         TRY(str = in.str());  TEST_EQUAL(str, "<z");
    TRY(in = Itv("a", "z", "<="));                        TRY(str = in.str());  TEST_EQUAL(str, "<=z");
    TRY(in = Itv("a", "z", ">"));                         TRY(str = in.str());  TEST_EQUAL(str, ">a");
    TRY(in = Itv("a", "z", ">="));                        TRY(str = in.str());  TEST_EQUAL(str, ">=a");
    TRY(in = Itv("a", "z", "*"));                         TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = Itv("z", "a"));                              TRY(str = in.str());  TEST_EQUAL(str, "{}");

    TRY(in = make_interval("a"s));                                    TRY(str = in.str());  TEST_EQUAL(str, "a");
    TRY(in = make_interval("a"s, Bound::closed, Bound::closed));      TRY(str = in.str());  TEST_EQUAL(str, "a");
    TRY(in = make_interval("a"s, Bound::closed, Bound::open));        TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = make_interval("a"s, Bound::open, Bound::closed));        TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = make_interval("a"s, Bound::closed, Bound::unbound));     TRY(str = in.str());  TEST_EQUAL(str, ">=a");
    TRY(in = make_interval("a"s, Bound::unbound, Bound::closed));     TRY(str = in.str());  TEST_EQUAL(str, "<=a");
    TRY(in = make_interval("a"s, Bound::open, Bound::unbound));       TRY(str = in.str());  TEST_EQUAL(str, ">a");
    TRY(in = make_interval("a"s, Bound::unbound, Bound::open));       TRY(str = in.str());  TEST_EQUAL(str, "<a");
    TRY(in = make_interval("a"s, Bound::unbound, Bound::unbound));    TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = make_interval("a"s, "z"s));                              TRY(str = in.str());  TEST_EQUAL(str, "[a,z]");
    TRY(in = make_interval("a"s, "z"s, Bound::closed));               TRY(str = in.str());  TEST_EQUAL(str, "[a,z]");
    TRY(in = make_interval("a"s, "z"s, Bound::open));                 TRY(str = in.str());  TEST_EQUAL(str, "(a,z)");
    TRY(in = make_interval("a"s, "z"s, Bound::closed, Bound::open));  TRY(str = in.str());  TEST_EQUAL(str, "[a,z)");
    TRY(in = make_interval("a"s, "z"s, Bound::open, Bound::closed));  TRY(str = in.str());  TEST_EQUAL(str, "(a,z]");
    TRY(in = make_interval("a"s, "z"s, "[]"));                        TRY(str = in.str());  TEST_EQUAL(str, "[a,z]");
    TRY(in = make_interval("a"s, "z"s, "()"));                        TRY(str = in.str());  TEST_EQUAL(str, "(a,z)");
    TRY(in = make_interval("a"s, "z"s, "[)"));                        TRY(str = in.str());  TEST_EQUAL(str, "[a,z)");
    TRY(in = make_interval("a"s, "z"s, "(]"));                        TRY(str = in.str());  TEST_EQUAL(str, "(a,z]");
    TRY(in = make_interval("a"s, "z"s, "<"));                         TRY(str = in.str());  TEST_EQUAL(str, "<z");
    TRY(in = make_interval("a"s, "z"s, "<="));                        TRY(str = in.str());  TEST_EQUAL(str, "<=z");
    TRY(in = make_interval("a"s, "z"s, ">"));                         TRY(str = in.str());  TEST_EQUAL(str, ">a");
    TRY(in = make_interval("a"s, "z"s, ">="));                        TRY(str = in.str());  TEST_EQUAL(str, ">=a");
    TRY(in = make_interval("a"s, "z"s, "*"));                         TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = make_interval("z"s, "a"s));                              TRY(str = in.str());  TEST_EQUAL(str, "{}");

    TRY(in = ordered_interval("z"s, "a"s, Bound::closed));               TRY(str = in.str());  TEST_EQUAL(str, "[a,z]");
    TRY(in = ordered_interval("z"s, "a"s, Bound::open));                 TRY(str = in.str());  TEST_EQUAL(str, "(a,z)");
    TRY(in = ordered_interval("z"s, "a"s, Bound::closed, Bound::open));  TRY(str = in.str());  TEST_EQUAL(str, "(a,z]");
    TRY(in = ordered_interval("z"s, "a"s, Bound::open, Bound::closed));  TRY(str = in.str());  TEST_EQUAL(str, "[a,z)");

}

void test_rs_interval_ordered_interval_string_conversions() {

    Itv in;

    auto fmt = [] (const std::string& x) {
        std::ostringstream out;
        out << "'" << x << "'";
        return out.str();
    };

    TRY(in = Itv::from_string(""));        TEST_EQUAL(in.str(), "{}");     TEST_EQUAL(in.str(fmt), "{}");
    TRY(in = Itv::from_string("{}"));      TEST_EQUAL(in.str(), "{}");     TEST_EQUAL(in.str(fmt), "{}");
    TRY(in = Itv::from_string("*"));       TEST_EQUAL(in.str(), "*");      TEST_EQUAL(in.str(fmt), "*");
    TRY(in = Itv::from_string("a"));       TEST_EQUAL(in.str(), "a");      TEST_EQUAL(in.str(fmt), "'a'");
    TRY(in = Itv::from_string(">e"));      TEST_EQUAL(in.str(), ">e");     TEST_EQUAL(in.str(fmt), ">'e'");
    TRY(in = Itv::from_string("<e"));      TEST_EQUAL(in.str(), "<e");     TEST_EQUAL(in.str(fmt), "<'e'");
    TRY(in = Itv::from_string(">=e"));     TEST_EQUAL(in.str(), ">=e");    TEST_EQUAL(in.str(fmt), ">='e'");
    TRY(in = Itv::from_string("<=e"));     TEST_EQUAL(in.str(), "<=e");    TEST_EQUAL(in.str(fmt), "<='e'");
    TRY(in = Itv::from_string("e+"));      TEST_EQUAL(in.str(), ">=e");    TEST_EQUAL(in.str(fmt), ">='e'");
    TRY(in = Itv::from_string("e-"));      TEST_EQUAL(in.str(), "<=e");    TEST_EQUAL(in.str(fmt), "<='e'");
    TRY(in = Itv::from_string("[e,j]"));   TEST_EQUAL(in.str(), "[e,j]");  TEST_EQUAL(in.str(fmt), "['e','j']");
    TRY(in = Itv::from_string("[e,j)"));   TEST_EQUAL(in.str(), "[e,j)");  TEST_EQUAL(in.str(fmt), "['e','j')");
    TRY(in = Itv::from_string("(e,j]"));   TEST_EQUAL(in.str(), "(e,j]");  TEST_EQUAL(in.str(fmt), "('e','j']");
    TRY(in = Itv::from_string("(e,j)"));   TEST_EQUAL(in.str(), "(e,j)");  TEST_EQUAL(in.str(fmt), "('e','j')");
    TRY(in = Itv::from_string("e-j"));     TEST_EQUAL(in.str(), "[e,j]");  TEST_EQUAL(in.str(fmt), "['e','j']");
    TRY(in = Itv::from_string("e..j"));    TEST_EQUAL(in.str(), "[e,j]");  TEST_EQUAL(in.str(fmt), "['e','j']");
    TRY(in = Itv::from_string("e...j"));   TEST_EQUAL(in.str(), "[e,j]");  TEST_EQUAL(in.str(fmt), "['e','j']");
    TRY(in = Itv::from_string("e..<j"));   TEST_EQUAL(in.str(), "[e,j)");  TEST_EQUAL(in.str(fmt), "['e','j')");
    TRY(in = Itv::from_string("e<..j"));   TEST_EQUAL(in.str(), "(e,j]");  TEST_EQUAL(in.str(fmt), "('e','j']");
    TRY(in = Itv::from_string("e<..<j"));  TEST_EQUAL(in.str(), "(e,j)");  TEST_EQUAL(in.str(fmt), "('e','j')");

}

void test_rs_interval_ordered_interval_order() {

    Itv a, b;
    Order io = {};

    TRY(a = Itv());              TRY(b = Itv());              TRY(io = a.order(b));  TEST_EQUAL(io, Order::equal);              // ...
    TRY(a = Itv());              TRY(b = Itv("a","a",">"));   TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_only);             // bbb
    TRY(a = Itv());              TRY(b = Itv("a","a",">="));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_only);             // bbb
    TRY(a = Itv());              TRY(b = Itv("a","a","<"));   TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_only);             // bbb
    TRY(a = Itv());              TRY(b = Itv("a","a","<="));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_only);             // bbb
    TRY(a = Itv());              TRY(b = Itv("a","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_only);             // bbb
    TRY(a = Itv());              TRY(b = Itv("a","e","()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_only);             // bbb
    TRY(a = Itv());              TRY(b = Itv("a"));           TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_only);             // b
    TRY(a = Itv("a","e","[]"));  TRY(b = Itv("f","g","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_below_b);          // aaa...bbb
    TRY(a = Itv("a","a","<"));   TRY(b = Itv("a","a",">"));   TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_below_b);          // aaa.bbb
    TRY(a = Itv("a","a","<"));   TRY(b = Itv("a","a",">="));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_touches_b);        // aaabbb
    TRY(a = Itv("a","a","<="));  TRY(b = Itv("a","a",">"));   TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_touches_b);        // aaabbb
    TRY(a = Itv("a","e","[]"));  TRY(b = Itv("e","f","()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_touches_b);        // aaabbb
    TRY(a = Itv("a","e","()"));  TRY(b = Itv("e","f","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_touches_b);        // aaabbb
    TRY(a = Itv("a","a","<="));  TRY(b = Itv("a","a",">="));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_overlaps_b);       // aaa*bbb
    TRY(a = Itv("a","e","[]"));  TRY(b = Itv("e","f","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_overlaps_b);       // aaa*bbb
    TRY(a = Itv("a","e","[]"));  TRY(b = Itv("c","f","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_overlaps_b);       // aaa***bbb
    TRY(a = Itv("a","e","()"));  TRY(b = Itv("c","f","()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_overlaps_b);       // aaa***bbb
    TRY(a = Itv("a","e","[)"));  TRY(b = Itv("c","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_overlaps_b);       // aaa***b
    TRY(a = Itv("a","e","[]"));  TRY(b = Itv("a","f","(]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_overlaps_b);       // a***bbb
    TRY(a = Itv("a","e","[)"));  TRY(b = Itv("a","e","(]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_overlaps_b);       // a***b
    TRY(a = Itv("a","e","[]"));  TRY(b = Itv("c","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_extends_below_b);  // aaa***
    TRY(a = Itv("a","e","()"));  TRY(b = Itv("c","e","()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_extends_below_b);  // aaa***
    TRY(a = Itv("a","e","[]"));  TRY(b = Itv("a","e","(]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_extends_below_b);  // a***
    TRY(a = Itv("a","e","[)"));  TRY(b = Itv("a","e","()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_extends_below_b);  // a***
    TRY(a = Itv("a","e","[]"));  TRY(b = Itv("b","d","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_encloses_b);       // aaa***aaa
    TRY(a = Itv("a","e","()"));  TRY(b = Itv("b","d","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_encloses_b);       // aaa***aaa
    TRY(a = Itv("a","e","[]"));  TRY(b = Itv("c","e","[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_encloses_b);       // aaa***a
    TRY(a = Itv("a","e","[]"));  TRY(b = Itv("a","c","(]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_encloses_b);       // a***aaa
    TRY(a = Itv("a","e","[]"));  TRY(b = Itv("a","e","()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_encloses_b);       // a***a
    TRY(a = Itv("a","e","[]"));  TRY(b = Itv("a","f","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_extends_above_a);  // ***bbb
    TRY(a = Itv("a","e","()"));  TRY(b = Itv("a","f","()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_extends_above_a);  // ***bbb
    TRY(a = Itv("a","e","[)"));  TRY(b = Itv("a","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_extends_above_a);  // ***b
    TRY(a = Itv("a","e","[]"));  TRY(b = Itv("a","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::equal);              // ***
    TRY(a = Itv("a","e","()"));  TRY(b = Itv("a","e","()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::equal);              // ***
    TRY(a = Itv("a","e","[)"));  TRY(b = Itv("a","e","[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::equal);              // ***
    TRY(a = Itv("a","e","[)"));  TRY(b = Itv("a","e","[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::equal);              // ***
    TRY(a = Itv("a","e","[]"));  TRY(b = Itv("a","e","[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_extends_above_b);  // ***a
    TRY(a = Itv("a","f","()"));  TRY(b = Itv("a","e","()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_extends_above_b);  // ***aaa
    TRY(a = Itv("a","f","[]"));  TRY(b = Itv("a","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_extends_above_b);  // ***aaa
    TRY(a = Itv("a","e","()"));  TRY(b = Itv("a","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_encloses_a);       // b***b
    TRY(a = Itv("a","c","(]"));  TRY(b = Itv("a","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_encloses_a);       // b***bbb
    TRY(a = Itv("c","e","[)"));  TRY(b = Itv("a","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_encloses_a);       // bbb***b
    TRY(a = Itv("b","d","[]"));  TRY(b = Itv("a","e","()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_encloses_a);       // bbb***bbb
    TRY(a = Itv("b","d","[]"));  TRY(b = Itv("a","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_encloses_a);       // bbb***bbb
    TRY(a = Itv("a","e","()"));  TRY(b = Itv("a","e","[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_extends_below_a);  // b***
    TRY(a = Itv("a","e","(]"));  TRY(b = Itv("a","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_extends_below_a);  // b***
    TRY(a = Itv("c","e","()"));  TRY(b = Itv("a","e","()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_extends_below_a);  // bbb***
    TRY(a = Itv("c","e","[]"));  TRY(b = Itv("a","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_extends_below_a);  // bbb***
    TRY(a = Itv("a","e","(]"));  TRY(b = Itv("a","e","[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_overlaps_a);       // b***a
    TRY(a = Itv("a","f","(]"));  TRY(b = Itv("a","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_overlaps_a);       // b***aaa
    TRY(a = Itv("c","e","[]"));  TRY(b = Itv("a","e","[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_overlaps_a);       // bbb***a
    TRY(a = Itv("c","f","()"));  TRY(b = Itv("a","e","()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_overlaps_a);       // bbb***aaa
    TRY(a = Itv("c","f","[]"));  TRY(b = Itv("a","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_overlaps_a);       // bbb***aaa
    TRY(a = Itv("e","f","[]"));  TRY(b = Itv("a","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_overlaps_a);       // bbb*aaa
    TRY(a = Itv("a","a",">="));  TRY(b = Itv("a","a","<="));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_overlaps_a);       // bbb*aaa
    TRY(a = Itv("e","f","[]"));  TRY(b = Itv("a","e","()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_touches_a);        // bbbaaa
    TRY(a = Itv("e","f","()"));  TRY(b = Itv("a","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_touches_a);        // bbbaaa
    TRY(a = Itv("a","a",">"));   TRY(b = Itv("a","a","<="));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_touches_a);        // bbbaaa
    TRY(a = Itv("a","a",">="));  TRY(b = Itv("a","a","<"));   TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_touches_a);        // bbbaaa
    TRY(a = Itv("a","a",">"));   TRY(b = Itv("a","a","<"));   TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_below_a);          // bbb.aaa
    TRY(a = Itv("f","g","[]"));  TRY(b = Itv("a","e","[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_below_a);          // bbb...aaa
    TRY(a = Itv("a"));           TRY(b = Itv());              TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_only);             // a
    TRY(a = Itv("a","e","()"));  TRY(b = Itv());              TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_only);             // aaa
    TRY(a = Itv("a","e","[]"));  TRY(b = Itv());              TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_only);             // aaa
    TRY(a = Itv("a","a","<="));  TRY(b = Itv());              TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_only);             // aaaa
    TRY(a = Itv("a","a","<"));   TRY(b = Itv());              TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_only);             // aaaa
    TRY(a = Itv("a","a",">="));  TRY(b = Itv());              TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_only);             // aaaa
    TRY(a = Itv("a","a",">"));   TRY(b = Itv());              TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_only);             // aaaa

}

void test_rs_interval_ordered_interval_complement() {

    Itv in;
    Set set;
    std::string str;

    TRY((in = {}));                      TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{*}");
    TRY((in = Itv::all()));              TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{}");
    TRY((in = {"hello"}));               TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<hello,>hello}");
    TRY((in = {"hello","hello","<"}));   TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{>=hello}");
    TRY((in = {"hello","hello","<="}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{>hello}");
    TRY((in = {"hello","hello",">"}));   TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<=hello}");
    TRY((in = {"hello","hello",">="}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<hello}");
    TRY((in = {"hello","world","()"}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<=hello,>=world}");
    TRY((in = {"hello","world","(]"}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<=hello,>world}");
    TRY((in = {"hello","world","[)"}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<hello,>=world}");
    TRY((in = {"hello","world","[]"}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<hello,>world}");

}

void test_rs_interval_ordered_interval_binary_operations() {

    struct test_info {
        int line;
        Itv lhs;
        Itv rhs;
        SO compare;
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

        // line      lhs              rhs              cmp           incl    over    touch   envel     inter     union             diff              symm              --
        { __LINE__,  {},              {},              SO::equal,    false,  false,  false,  "{}",     "{}",     "{}",             "{}",             "{}",             },
        { __LINE__,  {" "," ","*"},   {},              SO::greater,  false,  false,  false,  "*",      "{}",     "{*}",            "{*}",            "{*}",            },
        { __LINE__,  {" "," ","*"},   {" "," ","*"},   SO::equal,    true,   true,   true,   "*",      "*",      "{*}",            "{}",             "{}",             },
        { __LINE__,  {"b"},           {},              SO::greater,  false,  false,  false,  "b",      "{}",     "{b}",            "{b}",            "{b}",            },
        { __LINE__,  {"b"},           {" "," ","*"},   SO::greater,  false,  true,   true,   "*",      "b",      "{*}",            "{}",             "{<b,>b}",        },
        { __LINE__,  {"b"},           {"a"},           SO::greater,  false,  false,  false,  "[a,b]",  "{}",     "{a,b}",          "{b}",            "{a,b}",          },
        { __LINE__,  {"b"},           {"b"},           SO::equal,    true,   true,   true,   "b",      "b",      "{b}",            "{}",             "{}",             },
        { __LINE__,  {"b"},           {"c"},           SO::less,     false,  false,  false,  "[b,c]",  "{}",     "{b,c}",          "{b}",            "{b,c}",          },
        { __LINE__,  {"b","b","<"},   {},              SO::greater,  false,  false,  false,  "<b",     "{}",     "{<b}",           "{<b}",           "{<b}",           },
        { __LINE__,  {"b","b","<"},   {" "," ","*"},   SO::less,     false,  true,   true,   "*",      "<b",     "{*}",            "{}",             "{>=b}",          },
        { __LINE__,  {"b","b","<"},   {"a"},           SO::less,     true,   true,   true,   "<b",     "a",      "{<b}",           "{<a,(a,b)}",     "{<a,(a,b)}",     },
        { __LINE__,  {"b","b","<"},   {"b"},           SO::less,     false,  false,  true,   "<=b",    "{}",     "{<=b}",          "{<b}",           "{<=b}",          },
        { __LINE__,  {"b","b","<"},   {"c"},           SO::less,     false,  false,  false,  "<=c",    "{}",     "{<b,c}",         "{<b}",           "{<b,c}",         },
        { __LINE__,  {"b","b","<"},   {"a","a","<"},   SO::greater,  true,   true,   true,   "<b",     "<a",     "{<b}",           "{[a,b)}",        "{[a,b)}",        },
        { __LINE__,  {"b","b","<"},   {"b","b","<"},   SO::equal,    true,   true,   true,   "<b",     "<b",     "{<b}",           "{}",             "{}",             },
        { __LINE__,  {"b","b","<"},   {"c","c","<"},   SO::less,     false,  true,   true,   "<c",     "<b",     "{<c}",           "{}",             "{[b,c)}",        },
        { __LINE__,  {"b","b","<="},  {},              SO::greater,  false,  false,  false,  "<=b",    "{}",     "{<=b}",          "{<=b}",          "{<=b}",          },
        { __LINE__,  {"b","b","<="},  {" "," ","*"},   SO::less,     false,  true,   true,   "*",      "<=b",    "{*}",            "{}",             "{>b}",           },
        { __LINE__,  {"b","b","<="},  {"a"},           SO::less,     true,   true,   true,   "<=b",    "a",      "{<=b}",          "{<a,(a,b]}",     "{<a,(a,b]}",     },
        { __LINE__,  {"b","b","<="},  {"b"},           SO::less,     true,   true,   true,   "<=b",    "b",      "{<=b}",          "{<b}",           "{<b}",           },
        { __LINE__,  {"b","b","<="},  {"c"},           SO::less,     false,  false,  false,  "<=c",    "{}",     "{<=b,c}",        "{<=b}",          "{<=b,c}",        },
        { __LINE__,  {"b","b","<="},  {"a","a","<"},   SO::greater,  true,   true,   true,   "<=b",    "<a",     "{<=b}",          "{[a,b]}",        "{[a,b]}",        },
        { __LINE__,  {"b","b","<="},  {"b","b","<"},   SO::greater,  true,   true,   true,   "<=b",    "<b",     "{<=b}",          "{b}",            "{b}",            },
        { __LINE__,  {"b","b","<="},  {"c","c","<"},   SO::less,     false,  true,   true,   "<c",     "<=b",    "{<c}",           "{}",             "{(b,c)}",        },
        { __LINE__,  {"b","b","<="},  {"a","a","<="},  SO::greater,  true,   true,   true,   "<=b",    "<=a",    "{<=b}",          "{(a,b]}",        "{(a,b]}",        },
        { __LINE__,  {"b","b","<="},  {"b","b","<="},  SO::equal,    true,   true,   true,   "<=b",    "<=b",    "{<=b}",          "{}",             "{}",             },
        { __LINE__,  {"b","b","<="},  {"c","c","<="},  SO::less,     false,  true,   true,   "<=c",    "<=b",    "{<=c}",          "{}",             "{(b,c]}",        },
        { __LINE__,  {"b","b",">"},   {},              SO::greater,  false,  false,  false,  ">b",     "{}",     "{>b}",           "{>b}",           "{>b}",           },
        { __LINE__,  {"b","b",">"},   {" "," ","*"},   SO::greater,  false,  true,   true,   "*",      ">b",     "{*}",            "{}",             "{<=b}",          },
        { __LINE__,  {"b","b",">"},   {"a"},           SO::greater,  false,  false,  false,  ">=a",    "{}",     "{a,>b}",         "{>b}",           "{a,>b}",         },
        { __LINE__,  {"b","b",">"},   {"b"},           SO::greater,  false,  false,  true,   ">=b",    "{}",     "{>=b}",          "{>b}",           "{>=b}",          },
        { __LINE__,  {"b","b",">"},   {"c"},           SO::less,     true,   true,   true,   ">b",     "c",      "{>b}",           "{(b,c),>c}",     "{(b,c),>c}",     },
        { __LINE__,  {"b","b",">"},   {"a","a","<"},   SO::greater,  false,  false,  false,  "*",      "{}",     "{<a,>b}",        "{>b}",           "{<a,>b}",        },
        { __LINE__,  {"b","b",">"},   {"b","b","<"},   SO::greater,  false,  false,  false,  "*",      "{}",     "{<b,>b}",        "{>b}",           "{<b,>b}",        },
        { __LINE__,  {"b","b",">"},   {"c","c","<"},   SO::greater,  false,  true,   true,   "*",      "(b,c)",  "{*}",            "{>=c}",          "{<=b,>=c}",      },
        { __LINE__,  {"b","b",">"},   {"a","a","<="},  SO::greater,  false,  false,  false,  "*",      "{}",     "{<=a,>b}",       "{>b}",           "{<=a,>b}",       },
        { __LINE__,  {"b","b",">"},   {"b","b","<="},  SO::greater,  false,  false,  true,   "*",      "{}",     "{*}",            "{>b}",           "{*}",            },
        { __LINE__,  {"b","b",">"},   {"c","c","<="},  SO::greater,  false,  true,   true,   "*",      "(b,c]",  "{*}",            "{>c}",           "{<=b,>c}",       },
        { __LINE__,  {"b","b",">"},   {"a","a",">"},   SO::greater,  false,  true,   true,   ">a",     ">b",     "{>a}",           "{}",             "{(a,b]}",        },
        { __LINE__,  {"b","b",">"},   {"b","b",">"},   SO::equal,    true,   true,   true,   ">b",     ">b",     "{>b}",           "{}",             "{}",             },
        { __LINE__,  {"b","b",">"},   {"c","c",">"},   SO::less,     true,   true,   true,   ">b",     ">c",     "{>b}",           "{(b,c]}",        "{(b,c]}",        },
        { __LINE__,  {"b","b",">="},  {},              SO::greater,  false,  false,  false,  ">=b",    "{}",     "{>=b}",          "{>=b}",          "{>=b}",          },
        { __LINE__,  {"b","b",">="},  {" "," ","*"},   SO::greater,  false,  true,   true,   "*",      ">=b",    "{*}",            "{}",             "{<b}",           },
        { __LINE__,  {"b","b",">="},  {"a"},           SO::greater,  false,  false,  false,  ">=a",    "{}",     "{a,>=b}",        "{>=b}",          "{a,>=b}",        },
        { __LINE__,  {"b","b",">="},  {"b"},           SO::greater,  true,   true,   true,   ">=b",    "b",      "{>=b}",          "{>b}",           "{>b}",           },
        { __LINE__,  {"b","b",">="},  {"c"},           SO::less,     true,   true,   true,   ">=b",    "c",      "{>=b}",          "{[b,c),>c}",     "{[b,c),>c}",     },
        { __LINE__,  {"b","b",">="},  {"a","a","<"},   SO::greater,  false,  false,  false,  "*",      "{}",     "{<a,>=b}",       "{>=b}",          "{<a,>=b}",       },
        { __LINE__,  {"b","b",">="},  {"b","b","<"},   SO::greater,  false,  false,  true,   "*",      "{}",     "{*}",            "{>=b}",          "{*}",            },
        { __LINE__,  {"b","b",">="},  {"c","c","<"},   SO::greater,  false,  true,   true,   "*",      "[b,c)",  "{*}",            "{>=c}",          "{<b,>=c}",       },
        { __LINE__,  {"b","b",">="},  {"a","a","<="},  SO::greater,  false,  false,  false,  "*",      "{}",     "{<=a,>=b}",      "{>=b}",          "{<=a,>=b}",      },
        { __LINE__,  {"b","b",">="},  {"b","b","<="},  SO::greater,  false,  true,   true,   "*",      "b",      "{*}",            "{>b}",           "{<b,>b}",        },
        { __LINE__,  {"b","b",">="},  {"c","c","<="},  SO::greater,  false,  true,   true,   "*",      "[b,c]",  "{*}",            "{>c}",           "{<b,>c}",        },
        { __LINE__,  {"b","b",">="},  {"a","a",">"},   SO::greater,  false,  true,   true,   ">a",     ">=b",    "{>a}",           "{}",             "{(a,b)}",        },
        { __LINE__,  {"b","b",">="},  {"b","b",">"},   SO::less,     true,   true,   true,   ">=b",    ">b",     "{>=b}",          "{b}",            "{b}",            },
        { __LINE__,  {"b","b",">="},  {"c","c",">"},   SO::less,     true,   true,   true,   ">=b",    ">c",     "{>=b}",          "{[b,c]}",        "{[b,c]}",        },
        { __LINE__,  {"b","b",">="},  {"a","a",">="},  SO::greater,  false,  true,   true,   ">=a",    ">=b",    "{>=a}",          "{}",             "{[a,b)}",        },
        { __LINE__,  {"b","b",">="},  {"b","b",">="},  SO::equal,    true,   true,   true,   ">=b",    ">=b",    "{>=b}",          "{}",             "{}",             },
        { __LINE__,  {"b","b",">="},  {"c","c",">="},  SO::less,     true,   true,   true,   ">=b",    ">=c",    "{>=b}",          "{[b,c)}",        "{[b,c)}",        },
        { __LINE__,  {"b","d","()"},  {},              SO::greater,  false,  false,  false,  "(b,d)",  "{}",     "{(b,d)}",        "{(b,d)}",        "{(b,d)}",        },
        { __LINE__,  {"b","d","()"},  {" "," ","*"},   SO::greater,  false,  true,   true,   "*",      "(b,d)",  "{*}",            "{}",             "{<=b,>=d}",      },
        { __LINE__,  {"b","d","()"},  {"a"},           SO::greater,  false,  false,  false,  "[a,d)",  "{}",     "{a,(b,d)}",      "{(b,d)}",        "{a,(b,d)}",      },
        { __LINE__,  {"b","d","()"},  {"b"},           SO::greater,  false,  false,  true,   "[b,d)",  "{}",     "{[b,d)}",        "{(b,d)}",        "{[b,d)}",        },
        { __LINE__,  {"b","d","()"},  {"c"},           SO::less,     true,   true,   true,   "(b,d)",  "c",      "{(b,d)}",        "{(b,c),(c,d)}",  "{(b,c),(c,d)}",  },
        { __LINE__,  {"b","d","()"},  {"d"},           SO::less,     false,  false,  true,   "(b,d]",  "{}",     "{(b,d]}",        "{(b,d)}",        "{(b,d]}",        },
        { __LINE__,  {"b","d","()"},  {"e"},           SO::less,     false,  false,  false,  "(b,e]",  "{}",     "{(b,d),e}",      "{(b,d)}",        "{(b,d),e}",      },
        { __LINE__,  {"b","d","()"},  {"a","a","<"},   SO::greater,  false,  false,  false,  "<d",     "{}",     "{<a,(b,d)}",     "{(b,d)}",        "{<a,(b,d)}",     },
        { __LINE__,  {"b","d","()"},  {"b","b","<"},   SO::greater,  false,  false,  false,  "<d",     "{}",     "{<b,(b,d)}",     "{(b,d)}",        "{<b,(b,d)}",     },
        { __LINE__,  {"b","d","()"},  {"c","c","<"},   SO::greater,  false,  true,   true,   "<d",     "(b,c)",  "{<d}",           "{[c,d)}",        "{<=b,[c,d)}",    },
        { __LINE__,  {"b","d","()"},  {"d","d","<"},   SO::greater,  false,  true,   true,   "<d",     "(b,d)",  "{<d}",           "{}",             "{<=b}",          },
        { __LINE__,  {"b","d","()"},  {"e","e","<"},   SO::greater,  false,  true,   true,   "<e",     "(b,d)",  "{<e}",           "{}",             "{<=b,[d,e)}",    },
        { __LINE__,  {"b","d","()"},  {"a","a","<="},  SO::greater,  false,  false,  false,  "<d",     "{}",     "{<=a,(b,d)}",    "{(b,d)}",        "{<=a,(b,d)}",    },
        { __LINE__,  {"b","d","()"},  {"b","b","<="},  SO::greater,  false,  false,  true,   "<d",     "{}",     "{<d}",           "{(b,d)}",        "{<d}",           },
        { __LINE__,  {"b","d","()"},  {"c","c","<="},  SO::greater,  false,  true,   true,   "<d",     "(b,c]",  "{<d}",           "{(c,d)}",        "{<=b,(c,d)}",    },
        { __LINE__,  {"b","d","()"},  {"d","d","<="},  SO::greater,  false,  true,   true,   "<=d",    "(b,d)",  "{<=d}",          "{}",             "{<=b,d}",        },
        { __LINE__,  {"b","d","()"},  {"e","e","<="},  SO::greater,  false,  true,   true,   "<=e",    "(b,d)",  "{<=e}",          "{}",             "{<=b,[d,e]}",    },
        { __LINE__,  {"b","d","()"},  {"a","a",">"},   SO::greater,  false,  true,   true,   ">a",     "(b,d)",  "{>a}",           "{}",             "{(a,b],>=d}",    },
        { __LINE__,  {"b","d","()"},  {"b","b",">"},   SO::less,     false,  true,   true,   ">b",     "(b,d)",  "{>b}",           "{}",             "{>=d}",          },
        { __LINE__,  {"b","d","()"},  {"c","c",">"},   SO::less,     false,  true,   true,   ">b",     "(c,d)",  "{>b}",           "{(b,c]}",        "{(b,c],>=d}",    },
        { __LINE__,  {"b","d","()"},  {"d","d",">"},   SO::less,     false,  false,  false,  ">b",     "{}",     "{(b,d),>d}",     "{(b,d)}",        "{(b,d),>d}",     },
        { __LINE__,  {"b","d","()"},  {"e","e",">"},   SO::less,     false,  false,  false,  ">b",     "{}",     "{(b,d),>e}",     "{(b,d)}",        "{(b,d),>e}",     },
        { __LINE__,  {"b","d","()"},  {"a","a",">="},  SO::greater,  false,  true,   true,   ">=a",    "(b,d)",  "{>=a}",          "{}",             "{[a,b],>=d}",    },
        { __LINE__,  {"b","d","()"},  {"b","b",">="},  SO::greater,  false,  true,   true,   ">=b",    "(b,d)",  "{>=b}",          "{}",             "{b,>=d}",        },
        { __LINE__,  {"b","d","()"},  {"c","c",">="},  SO::less,     false,  true,   true,   ">b",     "[c,d)",  "{>b}",           "{(b,c)}",        "{(b,c),>=d}",    },
        { __LINE__,  {"b","d","()"},  {"d","d",">="},  SO::less,     false,  false,  true,   ">b",     "{}",     "{>b}",           "{(b,d)}",        "{>b}",           },
        { __LINE__,  {"b","d","()"},  {"e","e",">="},  SO::less,     false,  false,  false,  ">b",     "{}",     "{(b,d),>=e}",    "{(b,d)}",        "{(b,d),>=e}",    },
        { __LINE__,  {"b","d","()"},  {"a","b","()"},  SO::greater,  false,  false,  false,  "(a,d)",  "{}",     "{(a,b),(b,d)}",  "{(b,d)}",        "{(a,b),(b,d)}",  },
        { __LINE__,  {"b","d","()"},  {"a","c","()"},  SO::greater,  false,  true,   true,   "(a,d)",  "(b,c)",  "{(a,d)}",        "{[c,d)}",        "{(a,b],[c,d)}",  },
        { __LINE__,  {"b","d","()"},  {"a","d","()"},  SO::greater,  false,  true,   true,   "(a,d)",  "(b,d)",  "{(a,d)}",        "{}",             "{(a,b]}",        },
        { __LINE__,  {"b","d","()"},  {"a","e","()"},  SO::greater,  false,  true,   true,   "(a,e)",  "(b,d)",  "{(a,e)}",        "{}",             "{(a,b],[d,e)}",  },
        { __LINE__,  {"b","d","()"},  {"b","c","()"},  SO::greater,  true,   true,   true,   "(b,d)",  "(b,c)",  "{(b,d)}",        "{[c,d)}",        "{[c,d)}",        },
        { __LINE__,  {"b","d","()"},  {"b","d","()"},  SO::equal,    true,   true,   true,   "(b,d)",  "(b,d)",  "{(b,d)}",        "{}",             "{}",             },
        { __LINE__,  {"b","d","()"},  {"b","e","()"},  SO::less,     false,  true,   true,   "(b,e)",  "(b,d)",  "{(b,e)}",        "{}",             "{[d,e)}",        },
        { __LINE__,  {"b","d","()"},  {"c","d","()"},  SO::less,     true,   true,   true,   "(b,d)",  "(c,d)",  "{(b,d)}",        "{(b,c]}",        "{(b,c]}",        },
        { __LINE__,  {"b","d","()"},  {"c","e","()"},  SO::less,     false,  true,   true,   "(b,e)",  "(c,d)",  "{(b,e)}",        "{(b,c]}",        "{(b,c],[d,e)}",  },
        { __LINE__,  {"b","d","()"},  {"d","e","()"},  SO::less,     false,  false,  false,  "(b,e)",  "{}",     "{(b,d),(d,e)}",  "{(b,d)}",        "{(b,d),(d,e)}",  },
        { __LINE__,  {"b","d","[)"},  {},              SO::greater,  false,  false,  false,  "[b,d)",  "{}",     "{[b,d)}",        "{[b,d)}",        "{[b,d)}",        },
        { __LINE__,  {"b","d","[)"},  {" "," ","*"},   SO::greater,  false,  true,   true,   "*",      "[b,d)",  "{*}",            "{}",             "{<b,>=d}",       },
        { __LINE__,  {"b","d","[)"},  {"a"},           SO::greater,  false,  false,  false,  "[a,d)",  "{}",     "{a,[b,d)}",      "{[b,d)}",        "{a,[b,d)}",      },
        { __LINE__,  {"b","d","[)"},  {"b"},           SO::greater,  true,   true,   true,   "[b,d)",  "b",      "{[b,d)}",        "{(b,d)}",        "{(b,d)}",        },
        { __LINE__,  {"b","d","[)"},  {"c"},           SO::less,     true,   true,   true,   "[b,d)",  "c",      "{[b,d)}",        "{[b,c),(c,d)}",  "{[b,c),(c,d)}",  },
        { __LINE__,  {"b","d","[)"},  {"d"},           SO::less,     false,  false,  true,   "[b,d]",  "{}",     "{[b,d]}",        "{[b,d)}",        "{[b,d]}",        },
        { __LINE__,  {"b","d","[)"},  {"e"},           SO::less,     false,  false,  false,  "[b,e]",  "{}",     "{[b,d),e}",      "{[b,d)}",        "{[b,d),e}",      },
        { __LINE__,  {"b","d","[)"},  {"a","a","<"},   SO::greater,  false,  false,  false,  "<d",     "{}",     "{<a,[b,d)}",     "{[b,d)}",        "{<a,[b,d)}",     },
        { __LINE__,  {"b","d","[)"},  {"b","b","<"},   SO::greater,  false,  false,  true,   "<d",     "{}",     "{<d}",           "{[b,d)}",        "{<d}",           },
        { __LINE__,  {"b","d","[)"},  {"c","c","<"},   SO::greater,  false,  true,   true,   "<d",     "[b,c)",  "{<d}",           "{[c,d)}",        "{<b,[c,d)}",     },
        { __LINE__,  {"b","d","[)"},  {"d","d","<"},   SO::greater,  false,  true,   true,   "<d",     "[b,d)",  "{<d}",           "{}",             "{<b}",           },
        { __LINE__,  {"b","d","[)"},  {"e","e","<"},   SO::greater,  false,  true,   true,   "<e",     "[b,d)",  "{<e}",           "{}",             "{<b,[d,e)}",     },
        { __LINE__,  {"b","d","[)"},  {"a","a","<="},  SO::greater,  false,  false,  false,  "<d",     "{}",     "{<=a,[b,d)}",    "{[b,d)}",        "{<=a,[b,d)}",    },
        { __LINE__,  {"b","d","[)"},  {"b","b","<="},  SO::greater,  false,  true,   true,   "<d",     "b",      "{<d}",           "{(b,d)}",        "{<b,(b,d)}",     },
        { __LINE__,  {"b","d","[)"},  {"c","c","<="},  SO::greater,  false,  true,   true,   "<d",     "[b,c]",  "{<d}",           "{(c,d)}",        "{<b,(c,d)}",     },
        { __LINE__,  {"b","d","[)"},  {"d","d","<="},  SO::greater,  false,  true,   true,   "<=d",    "[b,d)",  "{<=d}",          "{}",             "{<b,d}",         },
        { __LINE__,  {"b","d","[)"},  {"e","e","<="},  SO::greater,  false,  true,   true,   "<=e",    "[b,d)",  "{<=e}",          "{}",             "{<b,[d,e]}",     },
        { __LINE__,  {"b","d","[)"},  {"a","a",">"},   SO::greater,  false,  true,   true,   ">a",     "[b,d)",  "{>a}",           "{}",             "{(a,b),>=d}",    },
        { __LINE__,  {"b","d","[)"},  {"b","b",">"},   SO::less,     false,  true,   true,   ">=b",    "(b,d)",  "{>=b}",          "{b}",            "{b,>=d}",        },
        { __LINE__,  {"b","d","[)"},  {"c","c",">"},   SO::less,     false,  true,   true,   ">=b",    "(c,d)",  "{>=b}",          "{[b,c]}",        "{[b,c],>=d}",    },
        { __LINE__,  {"b","d","[)"},  {"d","d",">"},   SO::less,     false,  false,  false,  ">=b",    "{}",     "{[b,d),>d}",     "{[b,d)}",        "{[b,d),>d}",     },
        { __LINE__,  {"b","d","[)"},  {"e","e",">"},   SO::less,     false,  false,  false,  ">=b",    "{}",     "{[b,d),>e}",     "{[b,d)}",        "{[b,d),>e}",     },
        { __LINE__,  {"b","d","[)"},  {"a","a",">="},  SO::greater,  false,  true,   true,   ">=a",    "[b,d)",  "{>=a}",          "{}",             "{[a,b),>=d}",    },
        { __LINE__,  {"b","d","[)"},  {"b","b",">="},  SO::less,     false,  true,   true,   ">=b",    "[b,d)",  "{>=b}",          "{}",             "{>=d}",          },
        { __LINE__,  {"b","d","[)"},  {"c","c",">="},  SO::less,     false,  true,   true,   ">=b",    "[c,d)",  "{>=b}",          "{[b,c)}",        "{[b,c),>=d}",    },
        { __LINE__,  {"b","d","[)"},  {"d","d",">="},  SO::less,     false,  false,  true,   ">=b",    "{}",     "{>=b}",          "{[b,d)}",        "{>=b}",          },
        { __LINE__,  {"b","d","[)"},  {"e","e",">="},  SO::less,     false,  false,  false,  ">=b",    "{}",     "{[b,d),>=e}",    "{[b,d)}",        "{[b,d),>=e}",    },
        { __LINE__,  {"b","d","[)"},  {"a","b","()"},  SO::greater,  false,  false,  true,   "(a,d)",  "{}",     "{(a,d)}",        "{[b,d)}",        "{(a,d)}",        },
        { __LINE__,  {"b","d","[)"},  {"a","c","()"},  SO::greater,  false,  true,   true,   "(a,d)",  "[b,c)",  "{(a,d)}",        "{[c,d)}",        "{(a,b),[c,d)}",  },
        { __LINE__,  {"b","d","[)"},  {"a","d","()"},  SO::greater,  false,  true,   true,   "(a,d)",  "[b,d)",  "{(a,d)}",        "{}",             "{(a,b)}",        },
        { __LINE__,  {"b","d","[)"},  {"a","e","()"},  SO::greater,  false,  true,   true,   "(a,e)",  "[b,d)",  "{(a,e)}",        "{}",             "{(a,b),[d,e)}",  },
        { __LINE__,  {"b","d","[)"},  {"b","c","()"},  SO::less,     true,   true,   true,   "[b,d)",  "(b,c)",  "{[b,d)}",        "{b,[c,d)}",      "{b,[c,d)}",      },
        { __LINE__,  {"b","d","[)"},  {"b","d","()"},  SO::less,     true,   true,   true,   "[b,d)",  "(b,d)",  "{[b,d)}",        "{b}",            "{b}",            },
        { __LINE__,  {"b","d","[)"},  {"b","e","()"},  SO::less,     false,  true,   true,   "[b,e)",  "(b,d)",  "{[b,e)}",        "{b}",            "{b,[d,e)}",      },
        { __LINE__,  {"b","d","[)"},  {"c","d","()"},  SO::less,     true,   true,   true,   "[b,d)",  "(c,d)",  "{[b,d)}",        "{[b,c]}",        "{[b,c]}",        },
        { __LINE__,  {"b","d","[)"},  {"c","e","()"},  SO::less,     false,  true,   true,   "[b,e)",  "(c,d)",  "{[b,e)}",        "{[b,c]}",        "{[b,c],[d,e)}",  },
        { __LINE__,  {"b","d","[)"},  {"d","e","()"},  SO::less,     false,  false,  false,  "[b,e)",  "{}",     "{[b,d),(d,e)}",  "{[b,d)}",        "{[b,d),(d,e)}",  },
        { __LINE__,  {"b","d","[)"},  {"a","b","[)"},  SO::greater,  false,  false,  true,   "[a,d)",  "{}",     "{[a,d)}",        "{[b,d)}",        "{[a,d)}",        },
        { __LINE__,  {"b","d","[)"},  {"a","c","[)"},  SO::greater,  false,  true,   true,   "[a,d)",  "[b,c)",  "{[a,d)}",        "{[c,d)}",        "{[a,b),[c,d)}",  },
        { __LINE__,  {"b","d","[)"},  {"a","d","[)"},  SO::greater,  false,  true,   true,   "[a,d)",  "[b,d)",  "{[a,d)}",        "{}",             "{[a,b)}",        },
        { __LINE__,  {"b","d","[)"},  {"a","e","[)"},  SO::greater,  false,  true,   true,   "[a,e)",  "[b,d)",  "{[a,e)}",        "{}",             "{[a,b),[d,e)}",  },
        { __LINE__,  {"b","d","[)"},  {"b","c","[)"},  SO::greater,  true,   true,   true,   "[b,d)",  "[b,c)",  "{[b,d)}",        "{[c,d)}",        "{[c,d)}",        },
        { __LINE__,  {"b","d","[)"},  {"b","d","[)"},  SO::equal,    true,   true,   true,   "[b,d)",  "[b,d)",  "{[b,d)}",        "{}",             "{}",             },
        { __LINE__,  {"b","d","[)"},  {"b","e","[)"},  SO::less,     false,  true,   true,   "[b,e)",  "[b,d)",  "{[b,e)}",        "{}",             "{[d,e)}",        },
        { __LINE__,  {"b","d","[)"},  {"c","d","[)"},  SO::less,     true,   true,   true,   "[b,d)",  "[c,d)",  "{[b,d)}",        "{[b,c)}",        "{[b,c)}",        },
        { __LINE__,  {"b","d","[)"},  {"c","e","[)"},  SO::less,     false,  true,   true,   "[b,e)",  "[c,d)",  "{[b,e)}",        "{[b,c)}",        "{[b,c),[d,e)}",  },
        { __LINE__,  {"b","d","[)"},  {"d","e","[)"},  SO::less,     false,  false,  true,   "[b,e)",  "{}",     "{[b,e)}",        "{[b,d)}",        "{[b,e)}",        },
        { __LINE__,  {"b","d","(]"},  {},              SO::greater,  false,  false,  false,  "(b,d]",  "{}",     "{(b,d]}",        "{(b,d]}",        "{(b,d]}",        },
        { __LINE__,  {"b","d","(]"},  {" "," ","*"},   SO::greater,  false,  true,   true,   "*",      "(b,d]",  "{*}",            "{}",             "{<=b,>d}",       },
        { __LINE__,  {"b","d","(]"},  {"a"},           SO::greater,  false,  false,  false,  "[a,d]",  "{}",     "{a,(b,d]}",      "{(b,d]}",        "{a,(b,d]}",      },
        { __LINE__,  {"b","d","(]"},  {"b"},           SO::greater,  false,  false,  true,   "[b,d]",  "{}",     "{[b,d]}",        "{(b,d]}",        "{[b,d]}",        },
        { __LINE__,  {"b","d","(]"},  {"c"},           SO::less,     true,   true,   true,   "(b,d]",  "c",      "{(b,d]}",        "{(b,c),(c,d]}",  "{(b,c),(c,d]}",  },
        { __LINE__,  {"b","d","(]"},  {"d"},           SO::less,     true,   true,   true,   "(b,d]",  "d",      "{(b,d]}",        "{(b,d)}",        "{(b,d)}",        },
        { __LINE__,  {"b","d","(]"},  {"e"},           SO::less,     false,  false,  false,  "(b,e]",  "{}",     "{(b,d],e}",      "{(b,d]}",        "{(b,d],e}",      },
        { __LINE__,  {"b","d","(]"},  {"a","a","<"},   SO::greater,  false,  false,  false,  "<=d",    "{}",     "{<a,(b,d]}",     "{(b,d]}",        "{<a,(b,d]}",     },
        { __LINE__,  {"b","d","(]"},  {"b","b","<"},   SO::greater,  false,  false,  false,  "<=d",    "{}",     "{<b,(b,d]}",     "{(b,d]}",        "{<b,(b,d]}",     },
        { __LINE__,  {"b","d","(]"},  {"c","c","<"},   SO::greater,  false,  true,   true,   "<=d",    "(b,c)",  "{<=d}",          "{[c,d]}",        "{<=b,[c,d]}",    },
        { __LINE__,  {"b","d","(]"},  {"d","d","<"},   SO::greater,  false,  true,   true,   "<=d",    "(b,d)",  "{<=d}",          "{d}",            "{<=b,d}",        },
        { __LINE__,  {"b","d","(]"},  {"e","e","<"},   SO::greater,  false,  true,   true,   "<e",     "(b,d]",  "{<e}",           "{}",             "{<=b,(d,e)}",    },
        { __LINE__,  {"b","d","(]"},  {"a","a","<="},  SO::greater,  false,  false,  false,  "<=d",    "{}",     "{<=a,(b,d]}",    "{(b,d]}",        "{<=a,(b,d]}",    },
        { __LINE__,  {"b","d","(]"},  {"b","b","<="},  SO::greater,  false,  false,  true,   "<=d",    "{}",     "{<=d}",          "{(b,d]}",        "{<=d}",          },
        { __LINE__,  {"b","d","(]"},  {"c","c","<="},  SO::greater,  false,  true,   true,   "<=d",    "(b,c]",  "{<=d}",          "{(c,d]}",        "{<=b,(c,d]}",    },
        { __LINE__,  {"b","d","(]"},  {"d","d","<="},  SO::greater,  false,  true,   true,   "<=d",    "(b,d]",  "{<=d}",          "{}",             "{<=b}",          },
        { __LINE__,  {"b","d","(]"},  {"e","e","<="},  SO::greater,  false,  true,   true,   "<=e",    "(b,d]",  "{<=e}",          "{}",             "{<=b,(d,e]}",    },
        { __LINE__,  {"b","d","(]"},  {"a","a",">"},   SO::greater,  false,  true,   true,   ">a",     "(b,d]",  "{>a}",           "{}",             "{(a,b],>d}",     },
        { __LINE__,  {"b","d","(]"},  {"b","b",">"},   SO::less,     false,  true,   true,   ">b",     "(b,d]",  "{>b}",           "{}",             "{>d}",           },
        { __LINE__,  {"b","d","(]"},  {"c","c",">"},   SO::less,     false,  true,   true,   ">b",     "(c,d]",  "{>b}",           "{(b,c]}",        "{(b,c],>d}",     },
        { __LINE__,  {"b","d","(]"},  {"d","d",">"},   SO::less,     false,  false,  true,   ">b",     "{}",     "{>b}",           "{(b,d]}",        "{>b}",           },
        { __LINE__,  {"b","d","(]"},  {"e","e",">"},   SO::less,     false,  false,  false,  ">b",     "{}",     "{(b,d],>e}",     "{(b,d]}",        "{(b,d],>e}",     },
        { __LINE__,  {"b","d","(]"},  {"a","a",">="},  SO::greater,  false,  true,   true,   ">=a",    "(b,d]",  "{>=a}",          "{}",             "{[a,b],>d}",     },
        { __LINE__,  {"b","d","(]"},  {"b","b",">="},  SO::greater,  false,  true,   true,   ">=b",    "(b,d]",  "{>=b}",          "{}",             "{b,>d}",         },
        { __LINE__,  {"b","d","(]"},  {"c","c",">="},  SO::less,     false,  true,   true,   ">b",     "[c,d]",  "{>b}",           "{(b,c)}",        "{(b,c),>d}",     },
        { __LINE__,  {"b","d","(]"},  {"d","d",">="},  SO::less,     false,  true,   true,   ">b",     "d",      "{>b}",           "{(b,d)}",        "{(b,d),>d}",     },
        { __LINE__,  {"b","d","(]"},  {"e","e",">="},  SO::less,     false,  false,  false,  ">b",     "{}",     "{(b,d],>=e}",    "{(b,d]}",        "{(b,d],>=e}",    },
        { __LINE__,  {"b","d","(]"},  {"a","b","()"},  SO::greater,  false,  false,  false,  "(a,d]",  "{}",     "{(a,b),(b,d]}",  "{(b,d]}",        "{(a,b),(b,d]}",  },
        { __LINE__,  {"b","d","(]"},  {"a","c","()"},  SO::greater,  false,  true,   true,   "(a,d]",  "(b,c)",  "{(a,d]}",        "{[c,d]}",        "{(a,b],[c,d]}",  },
        { __LINE__,  {"b","d","(]"},  {"a","d","()"},  SO::greater,  false,  true,   true,   "(a,d]",  "(b,d)",  "{(a,d]}",        "{d}",            "{(a,b],d}",      },
        { __LINE__,  {"b","d","(]"},  {"a","e","()"},  SO::greater,  false,  true,   true,   "(a,e)",  "(b,d]",  "{(a,e)}",        "{}",             "{(a,b],(d,e)}",  },
        { __LINE__,  {"b","d","(]"},  {"b","c","()"},  SO::greater,  true,   true,   true,   "(b,d]",  "(b,c)",  "{(b,d]}",        "{[c,d]}",        "{[c,d]}",        },
        { __LINE__,  {"b","d","(]"},  {"b","d","()"},  SO::greater,  true,   true,   true,   "(b,d]",  "(b,d)",  "{(b,d]}",        "{d}",            "{d}",            },
        { __LINE__,  {"b","d","(]"},  {"b","e","()"},  SO::less,     false,  true,   true,   "(b,e)",  "(b,d]",  "{(b,e)}",        "{}",             "{(d,e)}",        },
        { __LINE__,  {"b","d","(]"},  {"c","d","()"},  SO::less,     true,   true,   true,   "(b,d]",  "(c,d)",  "{(b,d]}",        "{(b,c],d}",      "{(b,c],d}",      },
        { __LINE__,  {"b","d","(]"},  {"c","e","()"},  SO::less,     false,  true,   true,   "(b,e)",  "(c,d]",  "{(b,e)}",        "{(b,c]}",        "{(b,c],(d,e)}",  },
        { __LINE__,  {"b","d","(]"},  {"d","e","()"},  SO::less,     false,  false,  true,   "(b,e)",  "{}",     "{(b,e)}",        "{(b,d]}",        "{(b,e)}",        },
        { __LINE__,  {"b","d","(]"},  {"a","b","[)"},  SO::greater,  false,  false,  false,  "[a,d]",  "{}",     "{[a,b),(b,d]}",  "{(b,d]}",        "{[a,b),(b,d]}",  },
        { __LINE__,  {"b","d","(]"},  {"a","c","[)"},  SO::greater,  false,  true,   true,   "[a,d]",  "(b,c)",  "{[a,d]}",        "{[c,d]}",        "{[a,b],[c,d]}",  },
        { __LINE__,  {"b","d","(]"},  {"a","d","[)"},  SO::greater,  false,  true,   true,   "[a,d]",  "(b,d)",  "{[a,d]}",        "{d}",            "{[a,b],d}",      },
        { __LINE__,  {"b","d","(]"},  {"a","e","[)"},  SO::greater,  false,  true,   true,   "[a,e)",  "(b,d]",  "{[a,e)}",        "{}",             "{[a,b],(d,e)}",  },
        { __LINE__,  {"b","d","(]"},  {"b","c","[)"},  SO::greater,  false,  true,   true,   "[b,d]",  "(b,c)",  "{[b,d]}",        "{[c,d]}",        "{b,[c,d]}",      },
        { __LINE__,  {"b","d","(]"},  {"b","d","[)"},  SO::greater,  false,  true,   true,   "[b,d]",  "(b,d)",  "{[b,d]}",        "{d}",            "{b,d}",          },
        { __LINE__,  {"b","d","(]"},  {"b","e","[)"},  SO::greater,  false,  true,   true,   "[b,e)",  "(b,d]",  "{[b,e)}",        "{}",             "{b,(d,e)}",      },
        { __LINE__,  {"b","d","(]"},  {"c","d","[)"},  SO::less,     true,   true,   true,   "(b,d]",  "[c,d)",  "{(b,d]}",        "{(b,c),d}",      "{(b,c),d}",      },
        { __LINE__,  {"b","d","(]"},  {"c","e","[)"},  SO::less,     false,  true,   true,   "(b,e)",  "[c,d]",  "{(b,e)}",        "{(b,c)}",        "{(b,c),(d,e)}",  },
        { __LINE__,  {"b","d","(]"},  {"d","e","[)"},  SO::less,     false,  true,   true,   "(b,e)",  "d",      "{(b,e)}",        "{(b,d)}",        "{(b,d),(d,e)}",  },
        { __LINE__,  {"b","d","(]"},  {"a","b","(]"},  SO::greater,  false,  false,  true,   "(a,d]",  "{}",     "{(a,d]}",        "{(b,d]}",        "{(a,d]}",        },
        { __LINE__,  {"b","d","(]"},  {"a","c","(]"},  SO::greater,  false,  true,   true,   "(a,d]",  "(b,c]",  "{(a,d]}",        "{(c,d]}",        "{(a,b],(c,d]}",  },
        { __LINE__,  {"b","d","(]"},  {"a","d","(]"},  SO::greater,  false,  true,   true,   "(a,d]",  "(b,d]",  "{(a,d]}",        "{}",             "{(a,b]}",        },
        { __LINE__,  {"b","d","(]"},  {"a","e","(]"},  SO::greater,  false,  true,   true,   "(a,e]",  "(b,d]",  "{(a,e]}",        "{}",             "{(a,b],(d,e]}",  },
        { __LINE__,  {"b","d","(]"},  {"b","c","(]"},  SO::greater,  true,   true,   true,   "(b,d]",  "(b,c]",  "{(b,d]}",        "{(c,d]}",        "{(c,d]}",        },
        { __LINE__,  {"b","d","(]"},  {"b","d","(]"},  SO::equal,    true,   true,   true,   "(b,d]",  "(b,d]",  "{(b,d]}",        "{}",             "{}",             },
        { __LINE__,  {"b","d","(]"},  {"b","e","(]"},  SO::less,     false,  true,   true,   "(b,e]",  "(b,d]",  "{(b,e]}",        "{}",             "{(d,e]}",        },
        { __LINE__,  {"b","d","(]"},  {"c","d","(]"},  SO::less,     true,   true,   true,   "(b,d]",  "(c,d]",  "{(b,d]}",        "{(b,c]}",        "{(b,c]}",        },
        { __LINE__,  {"b","d","(]"},  {"c","e","(]"},  SO::less,     false,  true,   true,   "(b,e]",  "(c,d]",  "{(b,e]}",        "{(b,c]}",        "{(b,c],(d,e]}",  },
        { __LINE__,  {"b","d","(]"},  {"d","e","(]"},  SO::less,     false,  false,  true,   "(b,e]",  "{}",     "{(b,e]}",        "{(b,d]}",        "{(b,e]}",        },
        { __LINE__,  {"b","d","[]"},  {},              SO::greater,  false,  false,  false,  "[b,d]",  "{}",     "{[b,d]}",        "{[b,d]}",        "{[b,d]}",        },
        { __LINE__,  {"b","d","[]"},  {" "," ","*"},   SO::greater,  false,  true,   true,   "*",      "[b,d]",  "{*}",            "{}",             "{<b,>d}",        },
        { __LINE__,  {"b","d","[]"},  {"a"},           SO::greater,  false,  false,  false,  "[a,d]",  "{}",     "{a,[b,d]}",      "{[b,d]}",        "{a,[b,d]}",      },
        { __LINE__,  {"b","d","[]"},  {"b"},           SO::greater,  true,   true,   true,   "[b,d]",  "b",      "{[b,d]}",        "{(b,d]}",        "{(b,d]}",        },
        { __LINE__,  {"b","d","[]"},  {"c"},           SO::less,     true,   true,   true,   "[b,d]",  "c",      "{[b,d]}",        "{[b,c),(c,d]}",  "{[b,c),(c,d]}",  },
        { __LINE__,  {"b","d","[]"},  {"d"},           SO::less,     true,   true,   true,   "[b,d]",  "d",      "{[b,d]}",        "{[b,d)}",        "{[b,d)}",        },
        { __LINE__,  {"b","d","[]"},  {"e"},           SO::less,     false,  false,  false,  "[b,e]",  "{}",     "{[b,d],e}",      "{[b,d]}",        "{[b,d],e}",      },
        { __LINE__,  {"b","d","[]"},  {"a","a","<"},   SO::greater,  false,  false,  false,  "<=d",    "{}",     "{<a,[b,d]}",     "{[b,d]}",        "{<a,[b,d]}",     },
        { __LINE__,  {"b","d","[]"},  {"b","b","<"},   SO::greater,  false,  false,  true,   "<=d",    "{}",     "{<=d}",          "{[b,d]}",        "{<=d}",          },
        { __LINE__,  {"b","d","[]"},  {"c","c","<"},   SO::greater,  false,  true,   true,   "<=d",    "[b,c)",  "{<=d}",          "{[c,d]}",        "{<b,[c,d]}",     },
        { __LINE__,  {"b","d","[]"},  {"d","d","<"},   SO::greater,  false,  true,   true,   "<=d",    "[b,d)",  "{<=d}",          "{d}",            "{<b,d}",         },
        { __LINE__,  {"b","d","[]"},  {"e","e","<"},   SO::greater,  false,  true,   true,   "<e",     "[b,d]",  "{<e}",           "{}",             "{<b,(d,e)}",     },
        { __LINE__,  {"b","d","[]"},  {"a","a","<="},  SO::greater,  false,  false,  false,  "<=d",    "{}",     "{<=a,[b,d]}",    "{[b,d]}",        "{<=a,[b,d]}",    },
        { __LINE__,  {"b","d","[]"},  {"b","b","<="},  SO::greater,  false,  true,   true,   "<=d",    "b",      "{<=d}",          "{(b,d]}",        "{<b,(b,d]}",     },
        { __LINE__,  {"b","d","[]"},  {"c","c","<="},  SO::greater,  false,  true,   true,   "<=d",    "[b,c]",  "{<=d}",          "{(c,d]}",        "{<b,(c,d]}",     },
        { __LINE__,  {"b","d","[]"},  {"d","d","<="},  SO::greater,  false,  true,   true,   "<=d",    "[b,d]",  "{<=d}",          "{}",             "{<b}",           },
        { __LINE__,  {"b","d","[]"},  {"e","e","<="},  SO::greater,  false,  true,   true,   "<=e",    "[b,d]",  "{<=e}",          "{}",             "{<b,(d,e]}",     },
        { __LINE__,  {"b","d","[]"},  {"a","a",">"},   SO::greater,  false,  true,   true,   ">a",     "[b,d]",  "{>a}",           "{}",             "{(a,b),>d}",     },
        { __LINE__,  {"b","d","[]"},  {"b","b",">"},   SO::less,     false,  true,   true,   ">=b",    "(b,d]",  "{>=b}",          "{b}",            "{b,>d}",         },
        { __LINE__,  {"b","d","[]"},  {"c","c",">"},   SO::less,     false,  true,   true,   ">=b",    "(c,d]",  "{>=b}",          "{[b,c]}",        "{[b,c],>d}",     },
        { __LINE__,  {"b","d","[]"},  {"d","d",">"},   SO::less,     false,  false,  true,   ">=b",    "{}",     "{>=b}",          "{[b,d]}",        "{>=b}",          },
        { __LINE__,  {"b","d","[]"},  {"e","e",">"},   SO::less,     false,  false,  false,  ">=b",    "{}",     "{[b,d],>e}",     "{[b,d]}",        "{[b,d],>e}",     },
        { __LINE__,  {"b","d","[]"},  {"a","a",">="},  SO::greater,  false,  true,   true,   ">=a",    "[b,d]",  "{>=a}",          "{}",             "{[a,b),>d}",     },
        { __LINE__,  {"b","d","[]"},  {"b","b",">="},  SO::less,     false,  true,   true,   ">=b",    "[b,d]",  "{>=b}",          "{}",             "{>d}",           },
        { __LINE__,  {"b","d","[]"},  {"c","c",">="},  SO::less,     false,  true,   true,   ">=b",    "[c,d]",  "{>=b}",          "{[b,c)}",        "{[b,c),>d}",     },
        { __LINE__,  {"b","d","[]"},  {"d","d",">="},  SO::less,     false,  true,   true,   ">=b",    "d",      "{>=b}",          "{[b,d)}",        "{[b,d),>d}",     },
        { __LINE__,  {"b","d","[]"},  {"e","e",">="},  SO::less,     false,  false,  false,  ">=b",    "{}",     "{[b,d],>=e}",    "{[b,d]}",        "{[b,d],>=e}",    },
        { __LINE__,  {"b","d","[]"},  {"a","b","()"},  SO::greater,  false,  false,  true,   "(a,d]",  "{}",     "{(a,d]}",        "{[b,d]}",        "{(a,d]}",        },
        { __LINE__,  {"b","d","[]"},  {"a","c","()"},  SO::greater,  false,  true,   true,   "(a,d]",  "[b,c)",  "{(a,d]}",        "{[c,d]}",        "{(a,b),[c,d]}",  },
        { __LINE__,  {"b","d","[]"},  {"a","d","()"},  SO::greater,  false,  true,   true,   "(a,d]",  "[b,d)",  "{(a,d]}",        "{d}",            "{(a,b),d}",      },
        { __LINE__,  {"b","d","[]"},  {"a","e","()"},  SO::greater,  false,  true,   true,   "(a,e)",  "[b,d]",  "{(a,e)}",        "{}",             "{(a,b),(d,e)}",  },
        { __LINE__,  {"b","d","[]"},  {"b","c","()"},  SO::less,     true,   true,   true,   "[b,d]",  "(b,c)",  "{[b,d]}",        "{b,[c,d]}",      "{b,[c,d]}",      },
        { __LINE__,  {"b","d","[]"},  {"b","d","()"},  SO::less,     true,   true,   true,   "[b,d]",  "(b,d)",  "{[b,d]}",        "{b,d}",          "{b,d}",          },
        { __LINE__,  {"b","d","[]"},  {"b","e","()"},  SO::less,     false,  true,   true,   "[b,e)",  "(b,d]",  "{[b,e)}",        "{b}",            "{b,(d,e)}",      },
        { __LINE__,  {"b","d","[]"},  {"c","d","()"},  SO::less,     true,   true,   true,   "[b,d]",  "(c,d)",  "{[b,d]}",        "{[b,c],d}",      "{[b,c],d}",      },
        { __LINE__,  {"b","d","[]"},  {"c","e","()"},  SO::less,     false,  true,   true,   "[b,e)",  "(c,d]",  "{[b,e)}",        "{[b,c]}",        "{[b,c],(d,e)}",  },
        { __LINE__,  {"b","d","[]"},  {"d","e","()"},  SO::less,     false,  false,  true,   "[b,e)",  "{}",     "{[b,e)}",        "{[b,d]}",        "{[b,e)}",        },
        { __LINE__,  {"b","d","[]"},  {"a","b","[)"},  SO::greater,  false,  false,  true,   "[a,d]",  "{}",     "{[a,d]}",        "{[b,d]}",        "{[a,d]}",        },
        { __LINE__,  {"b","d","[]"},  {"a","c","[)"},  SO::greater,  false,  true,   true,   "[a,d]",  "[b,c)",  "{[a,d]}",        "{[c,d]}",        "{[a,b),[c,d]}",  },
        { __LINE__,  {"b","d","[]"},  {"a","d","[)"},  SO::greater,  false,  true,   true,   "[a,d]",  "[b,d)",  "{[a,d]}",        "{d}",            "{[a,b),d}",      },
        { __LINE__,  {"b","d","[]"},  {"a","e","[)"},  SO::greater,  false,  true,   true,   "[a,e)",  "[b,d]",  "{[a,e)}",        "{}",             "{[a,b),(d,e)}",  },
        { __LINE__,  {"b","d","[]"},  {"b","c","[)"},  SO::greater,  true,   true,   true,   "[b,d]",  "[b,c)",  "{[b,d]}",        "{[c,d]}",        "{[c,d]}",        },
        { __LINE__,  {"b","d","[]"},  {"b","d","[)"},  SO::greater,  true,   true,   true,   "[b,d]",  "[b,d)",  "{[b,d]}",        "{d}",            "{d}",            },
        { __LINE__,  {"b","d","[]"},  {"b","e","[)"},  SO::less,     false,  true,   true,   "[b,e)",  "[b,d]",  "{[b,e)}",        "{}",             "{(d,e)}",        },
        { __LINE__,  {"b","d","[]"},  {"c","d","[)"},  SO::less,     true,   true,   true,   "[b,d]",  "[c,d)",  "{[b,d]}",        "{[b,c),d}",      "{[b,c),d}",      },
        { __LINE__,  {"b","d","[]"},  {"c","e","[)"},  SO::less,     false,  true,   true,   "[b,e)",  "[c,d]",  "{[b,e)}",        "{[b,c)}",        "{[b,c),(d,e)}",  },
        { __LINE__,  {"b","d","[]"},  {"d","e","[)"},  SO::less,     false,  true,   true,   "[b,e)",  "d",      "{[b,e)}",        "{[b,d)}",        "{[b,d),(d,e)}",  },
        { __LINE__,  {"b","d","[]"},  {"a","b","(]"},  SO::greater,  false,  true,   true,   "(a,d]",  "b",      "{(a,d]}",        "{(b,d]}",        "{(a,b),(b,d]}",  },
        { __LINE__,  {"b","d","[]"},  {"a","c","(]"},  SO::greater,  false,  true,   true,   "(a,d]",  "[b,c]",  "{(a,d]}",        "{(c,d]}",        "{(a,b),(c,d]}",  },
        { __LINE__,  {"b","d","[]"},  {"a","d","(]"},  SO::greater,  false,  true,   true,   "(a,d]",  "[b,d]",  "{(a,d]}",        "{}",             "{(a,b)}",        },
        { __LINE__,  {"b","d","[]"},  {"a","e","(]"},  SO::greater,  false,  true,   true,   "(a,e]",  "[b,d]",  "{(a,e]}",        "{}",             "{(a,b),(d,e]}",  },
        { __LINE__,  {"b","d","[]"},  {"b","c","(]"},  SO::less,     true,   true,   true,   "[b,d]",  "(b,c]",  "{[b,d]}",        "{b,(c,d]}",      "{b,(c,d]}",      },
        { __LINE__,  {"b","d","[]"},  {"b","d","(]"},  SO::less,     true,   true,   true,   "[b,d]",  "(b,d]",  "{[b,d]}",        "{b}",            "{b}",            },
        { __LINE__,  {"b","d","[]"},  {"b","e","(]"},  SO::less,     false,  true,   true,   "[b,e]",  "(b,d]",  "{[b,e]}",        "{b}",            "{b,(d,e]}",      },
        { __LINE__,  {"b","d","[]"},  {"c","d","(]"},  SO::less,     true,   true,   true,   "[b,d]",  "(c,d]",  "{[b,d]}",        "{[b,c]}",        "{[b,c]}",        },
        { __LINE__,  {"b","d","[]"},  {"c","e","(]"},  SO::less,     false,  true,   true,   "[b,e]",  "(c,d]",  "{[b,e]}",        "{[b,c]}",        "{[b,c],(d,e]}",  },
        { __LINE__,  {"b","d","[]"},  {"d","e","(]"},  SO::less,     false,  false,  true,   "[b,e]",  "{}",     "{[b,e]}",        "{[b,d]}",        "{[b,e]}",        },
        { __LINE__,  {"b","d","[]"},  {"a","a","[]"},  SO::greater,  false,  false,  false,  "[a,d]",  "{}",     "{a,[b,d]}",      "{[b,d]}",        "{a,[b,d]}",      },
        { __LINE__,  {"b","d","[]"},  {"a","b","[]"},  SO::greater,  false,  true,   true,   "[a,d]",  "b",      "{[a,d]}",        "{(b,d]}",        "{[a,b),(b,d]}",  },
        { __LINE__,  {"b","d","[]"},  {"a","c","[]"},  SO::greater,  false,  true,   true,   "[a,d]",  "[b,c]",  "{[a,d]}",        "{(c,d]}",        "{[a,b),(c,d]}",  },
        { __LINE__,  {"b","d","[]"},  {"a","d","[]"},  SO::greater,  false,  true,   true,   "[a,d]",  "[b,d]",  "{[a,d]}",        "{}",             "{[a,b)}",        },
        { __LINE__,  {"b","d","[]"},  {"a","e","[]"},  SO::greater,  false,  true,   true,   "[a,e]",  "[b,d]",  "{[a,e]}",        "{}",             "{[a,b),(d,e]}",  },
        { __LINE__,  {"b","d","[]"},  {"b","c","[]"},  SO::greater,  true,   true,   true,   "[b,d]",  "[b,c]",  "{[b,d]}",        "{(c,d]}",        "{(c,d]}",        },
        { __LINE__,  {"b","d","[]"},  {"b","d","[]"},  SO::equal,    true,   true,   true,   "[b,d]",  "[b,d]",  "{[b,d]}",        "{}",             "{}",             },
        { __LINE__,  {"b","d","[]"},  {"b","e","[]"},  SO::less,     false,  true,   true,   "[b,e]",  "[b,d]",  "{[b,e]}",        "{}",             "{(d,e]}",        },
        { __LINE__,  {"b","d","[]"},  {"c","d","[]"},  SO::less,     true,   true,   true,   "[b,d]",  "[c,d]",  "{[b,d]}",        "{[b,c)}",        "{[b,c)}",        },
        { __LINE__,  {"b","d","[]"},  {"c","e","[]"},  SO::less,     false,  true,   true,   "[b,e]",  "[c,d]",  "{[b,e]}",        "{[b,c)}",        "{[b,c),(d,e]}",  },
        { __LINE__,  {"b","d","[]"},  {"d","e","[]"},  SO::less,     false,  true,   true,   "[b,e]",  "d",      "{[b,e]}",        "{[b,d)}",        "{[b,d),(d,e]}",  },

    };

    SO cmp = SO::equal;
    bool flag = false;
    std::string str;

    for (auto& t: tests) {

        int errors = 0;

        TRY(cmp = t.lhs <=> t.rhs);                              TEST(cmp == t.compare);                       errors += int(cmp != t.compare);
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

void test_rs_interval_ordered_interval_hashing() {

    std::unordered_set<Itv> set;

    TRY(set.insert(Itv("a")));
    TRY(set.insert(Itv("b", "c")));
    TRY(set.insert(Itv("d", "d", ">")));
    TEST_EQUAL(set.size(), 3u);

}
