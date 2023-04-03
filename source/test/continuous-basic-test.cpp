#include "rs-intervals/interval.hpp"
#include "rs-intervals/set.hpp"
#include "rs-intervals/types.hpp"
#include "test/unit-test.hpp"
#include <compare>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace RS::Intervals;

using Itv = Interval<double>;
using Set = IntervalSet<double>;
using SO = std::strong_ordering;

void test_rs_interval_continuous_interval_basic_properties() {

    Itv in;

    TEST_TYPE(Itv::value_type, double);
    TEST_EQUAL(Itv::category, Category::continuous);
    TEST(in.empty());
    TEST_EQUAL(in.size(), 0u);
    TEST(! in);
    TEST_EQUAL(in.str(), "{}");
    TEST_EQUAL(in.match(0), Match::empty);
    TEST_EQUAL(in.match(123.456), Match::empty);
    TEST(! in(0));
    TEST(! in(123.456));

}

void test_rs_interval_continuous_interval_construction() {

    static constexpr auto inf = std::numeric_limits<double>::infinity();

    Itv in;
    std::string str;

    TRY(in = Itv());                                      TRY(str = in.str());  TEST_EQUAL(str, "{}");     TEST_EQUAL(in.size(), 0);
    TRY(in = Itv(1.0));                                   TRY(str = in.str());  TEST_EQUAL(str, "1");      TEST_EQUAL(in.size(), 0);
    TRY(in = Itv(1.0, Bound::closed, Bound::closed));     TRY(str = in.str());  TEST_EQUAL(str, "1");      TEST_EQUAL(in.size(), 0);
    TRY(in = Itv(1.0, Bound::closed, Bound::open));       TRY(str = in.str());  TEST_EQUAL(str, "{}");     TEST_EQUAL(in.size(), 0);
    TRY(in = Itv(1.0, Bound::open, Bound::closed));       TRY(str = in.str());  TEST_EQUAL(str, "{}");     TEST_EQUAL(in.size(), 0);
    TRY(in = Itv(1.0, Bound::closed, Bound::unbound));    TRY(str = in.str());  TEST_EQUAL(str, ">=1");    TEST_EQUAL(in.size(), inf);
    TRY(in = Itv(1.0, Bound::unbound, Bound::closed));    TRY(str = in.str());  TEST_EQUAL(str, "<=1");    TEST_EQUAL(in.size(), inf);
    TRY(in = Itv(1.0, Bound::open, Bound::unbound));      TRY(str = in.str());  TEST_EQUAL(str, ">1");     TEST_EQUAL(in.size(), inf);
    TRY(in = Itv(1.0, Bound::unbound, Bound::open));      TRY(str = in.str());  TEST_EQUAL(str, "<1");     TEST_EQUAL(in.size(), inf);
    TRY(in = Itv(1.0, Bound::unbound, Bound::unbound));   TRY(str = in.str());  TEST_EQUAL(str, "*");      TEST_EQUAL(in.size(), inf);
    TRY(in = Itv(1.0, 2.0));                              TRY(str = in.str());  TEST_EQUAL(str, "[1,2]");  TEST_EQUAL(in.size(), 1);
    TRY(in = Itv(1.0, 2.0, Bound::closed));               TRY(str = in.str());  TEST_EQUAL(str, "[1,2]");  TEST_EQUAL(in.size(), 1);
    TRY(in = Itv(1.0, 2.0, Bound::open));                 TRY(str = in.str());  TEST_EQUAL(str, "(1,2)");  TEST_EQUAL(in.size(), 1);
    TRY(in = Itv(1.0, 2.0, Bound::closed, Bound::open));  TRY(str = in.str());  TEST_EQUAL(str, "[1,2)");  TEST_EQUAL(in.size(), 1);
    TRY(in = Itv(1.0, 2.0, Bound::open, Bound::closed));  TRY(str = in.str());  TEST_EQUAL(str, "(1,2]");  TEST_EQUAL(in.size(), 1);
    TRY(in = Itv(1.0, 2.0, "[]"));                        TRY(str = in.str());  TEST_EQUAL(str, "[1,2]");  TEST_EQUAL(in.size(), 1);
    TRY(in = Itv(1.0, 2.0, "()"));                        TRY(str = in.str());  TEST_EQUAL(str, "(1,2)");  TEST_EQUAL(in.size(), 1);
    TRY(in = Itv(1.0, 2.0, "[)"));                        TRY(str = in.str());  TEST_EQUAL(str, "[1,2)");  TEST_EQUAL(in.size(), 1);
    TRY(in = Itv(1.0, 2.0, "(]"));                        TRY(str = in.str());  TEST_EQUAL(str, "(1,2]");  TEST_EQUAL(in.size(), 1);
    TRY(in = Itv(1.0, 2.0, "<"));                         TRY(str = in.str());  TEST_EQUAL(str, "<2");     TEST_EQUAL(in.size(), inf);
    TRY(in = Itv(1.0, 2.0, "<="));                        TRY(str = in.str());  TEST_EQUAL(str, "<=2");    TEST_EQUAL(in.size(), inf);
    TRY(in = Itv(1.0, 2.0, ">"));                         TRY(str = in.str());  TEST_EQUAL(str, ">1");     TEST_EQUAL(in.size(), inf);
    TRY(in = Itv(1.0, 2.0, ">="));                        TRY(str = in.str());  TEST_EQUAL(str, ">=1");    TEST_EQUAL(in.size(), inf);
    TRY(in = Itv(1.0, 2.0, "*"));                         TRY(str = in.str());  TEST_EQUAL(str, "*");      TEST_EQUAL(in.size(), inf);
    TRY(in = Itv(2.0, 1.0));                              TRY(str = in.str());  TEST_EQUAL(str, "{}");     TEST_EQUAL(in.size(), 0);

    TRY(in = make_interval(1.0));                                   TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = make_interval(1.0, Bound::closed, Bound::closed));     TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = make_interval(1.0, Bound::closed, Bound::open));       TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = make_interval(1.0, Bound::open, Bound::closed));       TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = make_interval(1.0, Bound::closed, Bound::unbound));    TRY(str = in.str());  TEST_EQUAL(str, ">=1");
    TRY(in = make_interval(1.0, Bound::unbound, Bound::closed));    TRY(str = in.str());  TEST_EQUAL(str, "<=1");
    TRY(in = make_interval(1.0, Bound::open, Bound::unbound));      TRY(str = in.str());  TEST_EQUAL(str, ">1");
    TRY(in = make_interval(1.0, Bound::unbound, Bound::open));      TRY(str = in.str());  TEST_EQUAL(str, "<1");
    TRY(in = make_interval(1.0, Bound::unbound, Bound::unbound));   TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = make_interval(1.0, 2.0));                              TRY(str = in.str());  TEST_EQUAL(str, "[1,2]");
    TRY(in = make_interval(1.0, 2.0, Bound::closed));               TRY(str = in.str());  TEST_EQUAL(str, "[1,2]");
    TRY(in = make_interval(1.0, 2.0, Bound::open));                 TRY(str = in.str());  TEST_EQUAL(str, "(1,2)");
    TRY(in = make_interval(1.0, 2.0, Bound::closed, Bound::open));  TRY(str = in.str());  TEST_EQUAL(str, "[1,2)");
    TRY(in = make_interval(1.0, 2.0, Bound::open, Bound::closed));  TRY(str = in.str());  TEST_EQUAL(str, "(1,2]");
    TRY(in = make_interval(1.0, 2.0, "[]"));                        TRY(str = in.str());  TEST_EQUAL(str, "[1,2]");
    TRY(in = make_interval(1.0, 2.0, "()"));                        TRY(str = in.str());  TEST_EQUAL(str, "(1,2)");
    TRY(in = make_interval(1.0, 2.0, "[)"));                        TRY(str = in.str());  TEST_EQUAL(str, "[1,2)");
    TRY(in = make_interval(1.0, 2.0, "(]"));                        TRY(str = in.str());  TEST_EQUAL(str, "(1,2]");
    TRY(in = make_interval(1.0, 2.0, "<"));                         TRY(str = in.str());  TEST_EQUAL(str, "<2");
    TRY(in = make_interval(1.0, 2.0, "<="));                        TRY(str = in.str());  TEST_EQUAL(str, "<=2");
    TRY(in = make_interval(1.0, 2.0, ">"));                         TRY(str = in.str());  TEST_EQUAL(str, ">1");
    TRY(in = make_interval(1.0, 2.0, ">="));                        TRY(str = in.str());  TEST_EQUAL(str, ">=1");
    TRY(in = make_interval(1.0, 2.0, "*"));                         TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = make_interval(2.0, 1.0));                              TRY(str = in.str());  TEST_EQUAL(str, "{}");

    TRY(in = ordered_interval(2.0, 1.0));                              TRY(str = in.str());  TEST_EQUAL(str, "[1,2]");
    TRY(in = ordered_interval(2.0, 1.0, Bound::closed));               TRY(str = in.str());  TEST_EQUAL(str, "[1,2]");
    TRY(in = ordered_interval(2.0, 1.0, Bound::open));                 TRY(str = in.str());  TEST_EQUAL(str, "(1,2)");
    TRY(in = ordered_interval(2.0, 1.0, Bound::closed, Bound::open));  TRY(str = in.str());  TEST_EQUAL(str, "(1,2]");
    TRY(in = ordered_interval(2.0, 1.0, Bound::open, Bound::closed));  TRY(str = in.str());  TEST_EQUAL(str, "[1,2)");

}

void test_rs_interval_continuous_interval_string_conversions() {

    Itv in;

    auto fmt = [] (double x) {
        std::ostringstream out;
        out << "'" << x << "'";
        return out.str();
    };

    TRY(in = Itv::from_string(""));         TEST_EQUAL(in.str(), "{}");      TEST_EQUAL(in.str(fmt), "{}");
    TRY(in = Itv::from_string("{}"));       TEST_EQUAL(in.str(), "{}");      TEST_EQUAL(in.str(fmt), "{}");
    TRY(in = Itv::from_string("*"));        TEST_EQUAL(in.str(), "*");       TEST_EQUAL(in.str(fmt), "*");
    TRY(in = Itv::from_string("5"));        TEST_EQUAL(in.str(), "5");       TEST_EQUAL(in.str(fmt), "'5'");
    TRY(in = Itv::from_string(">5"));       TEST_EQUAL(in.str(), ">5");      TEST_EQUAL(in.str(fmt), ">'5'");
    TRY(in = Itv::from_string("<5"));       TEST_EQUAL(in.str(), "<5");      TEST_EQUAL(in.str(fmt), "<'5'");
    TRY(in = Itv::from_string(">=5"));      TEST_EQUAL(in.str(), ">=5");     TEST_EQUAL(in.str(fmt), ">='5'");
    TRY(in = Itv::from_string("<=5"));      TEST_EQUAL(in.str(), "<=5");     TEST_EQUAL(in.str(fmt), "<='5'");
    TRY(in = Itv::from_string("5+"));       TEST_EQUAL(in.str(), ">=5");     TEST_EQUAL(in.str(fmt), ">='5'");
    TRY(in = Itv::from_string("5-"));       TEST_EQUAL(in.str(), "<=5");     TEST_EQUAL(in.str(fmt), "<='5'");
    TRY(in = Itv::from_string("[5,10]"));   TEST_EQUAL(in.str(), "[5,10]");  TEST_EQUAL(in.str(fmt), "['5','10']");
    TRY(in = Itv::from_string("[5,10)"));   TEST_EQUAL(in.str(), "[5,10)");  TEST_EQUAL(in.str(fmt), "['5','10')");
    TRY(in = Itv::from_string("(5,10]"));   TEST_EQUAL(in.str(), "(5,10]");  TEST_EQUAL(in.str(fmt), "('5','10']");
    TRY(in = Itv::from_string("(5,10)"));   TEST_EQUAL(in.str(), "(5,10)");  TEST_EQUAL(in.str(fmt), "('5','10')");
    TRY(in = Itv::from_string("5-10"));     TEST_EQUAL(in.str(), "[5,10]");  TEST_EQUAL(in.str(fmt), "['5','10']");
    TRY(in = Itv::from_string("5..10"));    TEST_EQUAL(in.str(), "[5,10]");  TEST_EQUAL(in.str(fmt), "['5','10']");
    TRY(in = Itv::from_string("5...10"));   TEST_EQUAL(in.str(), "[5,10]");  TEST_EQUAL(in.str(fmt), "['5','10']");
    TRY(in = Itv::from_string("5..<10"));   TEST_EQUAL(in.str(), "[5,10)");  TEST_EQUAL(in.str(fmt), "['5','10')");
    TRY(in = Itv::from_string("5<..10"));   TEST_EQUAL(in.str(), "(5,10]");  TEST_EQUAL(in.str(fmt), "('5','10']");
    TRY(in = Itv::from_string("5<..<10"));  TEST_EQUAL(in.str(), "(5,10)");  TEST_EQUAL(in.str(fmt), "('5','10')");

}

void test_rs_interval_continuous_interval_order() {

    Itv a, b;
    Order io = {};

    TRY(a = Itv());            TRY(b = Itv());            TRY(io = a.order(b));  TEST_EQUAL(io, Order::equal);              // ...
    TRY(a = Itv());            TRY(b = Itv(10,10,">"));   TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_only);             // bbb
    TRY(a = Itv());            TRY(b = Itv(10,10,">="));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_only);             // bbb
    TRY(a = Itv());            TRY(b = Itv(10,10,"<"));   TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_only);             // bbb
    TRY(a = Itv());            TRY(b = Itv(10,10,"<="));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_only);             // bbb
    TRY(a = Itv());            TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_only);             // bbb
    TRY(a = Itv());            TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_only);             // bbb
    TRY(a = Itv());            TRY(b = Itv(10));          TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_only);             // b
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(25,30,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_below_b);          // aaa...bbb
    TRY(a = Itv(10,10,"<"));   TRY(b = Itv(10,10,">"));   TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_below_b);          // aaa.bbb
    TRY(a = Itv(10,10,"<"));   TRY(b = Itv(10,10,">="));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_touches_b);        // aaabbb
    TRY(a = Itv(10,10,"<="));  TRY(b = Itv(10,10,">"));   TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_touches_b);        // aaabbb
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(20,25,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_touches_b);        // aaabbb
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv(20,25,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_touches_b);        // aaabbb
    TRY(a = Itv(10,10,"<="));  TRY(b = Itv(10,10,">="));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_overlaps_b);       // aaa*bbb
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(20,25,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_overlaps_b);       // aaa*bbb
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(15,25,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_overlaps_b);       // aaa***bbb
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv(15,25,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_overlaps_b);       // aaa***bbb
    TRY(a = Itv(10,20,"[)"));  TRY(b = Itv(15,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_overlaps_b);       // aaa***b
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(10,25,"(]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_overlaps_b);       // a***bbb
    TRY(a = Itv(10,20,"[)"));  TRY(b = Itv(10,20,"(]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_overlaps_b);       // a***b
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(15,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_extends_below_b);  // aaa***
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv(15,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_extends_below_b);  // aaa***
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(10,20,"(]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_extends_below_b);  // a***
    TRY(a = Itv(10,20,"[)"));  TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_extends_below_b);  // a***
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(12,18,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_encloses_b);       // aaa***aaa
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv(12,18,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_encloses_b);       // aaa***aaa
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(15,20,"[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_encloses_b);       // aaa***a
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(10,15,"(]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_encloses_b);       // a***aaa
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_encloses_b);       // a***a
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(10,25,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_extends_above_a);  // ***bbb
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv(10,25,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_extends_above_a);  // ***bbb
    TRY(a = Itv(10,20,"[)"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_extends_above_a);  // ***b
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::equal);              // ***
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::equal);              // ***
    TRY(a = Itv(10,20,"[)"));  TRY(b = Itv(10,20,"[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::equal);              // ***
    TRY(a = Itv(10,20,"[)"));  TRY(b = Itv(10,20,"[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::equal);              // ***
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(10,20,"[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_extends_above_b);  // ***a
    TRY(a = Itv(10,25,"()"));  TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_extends_above_b);  // ***aaa
    TRY(a = Itv(10,25,"[]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_extends_above_b);  // ***aaa
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_encloses_a);       // b***b
    TRY(a = Itv(10,15,"(]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_encloses_a);       // b***bbb
    TRY(a = Itv(15,20,"[)"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_encloses_a);       // bbb***b
    TRY(a = Itv(12,18,"[]"));  TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_encloses_a);       // bbb***bbb
    TRY(a = Itv(12,18,"[]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_encloses_a);       // bbb***bbb
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv(10,20,"[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_extends_below_a);  // b***
    TRY(a = Itv(10,20,"(]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_extends_below_a);  // b***
    TRY(a = Itv(15,20,"()"));  TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_extends_below_a);  // bbb***
    TRY(a = Itv(15,20,"[]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_extends_below_a);  // bbb***
    TRY(a = Itv(10,20,"(]"));  TRY(b = Itv(10,20,"[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_overlaps_a);       // b***a
    TRY(a = Itv(10,25,"(]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_overlaps_a);       // b***aaa
    TRY(a = Itv(15,20,"[]"));  TRY(b = Itv(10,20,"[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_overlaps_a);       // bbb***a
    TRY(a = Itv(15,25,"()"));  TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_overlaps_a);       // bbb***aaa
    TRY(a = Itv(15,25,"[]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_overlaps_a);       // bbb***aaa
    TRY(a = Itv(20,25,"[]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_overlaps_a);       // bbb*aaa
    TRY(a = Itv(10,10,">="));  TRY(b = Itv(10,10,"<="));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_overlaps_a);       // bbb*aaa
    TRY(a = Itv(20,25,"[]"));  TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_touches_a);        // bbbaaa
    TRY(a = Itv(20,25,"()"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_touches_a);        // bbbaaa
    TRY(a = Itv(10,10,">"));   TRY(b = Itv(10,10,"<="));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_touches_a);        // bbbaaa
    TRY(a = Itv(10,10,">="));  TRY(b = Itv(10,10,"<"));   TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_touches_a);        // bbbaaa
    TRY(a = Itv(10,10,">"));   TRY(b = Itv(10,10,"<"));   TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_below_a);          // bbb.aaa
    TRY(a = Itv(25,30,"[]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, Order::b_below_a);          // bbb...aaa
    TRY(a = Itv(10));          TRY(b = Itv());            TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_only);             // a
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv());            TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_only);             // aaa
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv());            TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_only);             // aaa
    TRY(a = Itv(10,10,"<="));  TRY(b = Itv());            TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_only);             // aaaa
    TRY(a = Itv(10,10,"<"));   TRY(b = Itv());            TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_only);             // aaaa
    TRY(a = Itv(10,10,">="));  TRY(b = Itv());            TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_only);             // aaaa
    TRY(a = Itv(10,10,">"));   TRY(b = Itv());            TRY(io = a.order(b));  TEST_EQUAL(io, Order::a_only);             // aaaa

}

void test_rs_interval_continuous_interval_complement() {

    Itv in;
    Set set;
    std::string str;

    TRY((in = {}));            TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{*}");
    TRY((in = Itv::all()));    TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{}");
    TRY((in = 42));            TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<42,>42}");
    TRY((in = {42,42,"<"}));   TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{>=42}");
    TRY((in = {42,42,"<="}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{>42}");
    TRY((in = {42,42,">"}));   TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<=42}");
    TRY((in = {42,42,">="}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<42}");
    TRY((in = {86,99,"()"}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<=86,>=99}");
    TRY((in = {86,99,"(]"}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<=86,>99}");
    TRY((in = {86,99,"[)"}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<86,>=99}");
    TRY((in = {86,99,"[]"}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<86,>99}");

}

void test_rs_interval_continuous_interval_binary_operations() {

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

        // line      lhs          rhs          cmp           incl    over    touch   envel     inter     union             diff              symm              --
        { __LINE__,  {},          {},          SO::equal,    false,  false,  false,  "{}",     "{}",     "{}",             "{}",             "{}",             },
        { __LINE__,  {0,0,"*"},   {},          SO::greater,  false,  false,  false,  "*",      "{}",     "{*}",            "{*}",            "{*}",            },
        { __LINE__,  {0,0,"*"},   {0,0,"*"},   SO::equal,    true,   true,   true,   "*",      "*",      "{*}",            "{}",             "{}",             },
        { __LINE__,  {2},         {},          SO::greater,  false,  false,  false,  "2",      "{}",     "{2}",            "{2}",            "{2}",            },
        { __LINE__,  {2},         {0,0,"*"},   SO::greater,  false,  true,   true,   "*",      "2",      "{*}",            "{}",             "{<2,>2}",        },
        { __LINE__,  {2},         {1},         SO::greater,  false,  false,  false,  "[1,2]",  "{}",     "{1,2}",          "{2}",            "{1,2}",          },
        { __LINE__,  {2},         {2},         SO::equal,    true,   true,   true,   "2",      "2",      "{2}",            "{}",             "{}",             },
        { __LINE__,  {2},         {3},         SO::less,     false,  false,  false,  "[2,3]",  "{}",     "{2,3}",          "{2}",            "{2,3}",          },
        { __LINE__,  {2,2,"<"},   {},          SO::greater,  false,  false,  false,  "<2",     "{}",     "{<2}",           "{<2}",           "{<2}",           },
        { __LINE__,  {2,2,"<"},   {0,0,"*"},   SO::less,     false,  true,   true,   "*",      "<2",     "{*}",            "{}",             "{>=2}",          },
        { __LINE__,  {2,2,"<"},   {1},         SO::less,     true,   true,   true,   "<2",     "1",      "{<2}",           "{<1,(1,2)}",     "{<1,(1,2)}",     },
        { __LINE__,  {2,2,"<"},   {2},         SO::less,     false,  false,  true,   "<=2",    "{}",     "{<=2}",          "{<2}",           "{<=2}",          },
        { __LINE__,  {2,2,"<"},   {3},         SO::less,     false,  false,  false,  "<=3",    "{}",     "{<2,3}",         "{<2}",           "{<2,3}",         },
        { __LINE__,  {2,2,"<"},   {1,1,"<"},   SO::greater,  true,   true,   true,   "<2",     "<1",     "{<2}",           "{[1,2)}",        "{[1,2)}",        },
        { __LINE__,  {2,2,"<"},   {2,2,"<"},   SO::equal,    true,   true,   true,   "<2",     "<2",     "{<2}",           "{}",             "{}",             },
        { __LINE__,  {2,2,"<"},   {3,3,"<"},   SO::less,     false,  true,   true,   "<3",     "<2",     "{<3}",           "{}",             "{[2,3)}",        },
        { __LINE__,  {2,2,"<="},  {},          SO::greater,  false,  false,  false,  "<=2",    "{}",     "{<=2}",          "{<=2}",          "{<=2}",          },
        { __LINE__,  {2,2,"<="},  {0,0,"*"},   SO::less,     false,  true,   true,   "*",      "<=2",    "{*}",            "{}",             "{>2}",           },
        { __LINE__,  {2,2,"<="},  {1},         SO::less,     true,   true,   true,   "<=2",    "1",      "{<=2}",          "{<1,(1,2]}",     "{<1,(1,2]}",     },
        { __LINE__,  {2,2,"<="},  {2},         SO::less,     true,   true,   true,   "<=2",    "2",      "{<=2}",          "{<2}",           "{<2}",           },
        { __LINE__,  {2,2,"<="},  {3},         SO::less,     false,  false,  false,  "<=3",    "{}",     "{<=2,3}",        "{<=2}",          "{<=2,3}",        },
        { __LINE__,  {2,2,"<="},  {1,1,"<"},   SO::greater,  true,   true,   true,   "<=2",    "<1",     "{<=2}",          "{[1,2]}",        "{[1,2]}",        },
        { __LINE__,  {2,2,"<="},  {2,2,"<"},   SO::greater,  true,   true,   true,   "<=2",    "<2",     "{<=2}",          "{2}",            "{2}",            },
        { __LINE__,  {2,2,"<="},  {3,3,"<"},   SO::less,     false,  true,   true,   "<3",     "<=2",    "{<3}",           "{}",             "{(2,3)}",        },
        { __LINE__,  {2,2,"<="},  {1,1,"<="},  SO::greater,  true,   true,   true,   "<=2",    "<=1",    "{<=2}",          "{(1,2]}",        "{(1,2]}",        },
        { __LINE__,  {2,2,"<="},  {2,2,"<="},  SO::equal,    true,   true,   true,   "<=2",    "<=2",    "{<=2}",          "{}",             "{}",             },
        { __LINE__,  {2,2,"<="},  {3,3,"<="},  SO::less,     false,  true,   true,   "<=3",    "<=2",    "{<=3}",          "{}",             "{(2,3]}",        },
        { __LINE__,  {2,2,">"},   {},          SO::greater,  false,  false,  false,  ">2",     "{}",     "{>2}",           "{>2}",           "{>2}",           },
        { __LINE__,  {2,2,">"},   {0,0,"*"},   SO::greater,  false,  true,   true,   "*",      ">2",     "{*}",            "{}",             "{<=2}",          },
        { __LINE__,  {2,2,">"},   {1},         SO::greater,  false,  false,  false,  ">=1",    "{}",     "{1,>2}",         "{>2}",           "{1,>2}",         },
        { __LINE__,  {2,2,">"},   {2},         SO::greater,  false,  false,  true,   ">=2",    "{}",     "{>=2}",          "{>2}",           "{>=2}",          },
        { __LINE__,  {2,2,">"},   {3},         SO::less,     true,   true,   true,   ">2",     "3",      "{>2}",           "{(2,3),>3}",     "{(2,3),>3}",     },
        { __LINE__,  {2,2,">"},   {1,1,"<"},   SO::greater,  false,  false,  false,  "*",      "{}",     "{<1,>2}",        "{>2}",           "{<1,>2}",        },
        { __LINE__,  {2,2,">"},   {2,2,"<"},   SO::greater,  false,  false,  false,  "*",      "{}",     "{<2,>2}",        "{>2}",           "{<2,>2}",        },
        { __LINE__,  {2,2,">"},   {3,3,"<"},   SO::greater,  false,  true,   true,   "*",      "(2,3)",  "{*}",            "{>=3}",          "{<=2,>=3}",      },
        { __LINE__,  {2,2,">"},   {1,1,"<="},  SO::greater,  false,  false,  false,  "*",      "{}",     "{<=1,>2}",       "{>2}",           "{<=1,>2}",       },
        { __LINE__,  {2,2,">"},   {2,2,"<="},  SO::greater,  false,  false,  true,   "*",      "{}",     "{*}",            "{>2}",           "{*}",            },
        { __LINE__,  {2,2,">"},   {3,3,"<="},  SO::greater,  false,  true,   true,   "*",      "(2,3]",  "{*}",            "{>3}",           "{<=2,>3}",       },
        { __LINE__,  {2,2,">"},   {1,1,">"},   SO::greater,  false,  true,   true,   ">1",     ">2",     "{>1}",           "{}",             "{(1,2]}",        },
        { __LINE__,  {2,2,">"},   {2,2,">"},   SO::equal,    true,   true,   true,   ">2",     ">2",     "{>2}",           "{}",             "{}",             },
        { __LINE__,  {2,2,">"},   {3,3,">"},   SO::less,     true,   true,   true,   ">2",     ">3",     "{>2}",           "{(2,3]}",        "{(2,3]}",        },
        { __LINE__,  {2,2,">="},  {},          SO::greater,  false,  false,  false,  ">=2",    "{}",     "{>=2}",          "{>=2}",          "{>=2}",          },
        { __LINE__,  {2,2,">="},  {0,0,"*"},   SO::greater,  false,  true,   true,   "*",      ">=2",    "{*}",            "{}",             "{<2}",           },
        { __LINE__,  {2,2,">="},  {1},         SO::greater,  false,  false,  false,  ">=1",    "{}",     "{1,>=2}",        "{>=2}",          "{1,>=2}",        },
        { __LINE__,  {2,2,">="},  {2},         SO::greater,  true,   true,   true,   ">=2",    "2",      "{>=2}",          "{>2}",           "{>2}",           },
        { __LINE__,  {2,2,">="},  {3},         SO::less,     true,   true,   true,   ">=2",    "3",      "{>=2}",          "{[2,3),>3}",     "{[2,3),>3}",     },
        { __LINE__,  {2,2,">="},  {1,1,"<"},   SO::greater,  false,  false,  false,  "*",      "{}",     "{<1,>=2}",       "{>=2}",          "{<1,>=2}",       },
        { __LINE__,  {2,2,">="},  {2,2,"<"},   SO::greater,  false,  false,  true,   "*",      "{}",     "{*}",            "{>=2}",          "{*}",            },
        { __LINE__,  {2,2,">="},  {3,3,"<"},   SO::greater,  false,  true,   true,   "*",      "[2,3)",  "{*}",            "{>=3}",          "{<2,>=3}",       },
        { __LINE__,  {2,2,">="},  {1,1,"<="},  SO::greater,  false,  false,  false,  "*",      "{}",     "{<=1,>=2}",      "{>=2}",          "{<=1,>=2}",      },
        { __LINE__,  {2,2,">="},  {2,2,"<="},  SO::greater,  false,  true,   true,   "*",      "2",      "{*}",            "{>2}",           "{<2,>2}",        },
        { __LINE__,  {2,2,">="},  {3,3,"<="},  SO::greater,  false,  true,   true,   "*",      "[2,3]",  "{*}",            "{>3}",           "{<2,>3}",        },
        { __LINE__,  {2,2,">="},  {1,1,">"},   SO::greater,  false,  true,   true,   ">1",     ">=2",    "{>1}",           "{}",             "{(1,2)}",        },
        { __LINE__,  {2,2,">="},  {2,2,">"},   SO::less,     true,   true,   true,   ">=2",    ">2",     "{>=2}",          "{2}",            "{2}",            },
        { __LINE__,  {2,2,">="},  {3,3,">"},   SO::less,     true,   true,   true,   ">=2",    ">3",     "{>=2}",          "{[2,3]}",        "{[2,3]}",        },
        { __LINE__,  {2,2,">="},  {1,1,">="},  SO::greater,  false,  true,   true,   ">=1",    ">=2",    "{>=1}",          "{}",             "{[1,2)}",        },
        { __LINE__,  {2,2,">="},  {2,2,">="},  SO::equal,    true,   true,   true,   ">=2",    ">=2",    "{>=2}",          "{}",             "{}",             },
        { __LINE__,  {2,2,">="},  {3,3,">="},  SO::less,     true,   true,   true,   ">=2",    ">=3",    "{>=2}",          "{[2,3)}",        "{[2,3)}",        },
        { __LINE__,  {2,4,"()"},  {},          SO::greater,  false,  false,  false,  "(2,4)",  "{}",     "{(2,4)}",        "{(2,4)}",        "{(2,4)}",        },
        { __LINE__,  {2,4,"()"},  {0,0,"*"},   SO::greater,  false,  true,   true,   "*",      "(2,4)",  "{*}",            "{}",             "{<=2,>=4}",      },
        { __LINE__,  {2,4,"()"},  {1},         SO::greater,  false,  false,  false,  "[1,4)",  "{}",     "{1,(2,4)}",      "{(2,4)}",        "{1,(2,4)}",      },
        { __LINE__,  {2,4,"()"},  {2},         SO::greater,  false,  false,  true,   "[2,4)",  "{}",     "{[2,4)}",        "{(2,4)}",        "{[2,4)}",        },
        { __LINE__,  {2,4,"()"},  {3},         SO::less,     true,   true,   true,   "(2,4)",  "3",      "{(2,4)}",        "{(2,3),(3,4)}",  "{(2,3),(3,4)}",  },
        { __LINE__,  {2,4,"()"},  {4},         SO::less,     false,  false,  true,   "(2,4]",  "{}",     "{(2,4]}",        "{(2,4)}",        "{(2,4]}",        },
        { __LINE__,  {2,4,"()"},  {5},         SO::less,     false,  false,  false,  "(2,5]",  "{}",     "{(2,4),5}",      "{(2,4)}",        "{(2,4),5}",      },
        { __LINE__,  {2,4,"()"},  {1,1,"<"},   SO::greater,  false,  false,  false,  "<4",     "{}",     "{<1,(2,4)}",     "{(2,4)}",        "{<1,(2,4)}",     },
        { __LINE__,  {2,4,"()"},  {2,2,"<"},   SO::greater,  false,  false,  false,  "<4",     "{}",     "{<2,(2,4)}",     "{(2,4)}",        "{<2,(2,4)}",     },
        { __LINE__,  {2,4,"()"},  {3,3,"<"},   SO::greater,  false,  true,   true,   "<4",     "(2,3)",  "{<4}",           "{[3,4)}",        "{<=2,[3,4)}",    },
        { __LINE__,  {2,4,"()"},  {4,4,"<"},   SO::greater,  false,  true,   true,   "<4",     "(2,4)",  "{<4}",           "{}",             "{<=2}",          },
        { __LINE__,  {2,4,"()"},  {5,5,"<"},   SO::greater,  false,  true,   true,   "<5",     "(2,4)",  "{<5}",           "{}",             "{<=2,[4,5)}",    },
        { __LINE__,  {2,4,"()"},  {1,1,"<="},  SO::greater,  false,  false,  false,  "<4",     "{}",     "{<=1,(2,4)}",    "{(2,4)}",        "{<=1,(2,4)}",    },
        { __LINE__,  {2,4,"()"},  {2,2,"<="},  SO::greater,  false,  false,  true,   "<4",     "{}",     "{<4}",           "{(2,4)}",        "{<4}",           },
        { __LINE__,  {2,4,"()"},  {3,3,"<="},  SO::greater,  false,  true,   true,   "<4",     "(2,3]",  "{<4}",           "{(3,4)}",        "{<=2,(3,4)}",    },
        { __LINE__,  {2,4,"()"},  {4,4,"<="},  SO::greater,  false,  true,   true,   "<=4",    "(2,4)",  "{<=4}",          "{}",             "{<=2,4}",        },
        { __LINE__,  {2,4,"()"},  {5,5,"<="},  SO::greater,  false,  true,   true,   "<=5",    "(2,4)",  "{<=5}",          "{}",             "{<=2,[4,5]}",    },
        { __LINE__,  {2,4,"()"},  {1,1,">"},   SO::greater,  false,  true,   true,   ">1",     "(2,4)",  "{>1}",           "{}",             "{(1,2],>=4}",    },
        { __LINE__,  {2,4,"()"},  {2,2,">"},   SO::less,     false,  true,   true,   ">2",     "(2,4)",  "{>2}",           "{}",             "{>=4}",          },
        { __LINE__,  {2,4,"()"},  {3,3,">"},   SO::less,     false,  true,   true,   ">2",     "(3,4)",  "{>2}",           "{(2,3]}",        "{(2,3],>=4}",    },
        { __LINE__,  {2,4,"()"},  {4,4,">"},   SO::less,     false,  false,  false,  ">2",     "{}",     "{(2,4),>4}",     "{(2,4)}",        "{(2,4),>4}",     },
        { __LINE__,  {2,4,"()"},  {5,5,">"},   SO::less,     false,  false,  false,  ">2",     "{}",     "{(2,4),>5}",     "{(2,4)}",        "{(2,4),>5}",     },
        { __LINE__,  {2,4,"()"},  {1,1,">="},  SO::greater,  false,  true,   true,   ">=1",    "(2,4)",  "{>=1}",          "{}",             "{[1,2],>=4}",    },
        { __LINE__,  {2,4,"()"},  {2,2,">="},  SO::greater,  false,  true,   true,   ">=2",    "(2,4)",  "{>=2}",          "{}",             "{2,>=4}",        },
        { __LINE__,  {2,4,"()"},  {3,3,">="},  SO::less,     false,  true,   true,   ">2",     "[3,4)",  "{>2}",           "{(2,3)}",        "{(2,3),>=4}",    },
        { __LINE__,  {2,4,"()"},  {4,4,">="},  SO::less,     false,  false,  true,   ">2",     "{}",     "{>2}",           "{(2,4)}",        "{>2}",           },
        { __LINE__,  {2,4,"()"},  {5,5,">="},  SO::less,     false,  false,  false,  ">2",     "{}",     "{(2,4),>=5}",    "{(2,4)}",        "{(2,4),>=5}",    },
        { __LINE__,  {2,4,"()"},  {1,2,"()"},  SO::greater,  false,  false,  false,  "(1,4)",  "{}",     "{(1,2),(2,4)}",  "{(2,4)}",        "{(1,2),(2,4)}",  },
        { __LINE__,  {2,4,"()"},  {1,3,"()"},  SO::greater,  false,  true,   true,   "(1,4)",  "(2,3)",  "{(1,4)}",        "{[3,4)}",        "{(1,2],[3,4)}",  },
        { __LINE__,  {2,4,"()"},  {1,4,"()"},  SO::greater,  false,  true,   true,   "(1,4)",  "(2,4)",  "{(1,4)}",        "{}",             "{(1,2]}",        },
        { __LINE__,  {2,4,"()"},  {1,5,"()"},  SO::greater,  false,  true,   true,   "(1,5)",  "(2,4)",  "{(1,5)}",        "{}",             "{(1,2],[4,5)}",  },
        { __LINE__,  {2,4,"()"},  {2,3,"()"},  SO::greater,  true,   true,   true,   "(2,4)",  "(2,3)",  "{(2,4)}",        "{[3,4)}",        "{[3,4)}",        },
        { __LINE__,  {2,4,"()"},  {2,4,"()"},  SO::equal,    true,   true,   true,   "(2,4)",  "(2,4)",  "{(2,4)}",        "{}",             "{}",             },
        { __LINE__,  {2,4,"()"},  {2,5,"()"},  SO::less,     false,  true,   true,   "(2,5)",  "(2,4)",  "{(2,5)}",        "{}",             "{[4,5)}",        },
        { __LINE__,  {2,4,"()"},  {3,4,"()"},  SO::less,     true,   true,   true,   "(2,4)",  "(3,4)",  "{(2,4)}",        "{(2,3]}",        "{(2,3]}",        },
        { __LINE__,  {2,4,"()"},  {3,5,"()"},  SO::less,     false,  true,   true,   "(2,5)",  "(3,4)",  "{(2,5)}",        "{(2,3]}",        "{(2,3],[4,5)}",  },
        { __LINE__,  {2,4,"()"},  {4,5,"()"},  SO::less,     false,  false,  false,  "(2,5)",  "{}",     "{(2,4),(4,5)}",  "{(2,4)}",        "{(2,4),(4,5)}",  },
        { __LINE__,  {2,4,"[)"},  {},          SO::greater,  false,  false,  false,  "[2,4)",  "{}",     "{[2,4)}",        "{[2,4)}",        "{[2,4)}",        },
        { __LINE__,  {2,4,"[)"},  {0,0,"*"},   SO::greater,  false,  true,   true,   "*",      "[2,4)",  "{*}",            "{}",             "{<2,>=4}",       },
        { __LINE__,  {2,4,"[)"},  {1},         SO::greater,  false,  false,  false,  "[1,4)",  "{}",     "{1,[2,4)}",      "{[2,4)}",        "{1,[2,4)}",      },
        { __LINE__,  {2,4,"[)"},  {2},         SO::greater,  true,   true,   true,   "[2,4)",  "2",      "{[2,4)}",        "{(2,4)}",        "{(2,4)}",        },
        { __LINE__,  {2,4,"[)"},  {3},         SO::less,     true,   true,   true,   "[2,4)",  "3",      "{[2,4)}",        "{[2,3),(3,4)}",  "{[2,3),(3,4)}",  },
        { __LINE__,  {2,4,"[)"},  {4},         SO::less,     false,  false,  true,   "[2,4]",  "{}",     "{[2,4]}",        "{[2,4)}",        "{[2,4]}",        },
        { __LINE__,  {2,4,"[)"},  {5},         SO::less,     false,  false,  false,  "[2,5]",  "{}",     "{[2,4),5}",      "{[2,4)}",        "{[2,4),5}",      },
        { __LINE__,  {2,4,"[)"},  {1,1,"<"},   SO::greater,  false,  false,  false,  "<4",     "{}",     "{<1,[2,4)}",     "{[2,4)}",        "{<1,[2,4)}",     },
        { __LINE__,  {2,4,"[)"},  {2,2,"<"},   SO::greater,  false,  false,  true,   "<4",     "{}",     "{<4}",           "{[2,4)}",        "{<4}",           },
        { __LINE__,  {2,4,"[)"},  {3,3,"<"},   SO::greater,  false,  true,   true,   "<4",     "[2,3)",  "{<4}",           "{[3,4)}",        "{<2,[3,4)}",     },
        { __LINE__,  {2,4,"[)"},  {4,4,"<"},   SO::greater,  false,  true,   true,   "<4",     "[2,4)",  "{<4}",           "{}",             "{<2}",           },
        { __LINE__,  {2,4,"[)"},  {5,5,"<"},   SO::greater,  false,  true,   true,   "<5",     "[2,4)",  "{<5}",           "{}",             "{<2,[4,5)}",     },
        { __LINE__,  {2,4,"[)"},  {1,1,"<="},  SO::greater,  false,  false,  false,  "<4",     "{}",     "{<=1,[2,4)}",    "{[2,4)}",        "{<=1,[2,4)}",    },
        { __LINE__,  {2,4,"[)"},  {2,2,"<="},  SO::greater,  false,  true,   true,   "<4",     "2",      "{<4}",           "{(2,4)}",        "{<2,(2,4)}",     },
        { __LINE__,  {2,4,"[)"},  {3,3,"<="},  SO::greater,  false,  true,   true,   "<4",     "[2,3]",  "{<4}",           "{(3,4)}",        "{<2,(3,4)}",     },
        { __LINE__,  {2,4,"[)"},  {4,4,"<="},  SO::greater,  false,  true,   true,   "<=4",    "[2,4)",  "{<=4}",          "{}",             "{<2,4}",         },
        { __LINE__,  {2,4,"[)"},  {5,5,"<="},  SO::greater,  false,  true,   true,   "<=5",    "[2,4)",  "{<=5}",          "{}",             "{<2,[4,5]}",     },
        { __LINE__,  {2,4,"[)"},  {1,1,">"},   SO::greater,  false,  true,   true,   ">1",     "[2,4)",  "{>1}",           "{}",             "{(1,2),>=4}",    },
        { __LINE__,  {2,4,"[)"},  {2,2,">"},   SO::less,     false,  true,   true,   ">=2",    "(2,4)",  "{>=2}",          "{2}",            "{2,>=4}",        },
        { __LINE__,  {2,4,"[)"},  {3,3,">"},   SO::less,     false,  true,   true,   ">=2",    "(3,4)",  "{>=2}",          "{[2,3]}",        "{[2,3],>=4}",    },
        { __LINE__,  {2,4,"[)"},  {4,4,">"},   SO::less,     false,  false,  false,  ">=2",    "{}",     "{[2,4),>4}",     "{[2,4)}",        "{[2,4),>4}",     },
        { __LINE__,  {2,4,"[)"},  {5,5,">"},   SO::less,     false,  false,  false,  ">=2",    "{}",     "{[2,4),>5}",     "{[2,4)}",        "{[2,4),>5}",     },
        { __LINE__,  {2,4,"[)"},  {1,1,">="},  SO::greater,  false,  true,   true,   ">=1",    "[2,4)",  "{>=1}",          "{}",             "{[1,2),>=4}",    },
        { __LINE__,  {2,4,"[)"},  {2,2,">="},  SO::less,     false,  true,   true,   ">=2",    "[2,4)",  "{>=2}",          "{}",             "{>=4}",          },
        { __LINE__,  {2,4,"[)"},  {3,3,">="},  SO::less,     false,  true,   true,   ">=2",    "[3,4)",  "{>=2}",          "{[2,3)}",        "{[2,3),>=4}",    },
        { __LINE__,  {2,4,"[)"},  {4,4,">="},  SO::less,     false,  false,  true,   ">=2",    "{}",     "{>=2}",          "{[2,4)}",        "{>=2}",          },
        { __LINE__,  {2,4,"[)"},  {5,5,">="},  SO::less,     false,  false,  false,  ">=2",    "{}",     "{[2,4),>=5}",    "{[2,4)}",        "{[2,4),>=5}",    },
        { __LINE__,  {2,4,"[)"},  {1,2,"()"},  SO::greater,  false,  false,  true,   "(1,4)",  "{}",     "{(1,4)}",        "{[2,4)}",        "{(1,4)}",        },
        { __LINE__,  {2,4,"[)"},  {1,3,"()"},  SO::greater,  false,  true,   true,   "(1,4)",  "[2,3)",  "{(1,4)}",        "{[3,4)}",        "{(1,2),[3,4)}",  },
        { __LINE__,  {2,4,"[)"},  {1,4,"()"},  SO::greater,  false,  true,   true,   "(1,4)",  "[2,4)",  "{(1,4)}",        "{}",             "{(1,2)}",        },
        { __LINE__,  {2,4,"[)"},  {1,5,"()"},  SO::greater,  false,  true,   true,   "(1,5)",  "[2,4)",  "{(1,5)}",        "{}",             "{(1,2),[4,5)}",  },
        { __LINE__,  {2,4,"[)"},  {2,3,"()"},  SO::less,     true,   true,   true,   "[2,4)",  "(2,3)",  "{[2,4)}",        "{2,[3,4)}",      "{2,[3,4)}",      },
        { __LINE__,  {2,4,"[)"},  {2,4,"()"},  SO::less,     true,   true,   true,   "[2,4)",  "(2,4)",  "{[2,4)}",        "{2}",            "{2}",            },
        { __LINE__,  {2,4,"[)"},  {2,5,"()"},  SO::less,     false,  true,   true,   "[2,5)",  "(2,4)",  "{[2,5)}",        "{2}",            "{2,[4,5)}",      },
        { __LINE__,  {2,4,"[)"},  {3,4,"()"},  SO::less,     true,   true,   true,   "[2,4)",  "(3,4)",  "{[2,4)}",        "{[2,3]}",        "{[2,3]}",        },
        { __LINE__,  {2,4,"[)"},  {3,5,"()"},  SO::less,     false,  true,   true,   "[2,5)",  "(3,4)",  "{[2,5)}",        "{[2,3]}",        "{[2,3],[4,5)}",  },
        { __LINE__,  {2,4,"[)"},  {4,5,"()"},  SO::less,     false,  false,  false,  "[2,5)",  "{}",     "{[2,4),(4,5)}",  "{[2,4)}",        "{[2,4),(4,5)}",  },
        { __LINE__,  {2,4,"[)"},  {1,2,"[)"},  SO::greater,  false,  false,  true,   "[1,4)",  "{}",     "{[1,4)}",        "{[2,4)}",        "{[1,4)}",        },
        { __LINE__,  {2,4,"[)"},  {1,3,"[)"},  SO::greater,  false,  true,   true,   "[1,4)",  "[2,3)",  "{[1,4)}",        "{[3,4)}",        "{[1,2),[3,4)}",  },
        { __LINE__,  {2,4,"[)"},  {1,4,"[)"},  SO::greater,  false,  true,   true,   "[1,4)",  "[2,4)",  "{[1,4)}",        "{}",             "{[1,2)}",        },
        { __LINE__,  {2,4,"[)"},  {1,5,"[)"},  SO::greater,  false,  true,   true,   "[1,5)",  "[2,4)",  "{[1,5)}",        "{}",             "{[1,2),[4,5)}",  },
        { __LINE__,  {2,4,"[)"},  {2,3,"[)"},  SO::greater,  true,   true,   true,   "[2,4)",  "[2,3)",  "{[2,4)}",        "{[3,4)}",        "{[3,4)}",        },
        { __LINE__,  {2,4,"[)"},  {2,4,"[)"},  SO::equal,    true,   true,   true,   "[2,4)",  "[2,4)",  "{[2,4)}",        "{}",             "{}",             },
        { __LINE__,  {2,4,"[)"},  {2,5,"[)"},  SO::less,     false,  true,   true,   "[2,5)",  "[2,4)",  "{[2,5)}",        "{}",             "{[4,5)}",        },
        { __LINE__,  {2,4,"[)"},  {3,4,"[)"},  SO::less,     true,   true,   true,   "[2,4)",  "[3,4)",  "{[2,4)}",        "{[2,3)}",        "{[2,3)}",        },
        { __LINE__,  {2,4,"[)"},  {3,5,"[)"},  SO::less,     false,  true,   true,   "[2,5)",  "[3,4)",  "{[2,5)}",        "{[2,3)}",        "{[2,3),[4,5)}",  },
        { __LINE__,  {2,4,"[)"},  {4,5,"[)"},  SO::less,     false,  false,  true,   "[2,5)",  "{}",     "{[2,5)}",        "{[2,4)}",        "{[2,5)}",        },
        { __LINE__,  {2,4,"(]"},  {},          SO::greater,  false,  false,  false,  "(2,4]",  "{}",     "{(2,4]}",        "{(2,4]}",        "{(2,4]}",        },
        { __LINE__,  {2,4,"(]"},  {0,0,"*"},   SO::greater,  false,  true,   true,   "*",      "(2,4]",  "{*}",            "{}",             "{<=2,>4}",       },
        { __LINE__,  {2,4,"(]"},  {1},         SO::greater,  false,  false,  false,  "[1,4]",  "{}",     "{1,(2,4]}",      "{(2,4]}",        "{1,(2,4]}",      },
        { __LINE__,  {2,4,"(]"},  {2},         SO::greater,  false,  false,  true,   "[2,4]",  "{}",     "{[2,4]}",        "{(2,4]}",        "{[2,4]}",        },
        { __LINE__,  {2,4,"(]"},  {3},         SO::less,     true,   true,   true,   "(2,4]",  "3",      "{(2,4]}",        "{(2,3),(3,4]}",  "{(2,3),(3,4]}",  },
        { __LINE__,  {2,4,"(]"},  {4},         SO::less,     true,   true,   true,   "(2,4]",  "4",      "{(2,4]}",        "{(2,4)}",        "{(2,4)}",        },
        { __LINE__,  {2,4,"(]"},  {5},         SO::less,     false,  false,  false,  "(2,5]",  "{}",     "{(2,4],5}",      "{(2,4]}",        "{(2,4],5}",      },
        { __LINE__,  {2,4,"(]"},  {1,1,"<"},   SO::greater,  false,  false,  false,  "<=4",    "{}",     "{<1,(2,4]}",     "{(2,4]}",        "{<1,(2,4]}",     },
        { __LINE__,  {2,4,"(]"},  {2,2,"<"},   SO::greater,  false,  false,  false,  "<=4",    "{}",     "{<2,(2,4]}",     "{(2,4]}",        "{<2,(2,4]}",     },
        { __LINE__,  {2,4,"(]"},  {3,3,"<"},   SO::greater,  false,  true,   true,   "<=4",    "(2,3)",  "{<=4}",          "{[3,4]}",        "{<=2,[3,4]}",    },
        { __LINE__,  {2,4,"(]"},  {4,4,"<"},   SO::greater,  false,  true,   true,   "<=4",    "(2,4)",  "{<=4}",          "{4}",            "{<=2,4}",        },
        { __LINE__,  {2,4,"(]"},  {5,5,"<"},   SO::greater,  false,  true,   true,   "<5",     "(2,4]",  "{<5}",           "{}",             "{<=2,(4,5)}",    },
        { __LINE__,  {2,4,"(]"},  {1,1,"<="},  SO::greater,  false,  false,  false,  "<=4",    "{}",     "{<=1,(2,4]}",    "{(2,4]}",        "{<=1,(2,4]}",    },
        { __LINE__,  {2,4,"(]"},  {2,2,"<="},  SO::greater,  false,  false,  true,   "<=4",    "{}",     "{<=4}",          "{(2,4]}",        "{<=4}",          },
        { __LINE__,  {2,4,"(]"},  {3,3,"<="},  SO::greater,  false,  true,   true,   "<=4",    "(2,3]",  "{<=4}",          "{(3,4]}",        "{<=2,(3,4]}",    },
        { __LINE__,  {2,4,"(]"},  {4,4,"<="},  SO::greater,  false,  true,   true,   "<=4",    "(2,4]",  "{<=4}",          "{}",             "{<=2}",          },
        { __LINE__,  {2,4,"(]"},  {5,5,"<="},  SO::greater,  false,  true,   true,   "<=5",    "(2,4]",  "{<=5}",          "{}",             "{<=2,(4,5]}",    },
        { __LINE__,  {2,4,"(]"},  {1,1,">"},   SO::greater,  false,  true,   true,   ">1",     "(2,4]",  "{>1}",           "{}",             "{(1,2],>4}",     },
        { __LINE__,  {2,4,"(]"},  {2,2,">"},   SO::less,     false,  true,   true,   ">2",     "(2,4]",  "{>2}",           "{}",             "{>4}",           },
        { __LINE__,  {2,4,"(]"},  {3,3,">"},   SO::less,     false,  true,   true,   ">2",     "(3,4]",  "{>2}",           "{(2,3]}",        "{(2,3],>4}",     },
        { __LINE__,  {2,4,"(]"},  {4,4,">"},   SO::less,     false,  false,  true,   ">2",     "{}",     "{>2}",           "{(2,4]}",        "{>2}",           },
        { __LINE__,  {2,4,"(]"},  {5,5,">"},   SO::less,     false,  false,  false,  ">2",     "{}",     "{(2,4],>5}",     "{(2,4]}",        "{(2,4],>5}",     },
        { __LINE__,  {2,4,"(]"},  {1,1,">="},  SO::greater,  false,  true,   true,   ">=1",    "(2,4]",  "{>=1}",          "{}",             "{[1,2],>4}",     },
        { __LINE__,  {2,4,"(]"},  {2,2,">="},  SO::greater,  false,  true,   true,   ">=2",    "(2,4]",  "{>=2}",          "{}",             "{2,>4}",         },
        { __LINE__,  {2,4,"(]"},  {3,3,">="},  SO::less,     false,  true,   true,   ">2",     "[3,4]",  "{>2}",           "{(2,3)}",        "{(2,3),>4}",     },
        { __LINE__,  {2,4,"(]"},  {4,4,">="},  SO::less,     false,  true,   true,   ">2",     "4",      "{>2}",           "{(2,4)}",        "{(2,4),>4}",     },
        { __LINE__,  {2,4,"(]"},  {5,5,">="},  SO::less,     false,  false,  false,  ">2",     "{}",     "{(2,4],>=5}",    "{(2,4]}",        "{(2,4],>=5}",    },
        { __LINE__,  {2,4,"(]"},  {1,2,"()"},  SO::greater,  false,  false,  false,  "(1,4]",  "{}",     "{(1,2),(2,4]}",  "{(2,4]}",        "{(1,2),(2,4]}",  },
        { __LINE__,  {2,4,"(]"},  {1,3,"()"},  SO::greater,  false,  true,   true,   "(1,4]",  "(2,3)",  "{(1,4]}",        "{[3,4]}",        "{(1,2],[3,4]}",  },
        { __LINE__,  {2,4,"(]"},  {1,4,"()"},  SO::greater,  false,  true,   true,   "(1,4]",  "(2,4)",  "{(1,4]}",        "{4}",            "{(1,2],4}",      },
        { __LINE__,  {2,4,"(]"},  {1,5,"()"},  SO::greater,  false,  true,   true,   "(1,5)",  "(2,4]",  "{(1,5)}",        "{}",             "{(1,2],(4,5)}",  },
        { __LINE__,  {2,4,"(]"},  {2,3,"()"},  SO::greater,  true,   true,   true,   "(2,4]",  "(2,3)",  "{(2,4]}",        "{[3,4]}",        "{[3,4]}",        },
        { __LINE__,  {2,4,"(]"},  {2,4,"()"},  SO::greater,  true,   true,   true,   "(2,4]",  "(2,4)",  "{(2,4]}",        "{4}",            "{4}",            },
        { __LINE__,  {2,4,"(]"},  {2,5,"()"},  SO::less,     false,  true,   true,   "(2,5)",  "(2,4]",  "{(2,5)}",        "{}",             "{(4,5)}",        },
        { __LINE__,  {2,4,"(]"},  {3,4,"()"},  SO::less,     true,   true,   true,   "(2,4]",  "(3,4)",  "{(2,4]}",        "{(2,3],4}",      "{(2,3],4}",      },
        { __LINE__,  {2,4,"(]"},  {3,5,"()"},  SO::less,     false,  true,   true,   "(2,5)",  "(3,4]",  "{(2,5)}",        "{(2,3]}",        "{(2,3],(4,5)}",  },
        { __LINE__,  {2,4,"(]"},  {4,5,"()"},  SO::less,     false,  false,  true,   "(2,5)",  "{}",     "{(2,5)}",        "{(2,4]}",        "{(2,5)}",        },
        { __LINE__,  {2,4,"(]"},  {1,2,"[)"},  SO::greater,  false,  false,  false,  "[1,4]",  "{}",     "{[1,2),(2,4]}",  "{(2,4]}",        "{[1,2),(2,4]}",  },
        { __LINE__,  {2,4,"(]"},  {1,3,"[)"},  SO::greater,  false,  true,   true,   "[1,4]",  "(2,3)",  "{[1,4]}",        "{[3,4]}",        "{[1,2],[3,4]}",  },
        { __LINE__,  {2,4,"(]"},  {1,4,"[)"},  SO::greater,  false,  true,   true,   "[1,4]",  "(2,4)",  "{[1,4]}",        "{4}",            "{[1,2],4}",      },
        { __LINE__,  {2,4,"(]"},  {1,5,"[)"},  SO::greater,  false,  true,   true,   "[1,5)",  "(2,4]",  "{[1,5)}",        "{}",             "{[1,2],(4,5)}",  },
        { __LINE__,  {2,4,"(]"},  {2,3,"[)"},  SO::greater,  false,  true,   true,   "[2,4]",  "(2,3)",  "{[2,4]}",        "{[3,4]}",        "{2,[3,4]}",      },
        { __LINE__,  {2,4,"(]"},  {2,4,"[)"},  SO::greater,  false,  true,   true,   "[2,4]",  "(2,4)",  "{[2,4]}",        "{4}",            "{2,4}",          },
        { __LINE__,  {2,4,"(]"},  {2,5,"[)"},  SO::greater,  false,  true,   true,   "[2,5)",  "(2,4]",  "{[2,5)}",        "{}",             "{2,(4,5)}",      },
        { __LINE__,  {2,4,"(]"},  {3,4,"[)"},  SO::less,     true,   true,   true,   "(2,4]",  "[3,4)",  "{(2,4]}",        "{(2,3),4}",      "{(2,3),4}",      },
        { __LINE__,  {2,4,"(]"},  {3,5,"[)"},  SO::less,     false,  true,   true,   "(2,5)",  "[3,4]",  "{(2,5)}",        "{(2,3)}",        "{(2,3),(4,5)}",  },
        { __LINE__,  {2,4,"(]"},  {4,5,"[)"},  SO::less,     false,  true,   true,   "(2,5)",  "4",      "{(2,5)}",        "{(2,4)}",        "{(2,4),(4,5)}",  },
        { __LINE__,  {2,4,"(]"},  {1,2,"(]"},  SO::greater,  false,  false,  true,   "(1,4]",  "{}",     "{(1,4]}",        "{(2,4]}",        "{(1,4]}",        },
        { __LINE__,  {2,4,"(]"},  {1,3,"(]"},  SO::greater,  false,  true,   true,   "(1,4]",  "(2,3]",  "{(1,4]}",        "{(3,4]}",        "{(1,2],(3,4]}",  },
        { __LINE__,  {2,4,"(]"},  {1,4,"(]"},  SO::greater,  false,  true,   true,   "(1,4]",  "(2,4]",  "{(1,4]}",        "{}",             "{(1,2]}",        },
        { __LINE__,  {2,4,"(]"},  {1,5,"(]"},  SO::greater,  false,  true,   true,   "(1,5]",  "(2,4]",  "{(1,5]}",        "{}",             "{(1,2],(4,5]}",  },
        { __LINE__,  {2,4,"(]"},  {2,3,"(]"},  SO::greater,  true,   true,   true,   "(2,4]",  "(2,3]",  "{(2,4]}",        "{(3,4]}",        "{(3,4]}",        },
        { __LINE__,  {2,4,"(]"},  {2,4,"(]"},  SO::equal,    true,   true,   true,   "(2,4]",  "(2,4]",  "{(2,4]}",        "{}",             "{}",             },
        { __LINE__,  {2,4,"(]"},  {2,5,"(]"},  SO::less,     false,  true,   true,   "(2,5]",  "(2,4]",  "{(2,5]}",        "{}",             "{(4,5]}",        },
        { __LINE__,  {2,4,"(]"},  {3,4,"(]"},  SO::less,     true,   true,   true,   "(2,4]",  "(3,4]",  "{(2,4]}",        "{(2,3]}",        "{(2,3]}",        },
        { __LINE__,  {2,4,"(]"},  {3,5,"(]"},  SO::less,     false,  true,   true,   "(2,5]",  "(3,4]",  "{(2,5]}",        "{(2,3]}",        "{(2,3],(4,5]}",  },
        { __LINE__,  {2,4,"(]"},  {4,5,"(]"},  SO::less,     false,  false,  true,   "(2,5]",  "{}",     "{(2,5]}",        "{(2,4]}",        "{(2,5]}",        },
        { __LINE__,  {2,4,"[]"},  {},          SO::greater,  false,  false,  false,  "[2,4]",  "{}",     "{[2,4]}",        "{[2,4]}",        "{[2,4]}",        },
        { __LINE__,  {2,4,"[]"},  {0,0,"*"},   SO::greater,  false,  true,   true,   "*",      "[2,4]",  "{*}",            "{}",             "{<2,>4}",        },
        { __LINE__,  {2,4,"[]"},  {1},         SO::greater,  false,  false,  false,  "[1,4]",  "{}",     "{1,[2,4]}",      "{[2,4]}",        "{1,[2,4]}",      },
        { __LINE__,  {2,4,"[]"},  {2},         SO::greater,  true,   true,   true,   "[2,4]",  "2",      "{[2,4]}",        "{(2,4]}",        "{(2,4]}",        },
        { __LINE__,  {2,4,"[]"},  {3},         SO::less,     true,   true,   true,   "[2,4]",  "3",      "{[2,4]}",        "{[2,3),(3,4]}",  "{[2,3),(3,4]}",  },
        { __LINE__,  {2,4,"[]"},  {4},         SO::less,     true,   true,   true,   "[2,4]",  "4",      "{[2,4]}",        "{[2,4)}",        "{[2,4)}",        },
        { __LINE__,  {2,4,"[]"},  {5},         SO::less,     false,  false,  false,  "[2,5]",  "{}",     "{[2,4],5}",      "{[2,4]}",        "{[2,4],5}",      },
        { __LINE__,  {2,4,"[]"},  {1,1,"<"},   SO::greater,  false,  false,  false,  "<=4",    "{}",     "{<1,[2,4]}",     "{[2,4]}",        "{<1,[2,4]}",     },
        { __LINE__,  {2,4,"[]"},  {2,2,"<"},   SO::greater,  false,  false,  true,   "<=4",    "{}",     "{<=4}",          "{[2,4]}",        "{<=4}",          },
        { __LINE__,  {2,4,"[]"},  {3,3,"<"},   SO::greater,  false,  true,   true,   "<=4",    "[2,3)",  "{<=4}",          "{[3,4]}",        "{<2,[3,4]}",     },
        { __LINE__,  {2,4,"[]"},  {4,4,"<"},   SO::greater,  false,  true,   true,   "<=4",    "[2,4)",  "{<=4}",          "{4}",            "{<2,4}",         },
        { __LINE__,  {2,4,"[]"},  {5,5,"<"},   SO::greater,  false,  true,   true,   "<5",     "[2,4]",  "{<5}",           "{}",             "{<2,(4,5)}",     },
        { __LINE__,  {2,4,"[]"},  {1,1,"<="},  SO::greater,  false,  false,  false,  "<=4",    "{}",     "{<=1,[2,4]}",    "{[2,4]}",        "{<=1,[2,4]}",    },
        { __LINE__,  {2,4,"[]"},  {2,2,"<="},  SO::greater,  false,  true,   true,   "<=4",    "2",      "{<=4}",          "{(2,4]}",        "{<2,(2,4]}",     },
        { __LINE__,  {2,4,"[]"},  {3,3,"<="},  SO::greater,  false,  true,   true,   "<=4",    "[2,3]",  "{<=4}",          "{(3,4]}",        "{<2,(3,4]}",     },
        { __LINE__,  {2,4,"[]"},  {4,4,"<="},  SO::greater,  false,  true,   true,   "<=4",    "[2,4]",  "{<=4}",          "{}",             "{<2}",           },
        { __LINE__,  {2,4,"[]"},  {5,5,"<="},  SO::greater,  false,  true,   true,   "<=5",    "[2,4]",  "{<=5}",          "{}",             "{<2,(4,5]}",     },
        { __LINE__,  {2,4,"[]"},  {1,1,">"},   SO::greater,  false,  true,   true,   ">1",     "[2,4]",  "{>1}",           "{}",             "{(1,2),>4}",     },
        { __LINE__,  {2,4,"[]"},  {2,2,">"},   SO::less,     false,  true,   true,   ">=2",    "(2,4]",  "{>=2}",          "{2}",            "{2,>4}",         },
        { __LINE__,  {2,4,"[]"},  {3,3,">"},   SO::less,     false,  true,   true,   ">=2",    "(3,4]",  "{>=2}",          "{[2,3]}",        "{[2,3],>4}",     },
        { __LINE__,  {2,4,"[]"},  {4,4,">"},   SO::less,     false,  false,  true,   ">=2",    "{}",     "{>=2}",          "{[2,4]}",        "{>=2}",          },
        { __LINE__,  {2,4,"[]"},  {5,5,">"},   SO::less,     false,  false,  false,  ">=2",    "{}",     "{[2,4],>5}",     "{[2,4]}",        "{[2,4],>5}",     },
        { __LINE__,  {2,4,"[]"},  {1,1,">="},  SO::greater,  false,  true,   true,   ">=1",    "[2,4]",  "{>=1}",          "{}",             "{[1,2),>4}",     },
        { __LINE__,  {2,4,"[]"},  {2,2,">="},  SO::less,     false,  true,   true,   ">=2",    "[2,4]",  "{>=2}",          "{}",             "{>4}",           },
        { __LINE__,  {2,4,"[]"},  {3,3,">="},  SO::less,     false,  true,   true,   ">=2",    "[3,4]",  "{>=2}",          "{[2,3)}",        "{[2,3),>4}",     },
        { __LINE__,  {2,4,"[]"},  {4,4,">="},  SO::less,     false,  true,   true,   ">=2",    "4",      "{>=2}",          "{[2,4)}",        "{[2,4),>4}",     },
        { __LINE__,  {2,4,"[]"},  {5,5,">="},  SO::less,     false,  false,  false,  ">=2",    "{}",     "{[2,4],>=5}",    "{[2,4]}",        "{[2,4],>=5}",    },
        { __LINE__,  {2,4,"[]"},  {1,2,"()"},  SO::greater,  false,  false,  true,   "(1,4]",  "{}",     "{(1,4]}",        "{[2,4]}",        "{(1,4]}",        },
        { __LINE__,  {2,4,"[]"},  {1,3,"()"},  SO::greater,  false,  true,   true,   "(1,4]",  "[2,3)",  "{(1,4]}",        "{[3,4]}",        "{(1,2),[3,4]}",  },
        { __LINE__,  {2,4,"[]"},  {1,4,"()"},  SO::greater,  false,  true,   true,   "(1,4]",  "[2,4)",  "{(1,4]}",        "{4}",            "{(1,2),4}",      },
        { __LINE__,  {2,4,"[]"},  {1,5,"()"},  SO::greater,  false,  true,   true,   "(1,5)",  "[2,4]",  "{(1,5)}",        "{}",             "{(1,2),(4,5)}",  },
        { __LINE__,  {2,4,"[]"},  {2,3,"()"},  SO::less,     true,   true,   true,   "[2,4]",  "(2,3)",  "{[2,4]}",        "{2,[3,4]}",      "{2,[3,4]}",      },
        { __LINE__,  {2,4,"[]"},  {2,4,"()"},  SO::less,     true,   true,   true,   "[2,4]",  "(2,4)",  "{[2,4]}",        "{2,4}",          "{2,4}",          },
        { __LINE__,  {2,4,"[]"},  {2,5,"()"},  SO::less,     false,  true,   true,   "[2,5)",  "(2,4]",  "{[2,5)}",        "{2}",            "{2,(4,5)}",      },
        { __LINE__,  {2,4,"[]"},  {3,4,"()"},  SO::less,     true,   true,   true,   "[2,4]",  "(3,4)",  "{[2,4]}",        "{[2,3],4}",      "{[2,3],4}",      },
        { __LINE__,  {2,4,"[]"},  {3,5,"()"},  SO::less,     false,  true,   true,   "[2,5)",  "(3,4]",  "{[2,5)}",        "{[2,3]}",        "{[2,3],(4,5)}",  },
        { __LINE__,  {2,4,"[]"},  {4,5,"()"},  SO::less,     false,  false,  true,   "[2,5)",  "{}",     "{[2,5)}",        "{[2,4]}",        "{[2,5)}",        },
        { __LINE__,  {2,4,"[]"},  {1,2,"[)"},  SO::greater,  false,  false,  true,   "[1,4]",  "{}",     "{[1,4]}",        "{[2,4]}",        "{[1,4]}",        },
        { __LINE__,  {2,4,"[]"},  {1,3,"[)"},  SO::greater,  false,  true,   true,   "[1,4]",  "[2,3)",  "{[1,4]}",        "{[3,4]}",        "{[1,2),[3,4]}",  },
        { __LINE__,  {2,4,"[]"},  {1,4,"[)"},  SO::greater,  false,  true,   true,   "[1,4]",  "[2,4)",  "{[1,4]}",        "{4}",            "{[1,2),4}",      },
        { __LINE__,  {2,4,"[]"},  {1,5,"[)"},  SO::greater,  false,  true,   true,   "[1,5)",  "[2,4]",  "{[1,5)}",        "{}",             "{[1,2),(4,5)}",  },
        { __LINE__,  {2,4,"[]"},  {2,3,"[)"},  SO::greater,  true,   true,   true,   "[2,4]",  "[2,3)",  "{[2,4]}",        "{[3,4]}",        "{[3,4]}",        },
        { __LINE__,  {2,4,"[]"},  {2,4,"[)"},  SO::greater,  true,   true,   true,   "[2,4]",  "[2,4)",  "{[2,4]}",        "{4}",            "{4}",            },
        { __LINE__,  {2,4,"[]"},  {2,5,"[)"},  SO::less,     false,  true,   true,   "[2,5)",  "[2,4]",  "{[2,5)}",        "{}",             "{(4,5)}",        },
        { __LINE__,  {2,4,"[]"},  {3,4,"[)"},  SO::less,     true,   true,   true,   "[2,4]",  "[3,4)",  "{[2,4]}",        "{[2,3),4}",      "{[2,3),4}",      },
        { __LINE__,  {2,4,"[]"},  {3,5,"[)"},  SO::less,     false,  true,   true,   "[2,5)",  "[3,4]",  "{[2,5)}",        "{[2,3)}",        "{[2,3),(4,5)}",  },
        { __LINE__,  {2,4,"[]"},  {4,5,"[)"},  SO::less,     false,  true,   true,   "[2,5)",  "4",      "{[2,5)}",        "{[2,4)}",        "{[2,4),(4,5)}",  },
        { __LINE__,  {2,4,"[]"},  {1,2,"(]"},  SO::greater,  false,  true,   true,   "(1,4]",  "2",      "{(1,4]}",        "{(2,4]}",        "{(1,2),(2,4]}",  },
        { __LINE__,  {2,4,"[]"},  {1,3,"(]"},  SO::greater,  false,  true,   true,   "(1,4]",  "[2,3]",  "{(1,4]}",        "{(3,4]}",        "{(1,2),(3,4]}",  },
        { __LINE__,  {2,4,"[]"},  {1,4,"(]"},  SO::greater,  false,  true,   true,   "(1,4]",  "[2,4]",  "{(1,4]}",        "{}",             "{(1,2)}",        },
        { __LINE__,  {2,4,"[]"},  {1,5,"(]"},  SO::greater,  false,  true,   true,   "(1,5]",  "[2,4]",  "{(1,5]}",        "{}",             "{(1,2),(4,5]}",  },
        { __LINE__,  {2,4,"[]"},  {2,3,"(]"},  SO::less,     true,   true,   true,   "[2,4]",  "(2,3]",  "{[2,4]}",        "{2,(3,4]}",      "{2,(3,4]}",      },
        { __LINE__,  {2,4,"[]"},  {2,4,"(]"},  SO::less,     true,   true,   true,   "[2,4]",  "(2,4]",  "{[2,4]}",        "{2}",            "{2}",            },
        { __LINE__,  {2,4,"[]"},  {2,5,"(]"},  SO::less,     false,  true,   true,   "[2,5]",  "(2,4]",  "{[2,5]}",        "{2}",            "{2,(4,5]}",      },
        { __LINE__,  {2,4,"[]"},  {3,4,"(]"},  SO::less,     true,   true,   true,   "[2,4]",  "(3,4]",  "{[2,4]}",        "{[2,3]}",        "{[2,3]}",        },
        { __LINE__,  {2,4,"[]"},  {3,5,"(]"},  SO::less,     false,  true,   true,   "[2,5]",  "(3,4]",  "{[2,5]}",        "{[2,3]}",        "{[2,3],(4,5]}",  },
        { __LINE__,  {2,4,"[]"},  {4,5,"(]"},  SO::less,     false,  false,  true,   "[2,5]",  "{}",     "{[2,5]}",        "{[2,4]}",        "{[2,5]}",        },
        { __LINE__,  {2,4,"[]"},  {1,1,"[]"},  SO::greater,  false,  false,  false,  "[1,4]",  "{}",     "{1,[2,4]}",      "{[2,4]}",        "{1,[2,4]}",      },
        { __LINE__,  {2,4,"[]"},  {1,2,"[]"},  SO::greater,  false,  true,   true,   "[1,4]",  "2",      "{[1,4]}",        "{(2,4]}",        "{[1,2),(2,4]}",  },
        { __LINE__,  {2,4,"[]"},  {1,3,"[]"},  SO::greater,  false,  true,   true,   "[1,4]",  "[2,3]",  "{[1,4]}",        "{(3,4]}",        "{[1,2),(3,4]}",  },
        { __LINE__,  {2,4,"[]"},  {1,4,"[]"},  SO::greater,  false,  true,   true,   "[1,4]",  "[2,4]",  "{[1,4]}",        "{}",             "{[1,2)}",        },
        { __LINE__,  {2,4,"[]"},  {1,5,"[]"},  SO::greater,  false,  true,   true,   "[1,5]",  "[2,4]",  "{[1,5]}",        "{}",             "{[1,2),(4,5]}",  },
        { __LINE__,  {2,4,"[]"},  {2,3,"[]"},  SO::greater,  true,   true,   true,   "[2,4]",  "[2,3]",  "{[2,4]}",        "{(3,4]}",        "{(3,4]}",        },
        { __LINE__,  {2,4,"[]"},  {2,4,"[]"},  SO::equal,    true,   true,   true,   "[2,4]",  "[2,4]",  "{[2,4]}",        "{}",             "{}",             },
        { __LINE__,  {2,4,"[]"},  {2,5,"[]"},  SO::less,     false,  true,   true,   "[2,5]",  "[2,4]",  "{[2,5]}",        "{}",             "{(4,5]}",        },
        { __LINE__,  {2,4,"[]"},  {3,4,"[]"},  SO::less,     true,   true,   true,   "[2,4]",  "[3,4]",  "{[2,4]}",        "{[2,3)}",        "{[2,3)}",        },
        { __LINE__,  {2,4,"[]"},  {3,5,"[]"},  SO::less,     false,  true,   true,   "[2,5]",  "[3,4]",  "{[2,5]}",        "{[2,3)}",        "{[2,3),(4,5]}",  },
        { __LINE__,  {2,4,"[]"},  {4,5,"[]"},  SO::less,     false,  true,   true,   "[2,5]",  "4",      "{[2,5]}",        "{[2,4)}",        "{[2,4),(4,5]}",  },

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

void test_rs_interval_continuous_interval_hashing() {

    std::unordered_set<Itv> set;

    TRY(set.insert(Itv(1.0)));
    TRY(set.insert(Itv(2.0, 3.0)));
    TRY(set.insert(Itv(4.0, 4.0, ">")));
    TEST_EQUAL(set.size(), 3u);

}
