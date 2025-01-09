#include "rs-interval/interval.hpp"
#include "rs-interval/set.hpp"
#include "rs-interval/types.hpp"
#include "test/stepwise-example.hpp"
#include "test/unit-test.hpp"
#include <compare>
#include <concepts>
#include <cstddef>
#include <format>
#include <iterator>
#include <print>
#include <string>
#include <unordered_set>
#include <vector>

using namespace RS::Interval;

using Itv = Interval<StepwiseType>;
using Set = IntervalSet<StepwiseType>;
using SO = std::strong_ordering;

static_assert(std::same_as<Itv::value_type, StepwiseType>);

void test_rs_interval_stepwise_interval_basic_properties() {

    Itv in;

    TEST_EQUAL(Itv::category, Category::stepwise);
    TEST(in.empty());
    TEST_EQUAL(in.size(), 0u);
    TEST(! in);
    TEST_EQUAL(std::format("{}", in), "{}");
    TEST_EQUAL(in.match(0_sw), Match::empty);
    TEST_EQUAL(in.match(42_sw), Match::empty);
    TEST(! in(0_sw));
    TEST(! in(42_sw));

}

void test_rs_interval_stepwise_interval_construction() {

    Itv in;
    std::string str;

    TRY(in = Itv());                                        TRY(str = std::format("{}", in));  TEST_EQUAL(str, "{}");
    TRY(in = Itv(1_sw));                                    TRY(str = std::format("{}", in));  TEST_EQUAL(str, "1");
    TRY(in = Itv(1_sw, Bound::closed, Bound::closed));      TRY(str = std::format("{}", in));  TEST_EQUAL(str, "1");
    TRY(in = Itv(1_sw, Bound::closed, Bound::open));        TRY(str = std::format("{}", in));  TEST_EQUAL(str, "{}");
    TRY(in = Itv(1_sw, Bound::open, Bound::closed));        TRY(str = std::format("{}", in));  TEST_EQUAL(str, "{}");
    TRY(in = Itv(1_sw, Bound::closed, Bound::unbound));     TRY(str = std::format("{}", in));  TEST_EQUAL(str, ">=1");
    TRY(in = Itv(1_sw, Bound::unbound, Bound::closed));     TRY(str = std::format("{}", in));  TEST_EQUAL(str, "<=1");
    TRY(in = Itv(1_sw, Bound::open, Bound::unbound));       TRY(str = std::format("{}", in));  TEST_EQUAL(str, ">=2");
    TRY(in = Itv(1_sw, Bound::unbound, Bound::open));       TRY(str = std::format("{}", in));  TEST_EQUAL(str, "<=0");
    TRY(in = Itv(1_sw, Bound::unbound, Bound::unbound));    TRY(str = std::format("{}", in));  TEST_EQUAL(str, "*");
    TRY(in = Itv(1_sw, 5_sw));                              TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[1,5]");
    TRY(in = Itv(1_sw, 5_sw, Bound::closed));               TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[1,5]");
    TRY(in = Itv(1_sw, 5_sw, Bound::open));                 TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[2,4]");
    TRY(in = Itv(1_sw, 5_sw, Bound::closed, Bound::open));  TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[1,4]");
    TRY(in = Itv(1_sw, 5_sw, Bound::open, Bound::closed));  TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[2,5]");
    TRY(in = Itv(1_sw, 5_sw, "[]"));                        TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[1,5]");
    TRY(in = Itv(1_sw, 5_sw, "()"));                        TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[2,4]");
    TRY(in = Itv(1_sw, 5_sw, "[)"));                        TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[1,4]");
    TRY(in = Itv(1_sw, 5_sw, "(]"));                        TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[2,5]");
    TRY(in = Itv(1_sw, 5_sw, "<"));                         TRY(str = std::format("{}", in));  TEST_EQUAL(str, "<=4");
    TRY(in = Itv(1_sw, 5_sw, "<="));                        TRY(str = std::format("{}", in));  TEST_EQUAL(str, "<=5");
    TRY(in = Itv(1_sw, 5_sw, ">"));                         TRY(str = std::format("{}", in));  TEST_EQUAL(str, ">=2");
    TRY(in = Itv(1_sw, 5_sw, ">="));                        TRY(str = std::format("{}", in));  TEST_EQUAL(str, ">=1");
    TRY(in = Itv(1_sw, 5_sw, "*"));                         TRY(str = std::format("{}", in));  TEST_EQUAL(str, "*");
    TRY(in = Itv(5_sw, 1_sw));                              TRY(str = std::format("{}", in));  TEST_EQUAL(str, "{}");

    TRY(in = make_interval(1_sw));                                    TRY(str = std::format("{}", in));  TEST_EQUAL(str, "1");
    TRY(in = make_interval(1_sw, Bound::closed, Bound::closed));      TRY(str = std::format("{}", in));  TEST_EQUAL(str, "1");
    TRY(in = make_interval(1_sw, Bound::closed, Bound::open));        TRY(str = std::format("{}", in));  TEST_EQUAL(str, "{}");
    TRY(in = make_interval(1_sw, Bound::open, Bound::closed));        TRY(str = std::format("{}", in));  TEST_EQUAL(str, "{}");
    TRY(in = make_interval(1_sw, Bound::closed, Bound::unbound));     TRY(str = std::format("{}", in));  TEST_EQUAL(str, ">=1");
    TRY(in = make_interval(1_sw, Bound::unbound, Bound::closed));     TRY(str = std::format("{}", in));  TEST_EQUAL(str, "<=1");
    TRY(in = make_interval(1_sw, Bound::open, Bound::unbound));       TRY(str = std::format("{}", in));  TEST_EQUAL(str, ">=2");
    TRY(in = make_interval(1_sw, Bound::unbound, Bound::open));       TRY(str = std::format("{}", in));  TEST_EQUAL(str, "<=0");
    TRY(in = make_interval(1_sw, Bound::unbound, Bound::unbound));    TRY(str = std::format("{}", in));  TEST_EQUAL(str, "*");
    TRY(in = make_interval(1_sw, 5_sw));                              TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[1,5]");
    TRY(in = make_interval(1_sw, 5_sw, Bound::closed));               TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[1,5]");
    TRY(in = make_interval(1_sw, 5_sw, Bound::open));                 TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[2,4]");
    TRY(in = make_interval(1_sw, 5_sw, Bound::closed, Bound::open));  TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[1,4]");
    TRY(in = make_interval(1_sw, 5_sw, Bound::open, Bound::closed));  TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[2,5]");
    TRY(in = make_interval(1_sw, 5_sw, "[]"));                        TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[1,5]");
    TRY(in = make_interval(1_sw, 5_sw, "()"));                        TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[2,4]");
    TRY(in = make_interval(1_sw, 5_sw, "[)"));                        TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[1,4]");
    TRY(in = make_interval(1_sw, 5_sw, "(]"));                        TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[2,5]");
    TRY(in = make_interval(1_sw, 5_sw, "<"));                         TRY(str = std::format("{}", in));  TEST_EQUAL(str, "<=4");
    TRY(in = make_interval(1_sw, 5_sw, "<="));                        TRY(str = std::format("{}", in));  TEST_EQUAL(str, "<=5");
    TRY(in = make_interval(1_sw, 5_sw, ">"));                         TRY(str = std::format("{}", in));  TEST_EQUAL(str, ">=2");
    TRY(in = make_interval(1_sw, 5_sw, ">="));                        TRY(str = std::format("{}", in));  TEST_EQUAL(str, ">=1");
    TRY(in = make_interval(1_sw, 5_sw, "*"));                         TRY(str = std::format("{}", in));  TEST_EQUAL(str, "*");
    TRY(in = make_interval(5_sw, 1_sw));                              TRY(str = std::format("{}", in));  TEST_EQUAL(str, "{}");

    TRY(in = ordered_interval(5_sw, 1_sw, Bound::closed));               TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[1,5]");
    TRY(in = ordered_interval(5_sw, 1_sw, Bound::open));                 TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[2,4]");
    TRY(in = ordered_interval(5_sw, 1_sw, Bound::closed, Bound::open));  TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[2,5]");
    TRY(in = ordered_interval(5_sw, 1_sw, Bound::open, Bound::closed));  TRY(str = std::format("{}", in));  TEST_EQUAL(str, "[1,4]");

}

void test_rs_interval_stepwise_interval_to_string() {

    Itv in;
    std::string str;

    TRY(in = make_interval(1_sw));                                    TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc1xyz");
    TRY(in = make_interval(1_sw, Bound::closed, Bound::closed));      TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc1xyz");
    TRY(in = make_interval(1_sw, Bound::closed, Bound::open));        TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc{}xyz");
    TRY(in = make_interval(1_sw, Bound::open, Bound::closed));        TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc{}xyz");
    TRY(in = make_interval(1_sw, Bound::closed, Bound::unbound));     TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc>=1xyz");
    TRY(in = make_interval(1_sw, Bound::unbound, Bound::closed));     TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc<=1xyz");
    TRY(in = make_interval(1_sw, Bound::open, Bound::unbound));       TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc>=2xyz");
    TRY(in = make_interval(1_sw, Bound::unbound, Bound::open));       TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc<=0xyz");
    TRY(in = make_interval(1_sw, Bound::unbound, Bound::unbound));    TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc*xyz");
    TRY(in = make_interval(1_sw, 5_sw));                              TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc[1,5]xyz");
    TRY(in = make_interval(1_sw, 5_sw, Bound::closed));               TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc[1,5]xyz");
    TRY(in = make_interval(1_sw, 5_sw, Bound::open));                 TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc[2,4]xyz");
    TRY(in = make_interval(1_sw, 5_sw, Bound::closed, Bound::open));  TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc[1,4]xyz");
    TRY(in = make_interval(1_sw, 5_sw, Bound::open, Bound::closed));  TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc[2,5]xyz");
    TRY(in = make_interval(1_sw, 5_sw, "[]"));                        TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc[1,5]xyz");
    TRY(in = make_interval(1_sw, 5_sw, "()"));                        TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc[2,4]xyz");
    TRY(in = make_interval(1_sw, 5_sw, "[)"));                        TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc[1,4]xyz");
    TRY(in = make_interval(1_sw, 5_sw, "(]"));                        TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc[2,5]xyz");
    TRY(in = make_interval(1_sw, 5_sw, "<"));                         TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc<=4xyz");
    TRY(in = make_interval(1_sw, 5_sw, "<="));                        TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc<=5xyz");
    TRY(in = make_interval(1_sw, 5_sw, ">"));                         TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc>=2xyz");
    TRY(in = make_interval(1_sw, 5_sw, ">="));                        TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc>=1xyz");
    TRY(in = make_interval(1_sw, 5_sw, "*"));                         TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc*xyz");
    TRY(in = make_interval(5_sw, 1_sw));                              TRY(str = std::format("abc{}xyz", in));  TEST_EQUAL(str, "abc{}xyz");

}

void test_rs_interval_stepwise_interval_from_string() {

    Itv in;

    TRY(in = Itv::from_string(""));         TEST_EQUAL(std::format("{}", in), "{}");
    TRY(in = Itv::from_string("{}"));       TEST_EQUAL(std::format("{}", in), "{}");
    TRY(in = Itv::from_string("*"));        TEST_EQUAL(std::format("{}", in), "*");
    TRY(in = Itv::from_string("5"));        TEST_EQUAL(std::format("{}", in), "5");
    TRY(in = Itv::from_string(">5"));       TEST_EQUAL(std::format("{}", in), ">=6");
    TRY(in = Itv::from_string("<5"));       TEST_EQUAL(std::format("{}", in), "<=4");
    TRY(in = Itv::from_string(">=5"));      TEST_EQUAL(std::format("{}", in), ">=5");
    TRY(in = Itv::from_string("<=5"));      TEST_EQUAL(std::format("{}", in), "<=5");
    TRY(in = Itv::from_string("5+"));       TEST_EQUAL(std::format("{}", in), ">=5");
    TRY(in = Itv::from_string("5-"));       TEST_EQUAL(std::format("{}", in), "<=5");
    TRY(in = Itv::from_string("[5,10]"));   TEST_EQUAL(std::format("{}", in), "[5,10]");
    TRY(in = Itv::from_string("[5,10)"));   TEST_EQUAL(std::format("{}", in), "[5,9]");
    TRY(in = Itv::from_string("(5,10]"));   TEST_EQUAL(std::format("{}", in), "[6,10]");
    TRY(in = Itv::from_string("(5,10)"));   TEST_EQUAL(std::format("{}", in), "[6,9]");
    TRY(in = Itv::from_string("5-10"));     TEST_EQUAL(std::format("{}", in), "[5,10]");
    TRY(in = Itv::from_string("5..10"));    TEST_EQUAL(std::format("{}", in), "[5,10]");
    TRY(in = Itv::from_string("5...10"));   TEST_EQUAL(std::format("{}", in), "[5,10]");
    TRY(in = Itv::from_string("5..<10"));   TEST_EQUAL(std::format("{}", in), "[5,9]");
    TRY(in = Itv::from_string("5<..10"));   TEST_EQUAL(std::format("{}", in), "[6,10]");
    TRY(in = Itv::from_string("5<..<10"));  TEST_EQUAL(std::format("{}", in), "[6,9]");

}

void test_rs_interval_stepwise_interval_iterators() {

    Itv in;
    Itv::iterator it1 = {}, it2 = {};
    std::ptrdiff_t diff = 0;

    TRY(in = {});               TEST_EQUAL(in.size(), 0u);  TRY(it1 = in.begin());  TRY(it2 = in.end());  TRY(diff = std::distance(it1, it2));  TEST_EQUAL(diff, 0);
    TRY(in = 1_sw);             TEST_EQUAL(in.size(), 1u);  TRY(it1 = in.begin());  TRY(it2 = in.end());  TRY(diff = std::distance(it1, it2));  TEST_EQUAL(diff, 1);
    TRY(in = Itv(1_sw, 5_sw));  TEST_EQUAL(in.size(), 5u);  TRY(it1 = in.begin());  TRY(it2 = in.end());  TRY(diff = std::distance(it1, it2));  TEST_EQUAL(diff, 5);

    TEST_EQUAL(*it1, 1_sw);  TRY(++it1);
    TEST_EQUAL(*it1, 2_sw);  TRY(++it1);
    TEST_EQUAL(*it1, 3_sw);  TRY(++it1);
    TEST_EQUAL(*it1, 4_sw);  TRY(++it1);
    TEST_EQUAL(*it1, 5_sw);  TRY(++it1);
    TEST(it1 == it2);

}

void test_rs_interval_stepwise_interval_order() {

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

void test_rs_interval_stepwise_interval_complement() {

    Itv in;
    Set set;
    std::string str;

    TRY((in = {}));                  TRY(set = in.complement());  TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{*}");
    TRY((in = Itv::all()));          TRY(set = in.complement());  TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{}");
    TRY((in = 42_sw));               TRY(set = in.complement());  TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{<=41,>=43}");
    TRY((in = {42_sw,42_sw,"<"}));   TRY(set = in.complement());  TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{>=42}");
    TRY((in = {42_sw,42_sw,"<="}));  TRY(set = in.complement());  TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{>=43}");
    TRY((in = {42_sw,42_sw,">"}));   TRY(set = in.complement());  TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{<=42}");
    TRY((in = {42_sw,42_sw,">="}));  TRY(set = in.complement());  TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{<=41}");
    TRY((in = {86_sw,99_sw,"()"}));  TRY(set = in.complement());  TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{<=86,>=99}");
    TRY((in = {86_sw,99_sw,"(]"}));  TRY(set = in.complement());  TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{<=86,>=100}");
    TRY((in = {86_sw,99_sw,"[)"}));  TRY(set = in.complement());  TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{<=85,>=99}");
    TRY((in = {86_sw,99_sw,"[]"}));  TRY(set = in.complement());  TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{<=85,>=100}");

}

void test_rs_interval_stepwise_interval_binary_operations() {

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

        // line      lhs                rhs                cmp           incl    over    touch   envel     inter     union           diff            symm              --
        { __LINE__,  {},                {},                SO::equal,    false,  false,  false,  "{}",     "{}",     "{}",           "{}",           "{}",             },
        { __LINE__,  {0_sw,0_sw,"*"},   {},                SO::greater,  false,  false,  false,  "*",      "{}",     "{*}",          "{*}",          "{*}",            },
        { __LINE__,  {0_sw,0_sw,"*"},   {0_sw,0_sw,"*"},   SO::equal,    true,   true,   true,   "*",      "*",      "{*}",          "{}",           "{}",             },
        { __LINE__,  {3_sw},            {},                SO::greater,  false,  false,  false,  "3",      "{}",     "{3}",          "{3}",          "{3}",            },
        { __LINE__,  {3_sw},            {0_sw,0_sw,"*"},   SO::greater,  false,  true,   true,   "*",      "3",      "{*}",          "{}",           "{<=2,>=4}",      },
        { __LINE__,  {3_sw},            {1_sw},            SO::greater,  false,  false,  false,  "[1,3]",  "{}",     "{1,3}",        "{3}",          "{1,3}",          },
        { __LINE__,  {3_sw},            {2_sw},            SO::greater,  false,  false,  true,   "[2,3]",  "{}",     "{[2,3]}",      "{3}",          "{[2,3]}",        },
        { __LINE__,  {3_sw},            {3_sw},            SO::equal,    true,   true,   true,   "3",      "3",      "{3}",          "{}",           "{}",             },
        { __LINE__,  {3_sw},            {4_sw},            SO::less,     false,  false,  true,   "[3,4]",  "{}",     "{[3,4]}",      "{3}",          "{[3,4]}",        },
        { __LINE__,  {3_sw},            {5_sw},            SO::less,     false,  false,  false,  "[3,5]",  "{}",     "{3,5}",        "{3}",          "{3,5}",          },
        { __LINE__,  {3_sw,3_sw,"<"},   {},                SO::greater,  false,  false,  false,  "<=2",    "{}",     "{<=2}",        "{<=2}",        "{<=2}",          },
        { __LINE__,  {3_sw,3_sw,"<"},   {0_sw,0_sw,"*"},   SO::less,     false,  true,   true,   "*",      "<=2",    "{*}",          "{}",           "{>=3}",          },
        { __LINE__,  {3_sw,3_sw,"<"},   {1_sw},            SO::less,     true,   true,   true,   "<=2",    "1",      "{<=2}",        "{<=0,2}",      "{<=0,2}",        },
        { __LINE__,  {3_sw,3_sw,"<"},   {2_sw},            SO::less,     true,   true,   true,   "<=2",    "2",      "{<=2}",        "{<=1}",        "{<=1}",          },
        { __LINE__,  {3_sw,3_sw,"<"},   {3_sw},            SO::less,     false,  false,  true,   "<=3",    "{}",     "{<=3}",        "{<=2}",        "{<=3}",          },
        { __LINE__,  {3_sw,3_sw,"<"},   {4_sw},            SO::less,     false,  false,  false,  "<=4",    "{}",     "{<=2,4}",      "{<=2}",        "{<=2,4}",        },
        { __LINE__,  {3_sw,3_sw,"<"},   {5_sw},            SO::less,     false,  false,  false,  "<=5",    "{}",     "{<=2,5}",      "{<=2}",        "{<=2,5}",        },
        { __LINE__,  {3_sw,3_sw,"<"},   {1_sw,1_sw,"<"},   SO::greater,  true,   true,   true,   "<=2",    "<=0",    "{<=2}",        "{[1,2]}",      "{[1,2]}",        },
        { __LINE__,  {3_sw,3_sw,"<"},   {2_sw,2_sw,"<"},   SO::greater,  true,   true,   true,   "<=2",    "<=1",    "{<=2}",        "{2}",          "{2}",            },
        { __LINE__,  {3_sw,3_sw,"<"},   {3_sw,3_sw,"<"},   SO::equal,    true,   true,   true,   "<=2",    "<=2",    "{<=2}",        "{}",           "{}",             },
        { __LINE__,  {3_sw,3_sw,"<"},   {4_sw,4_sw,"<"},   SO::less,     false,  true,   true,   "<=3",    "<=2",    "{<=3}",        "{}",           "{3}",            },
        { __LINE__,  {3_sw,3_sw,"<"},   {5_sw,5_sw,"<"},   SO::less,     false,  true,   true,   "<=4",    "<=2",    "{<=4}",        "{}",           "{[3,4]}",        },
        { __LINE__,  {3_sw,3_sw,"<="},  {},                SO::greater,  false,  false,  false,  "<=3",    "{}",     "{<=3}",        "{<=3}",        "{<=3}",          },
        { __LINE__,  {3_sw,3_sw,"<="},  {0_sw,0_sw,"*"},   SO::less,     false,  true,   true,   "*",      "<=3",    "{*}",          "{}",           "{>=4}",          },
        { __LINE__,  {3_sw,3_sw,"<="},  {1_sw},            SO::less,     true,   true,   true,   "<=3",    "1",      "{<=3}",        "{<=0,[2,3]}",  "{<=0,[2,3]}",    },
        { __LINE__,  {3_sw,3_sw,"<="},  {2_sw},            SO::less,     true,   true,   true,   "<=3",    "2",      "{<=3}",        "{<=1,3}",      "{<=1,3}",        },
        { __LINE__,  {3_sw,3_sw,"<="},  {3_sw},            SO::less,     true,   true,   true,   "<=3",    "3",      "{<=3}",        "{<=2}",        "{<=2}",          },
        { __LINE__,  {3_sw,3_sw,"<="},  {4_sw},            SO::less,     false,  false,  true,   "<=4",    "{}",     "{<=4}",        "{<=3}",        "{<=4}",          },
        { __LINE__,  {3_sw,3_sw,"<="},  {5_sw},            SO::less,     false,  false,  false,  "<=5",    "{}",     "{<=3,5}",      "{<=3}",        "{<=3,5}",        },
        { __LINE__,  {3_sw,3_sw,"<="},  {1_sw,1_sw,"<"},   SO::greater,  true,   true,   true,   "<=3",    "<=0",    "{<=3}",        "{[1,3]}",      "{[1,3]}",        },
        { __LINE__,  {3_sw,3_sw,"<="},  {2_sw,2_sw,"<"},   SO::greater,  true,   true,   true,   "<=3",    "<=1",    "{<=3}",        "{[2,3]}",      "{[2,3]}",        },
        { __LINE__,  {3_sw,3_sw,"<="},  {3_sw,3_sw,"<"},   SO::greater,  true,   true,   true,   "<=3",    "<=2",    "{<=3}",        "{3}",          "{3}",            },
        { __LINE__,  {3_sw,3_sw,"<="},  {4_sw,4_sw,"<"},   SO::equal,    true,   true,   true,   "<=3",    "<=3",    "{<=3}",        "{}",           "{}",             },
        { __LINE__,  {3_sw,3_sw,"<="},  {5_sw,5_sw,"<"},   SO::less,     false,  true,   true,   "<=4",    "<=3",    "{<=4}",        "{}",           "{4}",            },
        { __LINE__,  {3_sw,3_sw,"<="},  {1_sw,1_sw,"<="},  SO::greater,  true,   true,   true,   "<=3",    "<=1",    "{<=3}",        "{[2,3]}",      "{[2,3]}",        },
        { __LINE__,  {3_sw,3_sw,"<="},  {2_sw,2_sw,"<="},  SO::greater,  true,   true,   true,   "<=3",    "<=2",    "{<=3}",        "{3}",          "{3}",            },
        { __LINE__,  {3_sw,3_sw,"<="},  {3_sw,3_sw,"<="},  SO::equal,    true,   true,   true,   "<=3",    "<=3",    "{<=3}",        "{}",           "{}",             },
        { __LINE__,  {3_sw,3_sw,"<="},  {4_sw,4_sw,"<="},  SO::less,     false,  true,   true,   "<=4",    "<=3",    "{<=4}",        "{}",           "{4}",            },
        { __LINE__,  {3_sw,3_sw,"<="},  {5_sw,5_sw,"<="},  SO::less,     false,  true,   true,   "<=5",    "<=3",    "{<=5}",        "{}",           "{[4,5]}",        },
        { __LINE__,  {3_sw,3_sw,">"},   {},                SO::greater,  false,  false,  false,  ">=4",    "{}",     "{>=4}",        "{>=4}",        "{>=4}",          },
        { __LINE__,  {3_sw,3_sw,">"},   {0_sw,0_sw,"*"},   SO::greater,  false,  true,   true,   "*",      ">=4",    "{*}",          "{}",           "{<=3}",          },
        { __LINE__,  {3_sw,3_sw,">"},   {1_sw},            SO::greater,  false,  false,  false,  ">=1",    "{}",     "{1,>=4}",      "{>=4}",        "{1,>=4}",        },
        { __LINE__,  {3_sw,3_sw,">"},   {2_sw},            SO::greater,  false,  false,  false,  ">=2",    "{}",     "{2,>=4}",      "{>=4}",        "{2,>=4}",        },
        { __LINE__,  {3_sw,3_sw,">"},   {3_sw},            SO::greater,  false,  false,  true,   ">=3",    "{}",     "{>=3}",        "{>=4}",        "{>=3}",          },
        { __LINE__,  {3_sw,3_sw,">"},   {4_sw},            SO::greater,  true,   true,   true,   ">=4",    "4",      "{>=4}",        "{>=5}",        "{>=5}",          },
        { __LINE__,  {3_sw,3_sw,">"},   {5_sw},            SO::less,     true,   true,   true,   ">=4",    "5",      "{>=4}",        "{4,>=6}",      "{4,>=6}",        },
        { __LINE__,  {3_sw,3_sw,">"},   {1_sw,1_sw,"<"},   SO::greater,  false,  false,  false,  "*",      "{}",     "{<=0,>=4}",    "{>=4}",        "{<=0,>=4}",      },
        { __LINE__,  {3_sw,3_sw,">"},   {2_sw,2_sw,"<"},   SO::greater,  false,  false,  false,  "*",      "{}",     "{<=1,>=4}",    "{>=4}",        "{<=1,>=4}",      },
        { __LINE__,  {3_sw,3_sw,">"},   {3_sw,3_sw,"<"},   SO::greater,  false,  false,  false,  "*",      "{}",     "{<=2,>=4}",    "{>=4}",        "{<=2,>=4}",      },
        { __LINE__,  {3_sw,3_sw,">"},   {4_sw,4_sw,"<"},   SO::greater,  false,  false,  true,   "*",      "{}",     "{*}",          "{>=4}",        "{*}",            },
        { __LINE__,  {3_sw,3_sw,">"},   {5_sw,5_sw,"<"},   SO::greater,  false,  true,   true,   "*",      "4",      "{*}",          "{>=5}",        "{<=3,>=5}",      },
        { __LINE__,  {3_sw,3_sw,">"},   {1_sw,1_sw,"<="},  SO::greater,  false,  false,  false,  "*",      "{}",     "{<=1,>=4}",    "{>=4}",        "{<=1,>=4}",      },
        { __LINE__,  {3_sw,3_sw,">"},   {2_sw,2_sw,"<="},  SO::greater,  false,  false,  false,  "*",      "{}",     "{<=2,>=4}",    "{>=4}",        "{<=2,>=4}",      },
        { __LINE__,  {3_sw,3_sw,">"},   {3_sw,3_sw,"<="},  SO::greater,  false,  false,  true,   "*",      "{}",     "{*}",          "{>=4}",        "{*}",            },
        { __LINE__,  {3_sw,3_sw,">"},   {4_sw,4_sw,"<="},  SO::greater,  false,  true,   true,   "*",      "4",      "{*}",          "{>=5}",        "{<=3,>=5}",      },
        { __LINE__,  {3_sw,3_sw,">"},   {5_sw,5_sw,"<="},  SO::greater,  false,  true,   true,   "*",      "[4,5]",  "{*}",          "{>=6}",        "{<=3,>=6}",      },
        { __LINE__,  {3_sw,3_sw,">"},   {1_sw,1_sw,">"},   SO::greater,  false,  true,   true,   ">=2",    ">=4",    "{>=2}",        "{}",           "{[2,3]}",        },
        { __LINE__,  {3_sw,3_sw,">"},   {2_sw,2_sw,">"},   SO::greater,  false,  true,   true,   ">=3",    ">=4",    "{>=3}",        "{}",           "{3}",            },
        { __LINE__,  {3_sw,3_sw,">"},   {3_sw,3_sw,">"},   SO::equal,    true,   true,   true,   ">=4",    ">=4",    "{>=4}",        "{}",           "{}",             },
        { __LINE__,  {3_sw,3_sw,">"},   {4_sw,4_sw,">"},   SO::less,     true,   true,   true,   ">=4",    ">=5",    "{>=4}",        "{4}",          "{4}",            },
        { __LINE__,  {3_sw,3_sw,">"},   {5_sw,5_sw,">"},   SO::less,     true,   true,   true,   ">=4",    ">=6",    "{>=4}",        "{[4,5]}",      "{[4,5]}",        },
        { __LINE__,  {3_sw,3_sw,">="},  {},                SO::greater,  false,  false,  false,  ">=3",    "{}",     "{>=3}",        "{>=3}",        "{>=3}",          },
        { __LINE__,  {3_sw,3_sw,">="},  {0_sw,0_sw,"*"},   SO::greater,  false,  true,   true,   "*",      ">=3",    "{*}",          "{}",           "{<=2}",          },
        { __LINE__,  {3_sw,3_sw,">="},  {1_sw},            SO::greater,  false,  false,  false,  ">=1",    "{}",     "{1,>=3}",      "{>=3}",        "{1,>=3}",        },
        { __LINE__,  {3_sw,3_sw,">="},  {2_sw},            SO::greater,  false,  false,  true,   ">=2",    "{}",     "{>=2}",        "{>=3}",        "{>=2}",          },
        { __LINE__,  {3_sw,3_sw,">="},  {3_sw},            SO::greater,  true,   true,   true,   ">=3",    "3",      "{>=3}",        "{>=4}",        "{>=4}",          },
        { __LINE__,  {3_sw,3_sw,">="},  {4_sw},            SO::less,     true,   true,   true,   ">=3",    "4",      "{>=3}",        "{3,>=5}",      "{3,>=5}",        },
        { __LINE__,  {3_sw,3_sw,">="},  {5_sw},            SO::less,     true,   true,   true,   ">=3",    "5",      "{>=3}",        "{[3,4],>=6}",  "{[3,4],>=6}",    },
        { __LINE__,  {3_sw,3_sw,">="},  {1_sw,1_sw,"<"},   SO::greater,  false,  false,  false,  "*",      "{}",     "{<=0,>=3}",    "{>=3}",        "{<=0,>=3}",      },
        { __LINE__,  {3_sw,3_sw,">="},  {2_sw,2_sw,"<"},   SO::greater,  false,  false,  false,  "*",      "{}",     "{<=1,>=3}",    "{>=3}",        "{<=1,>=3}",      },
        { __LINE__,  {3_sw,3_sw,">="},  {3_sw,3_sw,"<"},   SO::greater,  false,  false,  true,   "*",      "{}",     "{*}",          "{>=3}",        "{*}",            },
        { __LINE__,  {3_sw,3_sw,">="},  {4_sw,4_sw,"<"},   SO::greater,  false,  true,   true,   "*",      "3",      "{*}",          "{>=4}",        "{<=2,>=4}",      },
        { __LINE__,  {3_sw,3_sw,">="},  {5_sw,5_sw,"<"},   SO::greater,  false,  true,   true,   "*",      "[3,4]",  "{*}",          "{>=5}",        "{<=2,>=5}",      },
        { __LINE__,  {3_sw,3_sw,">="},  {1_sw,1_sw,"<="},  SO::greater,  false,  false,  false,  "*",      "{}",     "{<=1,>=3}",    "{>=3}",        "{<=1,>=3}",      },
        { __LINE__,  {3_sw,3_sw,">="},  {2_sw,2_sw,"<="},  SO::greater,  false,  false,  true,   "*",      "{}",     "{*}",          "{>=3}",        "{*}",            },
        { __LINE__,  {3_sw,3_sw,">="},  {3_sw,3_sw,"<="},  SO::greater,  false,  true,   true,   "*",      "3",      "{*}",          "{>=4}",        "{<=2,>=4}",      },
        { __LINE__,  {3_sw,3_sw,">="},  {4_sw,4_sw,"<="},  SO::greater,  false,  true,   true,   "*",      "[3,4]",  "{*}",          "{>=5}",        "{<=2,>=5}",      },
        { __LINE__,  {3_sw,3_sw,">="},  {5_sw,5_sw,"<="},  SO::greater,  false,  true,   true,   "*",      "[3,5]",  "{*}",          "{>=6}",        "{<=2,>=6}",      },
        { __LINE__,  {3_sw,3_sw,">="},  {1_sw,1_sw,">"},   SO::greater,  false,  true,   true,   ">=2",    ">=3",    "{>=2}",        "{}",           "{2}",            },
        { __LINE__,  {3_sw,3_sw,">="},  {2_sw,2_sw,">"},   SO::equal,    true,   true,   true,   ">=3",    ">=3",    "{>=3}",        "{}",           "{}",             },
        { __LINE__,  {3_sw,3_sw,">="},  {3_sw,3_sw,">"},   SO::less,     true,   true,   true,   ">=3",    ">=4",    "{>=3}",        "{3}",          "{3}",            },
        { __LINE__,  {3_sw,3_sw,">="},  {4_sw,4_sw,">"},   SO::less,     true,   true,   true,   ">=3",    ">=5",    "{>=3}",        "{[3,4]}",      "{[3,4]}",        },
        { __LINE__,  {3_sw,3_sw,">="},  {5_sw,5_sw,">"},   SO::less,     true,   true,   true,   ">=3",    ">=6",    "{>=3}",        "{[3,5]}",      "{[3,5]}",        },
        { __LINE__,  {3_sw,3_sw,">="},  {1_sw,1_sw,">="},  SO::greater,  false,  true,   true,   ">=1",    ">=3",    "{>=1}",        "{}",           "{[1,2]}",        },
        { __LINE__,  {3_sw,3_sw,">="},  {2_sw,2_sw,">="},  SO::greater,  false,  true,   true,   ">=2",    ">=3",    "{>=2}",        "{}",           "{2}",            },
        { __LINE__,  {3_sw,3_sw,">="},  {3_sw,3_sw,">="},  SO::equal,    true,   true,   true,   ">=3",    ">=3",    "{>=3}",        "{}",           "{}",             },
        { __LINE__,  {3_sw,3_sw,">="},  {4_sw,4_sw,">="},  SO::less,     true,   true,   true,   ">=3",    ">=4",    "{>=3}",        "{3}",          "{3}",            },
        { __LINE__,  {3_sw,3_sw,">="},  {5_sw,5_sw,">="},  SO::less,     true,   true,   true,   ">=3",    ">=5",    "{>=3}",        "{[3,4]}",      "{[3,4]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {},                SO::greater,  false,  false,  false,  "[3,6]",  "{}",     "{[3,6]}",      "{[3,6]}",      "{[3,6]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {0_sw,0_sw,"*"},   SO::greater,  false,  true,   true,   "*",      "[3,6]",  "{*}",          "{}",           "{<=2,>=7}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw},            SO::greater,  false,  false,  false,  "[1,6]",  "{}",     "{1,[3,6]}",    "{[3,6]}",      "{1,[3,6]}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw},            SO::greater,  false,  false,  true,   "[2,6]",  "{}",     "{[2,6]}",      "{[3,6]}",      "{[2,6]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw},            SO::greater,  true,   true,   true,   "[3,6]",  "3",      "{[3,6]}",      "{[4,6]}",      "{[4,6]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw},            SO::less,     true,   true,   true,   "[3,6]",  "4",      "{[3,6]}",      "{3,[5,6]}",    "{3,[5,6]}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw},            SO::less,     true,   true,   true,   "[3,6]",  "5",      "{[3,6]}",      "{[3,4],6}",    "{[3,4],6}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {6_sw},            SO::less,     true,   true,   true,   "[3,6]",  "6",      "{[3,6]}",      "{[3,5]}",      "{[3,5]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {7_sw},            SO::less,     false,  false,  true,   "[3,7]",  "{}",     "{[3,7]}",      "{[3,6]}",      "{[3,7]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {8_sw},            SO::less,     false,  false,  false,  "[3,8]",  "{}",     "{[3,6],8}",    "{[3,6]}",      "{[3,6],8}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,1_sw,"<"},   SO::greater,  false,  false,  false,  "<=6",    "{}",     "{<=0,[3,6]}",  "{[3,6]}",      "{<=0,[3,6]}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,2_sw,"<"},   SO::greater,  false,  false,  false,  "<=6",    "{}",     "{<=1,[3,6]}",  "{[3,6]}",      "{<=1,[3,6]}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,3_sw,"<"},   SO::greater,  false,  false,  true,   "<=6",    "{}",     "{<=6}",        "{[3,6]}",      "{<=6}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,4_sw,"<"},   SO::greater,  false,  true,   true,   "<=6",    "3",      "{<=6}",        "{[4,6]}",      "{<=2,[4,6]}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw,5_sw,"<"},   SO::greater,  false,  true,   true,   "<=6",    "[3,4]",  "{<=6}",        "{[5,6]}",      "{<=2,[5,6]}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {6_sw,6_sw,"<"},   SO::greater,  false,  true,   true,   "<=6",    "[3,5]",  "{<=6}",        "{6}",          "{<=2,6}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {7_sw,7_sw,"<"},   SO::greater,  false,  true,   true,   "<=6",    "[3,6]",  "{<=6}",        "{}",           "{<=2}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {8_sw,8_sw,"<"},   SO::greater,  false,  true,   true,   "<=7",    "[3,6]",  "{<=7}",        "{}",           "{<=2,7}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,1_sw,"<="},  SO::greater,  false,  false,  false,  "<=6",    "{}",     "{<=1,[3,6]}",  "{[3,6]}",      "{<=1,[3,6]}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,2_sw,"<="},  SO::greater,  false,  false,  true,   "<=6",    "{}",     "{<=6}",        "{[3,6]}",      "{<=6}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,3_sw,"<="},  SO::greater,  false,  true,   true,   "<=6",    "3",      "{<=6}",        "{[4,6]}",      "{<=2,[4,6]}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,4_sw,"<="},  SO::greater,  false,  true,   true,   "<=6",    "[3,4]",  "{<=6}",        "{[5,6]}",      "{<=2,[5,6]}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw,5_sw,"<="},  SO::greater,  false,  true,   true,   "<=6",    "[3,5]",  "{<=6}",        "{6}",          "{<=2,6}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {6_sw,6_sw,"<="},  SO::greater,  false,  true,   true,   "<=6",    "[3,6]",  "{<=6}",        "{}",           "{<=2}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {7_sw,7_sw,"<="},  SO::greater,  false,  true,   true,   "<=7",    "[3,6]",  "{<=7}",        "{}",           "{<=2,7}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {8_sw,8_sw,"<="},  SO::greater,  false,  true,   true,   "<=8",    "[3,6]",  "{<=8}",        "{}",           "{<=2,[7,8]}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,1_sw,">"},   SO::greater,  false,  true,   true,   ">=2",    "[3,6]",  "{>=2}",        "{}",           "{2,>=7}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,2_sw,">"},   SO::less,     false,  true,   true,   ">=3",    "[3,6]",  "{>=3}",        "{}",           "{>=7}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,3_sw,">"},   SO::less,     false,  true,   true,   ">=3",    "[4,6]",  "{>=3}",        "{3}",          "{3,>=7}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,4_sw,">"},   SO::less,     false,  true,   true,   ">=3",    "[5,6]",  "{>=3}",        "{[3,4]}",      "{[3,4],>=7}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw,5_sw,">"},   SO::less,     false,  true,   true,   ">=3",    "6",      "{>=3}",        "{[3,5]}",      "{[3,5],>=7}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {6_sw,6_sw,">"},   SO::less,     false,  false,  true,   ">=3",    "{}",     "{>=3}",        "{[3,6]}",      "{>=3}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {7_sw,7_sw,">"},   SO::less,     false,  false,  false,  ">=3",    "{}",     "{[3,6],>=8}",  "{[3,6]}",      "{[3,6],>=8}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {8_sw,8_sw,">"},   SO::less,     false,  false,  false,  ">=3",    "{}",     "{[3,6],>=9}",  "{[3,6]}",      "{[3,6],>=9}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,1_sw,">="},  SO::greater,  false,  true,   true,   ">=1",    "[3,6]",  "{>=1}",        "{}",           "{[1,2],>=7}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,2_sw,">="},  SO::greater,  false,  true,   true,   ">=2",    "[3,6]",  "{>=2}",        "{}",           "{2,>=7}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,3_sw,">="},  SO::less,     false,  true,   true,   ">=3",    "[3,6]",  "{>=3}",        "{}",           "{>=7}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,4_sw,">="},  SO::less,     false,  true,   true,   ">=3",    "[4,6]",  "{>=3}",        "{3}",          "{3,>=7}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw,5_sw,">="},  SO::less,     false,  true,   true,   ">=3",    "[5,6]",  "{>=3}",        "{[3,4]}",      "{[3,4],>=7}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {6_sw,6_sw,">="},  SO::less,     false,  true,   true,   ">=3",    "6",      "{>=3}",        "{[3,5]}",      "{[3,5],>=7}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {7_sw,7_sw,">="},  SO::less,     false,  false,  true,   ">=3",    "{}",     "{>=3}",        "{[3,6]}",      "{>=3}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {8_sw,8_sw,">="},  SO::less,     false,  false,  false,  ">=3",    "{}",     "{[3,6],>=8}",  "{[3,6]}",      "{[3,6],>=8}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,1_sw,"[]"},  SO::greater,  false,  false,  false,  "[1,6]",  "{}",     "{1,[3,6]}",    "{[3,6]}",      "{1,[3,6]}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,2_sw,"[]"},  SO::greater,  false,  false,  true,   "[1,6]",  "{}",     "{[1,6]}",      "{[3,6]}",      "{[1,6]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,3_sw,"[]"},  SO::greater,  false,  true,   true,   "[1,6]",  "3",      "{[1,6]}",      "{[4,6]}",      "{[1,2],[4,6]}",  },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,4_sw,"[]"},  SO::greater,  false,  true,   true,   "[1,6]",  "[3,4]",  "{[1,6]}",      "{[5,6]}",      "{[1,2],[5,6]}",  },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,5_sw,"[]"},  SO::greater,  false,  true,   true,   "[1,6]",  "[3,5]",  "{[1,6]}",      "{6}",          "{[1,2],6}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,6_sw,"[]"},  SO::greater,  false,  true,   true,   "[1,6]",  "[3,6]",  "{[1,6]}",      "{}",           "{[1,2]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,7_sw,"[]"},  SO::greater,  false,  true,   true,   "[1,7]",  "[3,6]",  "{[1,7]}",      "{}",           "{[1,2],7}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,8_sw,"[]"},  SO::greater,  false,  true,   true,   "[1,8]",  "[3,6]",  "{[1,8]}",      "{}",           "{[1,2],[7,8]}",  },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,2_sw,"[]"},  SO::greater,  false,  false,  true,   "[2,6]",  "{}",     "{[2,6]}",      "{[3,6]}",      "{[2,6]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,3_sw,"[]"},  SO::greater,  false,  true,   true,   "[2,6]",  "3",      "{[2,6]}",      "{[4,6]}",      "{2,[4,6]}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,4_sw,"[]"},  SO::greater,  false,  true,   true,   "[2,6]",  "[3,4]",  "{[2,6]}",      "{[5,6]}",      "{2,[5,6]}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,5_sw,"[]"},  SO::greater,  false,  true,   true,   "[2,6]",  "[3,5]",  "{[2,6]}",      "{6}",          "{2,6}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,6_sw,"[]"},  SO::greater,  false,  true,   true,   "[2,6]",  "[3,6]",  "{[2,6]}",      "{}",           "{2}",            },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,7_sw,"[]"},  SO::greater,  false,  true,   true,   "[2,7]",  "[3,6]",  "{[2,7]}",      "{}",           "{2,7}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,8_sw,"[]"},  SO::greater,  false,  true,   true,   "[2,8]",  "[3,6]",  "{[2,8]}",      "{}",           "{2,[7,8]}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,3_sw,"[]"},  SO::greater,  true,   true,   true,   "[3,6]",  "3",      "{[3,6]}",      "{[4,6]}",      "{[4,6]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,4_sw,"[]"},  SO::greater,  true,   true,   true,   "[3,6]",  "[3,4]",  "{[3,6]}",      "{[5,6]}",      "{[5,6]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,5_sw,"[]"},  SO::greater,  true,   true,   true,   "[3,6]",  "[3,5]",  "{[3,6]}",      "{6}",          "{6}",            },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,6_sw,"[]"},  SO::equal,    true,   true,   true,   "[3,6]",  "[3,6]",  "{[3,6]}",      "{}",           "{}",             },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,7_sw,"[]"},  SO::less,     false,  true,   true,   "[3,7]",  "[3,6]",  "{[3,7]}",      "{}",           "{7}",            },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,8_sw,"[]"},  SO::less,     false,  true,   true,   "[3,8]",  "[3,6]",  "{[3,8]}",      "{}",           "{[7,8]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,4_sw,"[]"},  SO::less,     true,   true,   true,   "[3,6]",  "4",      "{[3,6]}",      "{3,[5,6]}",    "{3,[5,6]}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,5_sw,"[]"},  SO::less,     true,   true,   true,   "[3,6]",  "[4,5]",  "{[3,6]}",      "{3,6}",        "{3,6}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,6_sw,"[]"},  SO::less,     true,   true,   true,   "[3,6]",  "[4,6]",  "{[3,6]}",      "{3}",          "{3}",            },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,7_sw,"[]"},  SO::less,     false,  true,   true,   "[3,7]",  "[4,6]",  "{[3,7]}",      "{3}",          "{3,7}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,8_sw,"[]"},  SO::less,     false,  true,   true,   "[3,8]",  "[4,6]",  "{[3,8]}",      "{3}",          "{3,[7,8]}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw,5_sw,"[]"},  SO::less,     true,   true,   true,   "[3,6]",  "5",      "{[3,6]}",      "{[3,4],6}",    "{[3,4],6}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw,6_sw,"[]"},  SO::less,     true,   true,   true,   "[3,6]",  "[5,6]",  "{[3,6]}",      "{[3,4]}",      "{[3,4]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw,7_sw,"[]"},  SO::less,     false,  true,   true,   "[3,7]",  "[5,6]",  "{[3,7]}",      "{[3,4]}",      "{[3,4],7}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw,8_sw,"[]"},  SO::less,     false,  true,   true,   "[3,8]",  "[5,6]",  "{[3,8]}",      "{[3,4]}",      "{[3,4],[7,8]}",  },
        { __LINE__,  {3_sw,6_sw,"[]"},  {6_sw,6_sw,"[]"},  SO::less,     true,   true,   true,   "[3,6]",  "6",      "{[3,6]}",      "{[3,5]}",      "{[3,5]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {6_sw,7_sw,"[]"},  SO::less,     false,  true,   true,   "[3,7]",  "6",      "{[3,7]}",      "{[3,5]}",      "{[3,5],7}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {6_sw,8_sw,"[]"},  SO::less,     false,  true,   true,   "[3,8]",  "6",      "{[3,8]}",      "{[3,5]}",      "{[3,5],[7,8]}",  },
        { __LINE__,  {3_sw,6_sw,"[]"},  {7_sw,7_sw,"[]"},  SO::less,     false,  false,  true,   "[3,7]",  "{}",     "{[3,7]}",      "{[3,6]}",      "{[3,7]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {7_sw,8_sw,"[]"},  SO::less,     false,  false,  true,   "[3,8]",  "{}",     "{[3,8]}",      "{[3,6]}",      "{[3,8]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {8_sw,8_sw,"[]"},  SO::less,     false,  false,  false,  "[3,8]",  "{}",     "{[3,6],8}",    "{[3,6]}",      "{[3,6],8}",      },

    };

    SO cmp = SO::equal;
    bool flag = false;
    std::string str;

    for (const auto& t: tests) {

        int errors = 0;

        TRY(cmp = t.lhs <=> t.rhs);                                           TEST(cmp == t.compare);                       errors += int(cmp != t.compare);
        TRY(flag = t.lhs.includes(t.rhs));                                    TEST_EQUAL(flag, t.includes);                 errors += int(flag != t.includes);
        TRY(flag = t.lhs.overlaps(t.rhs));                                    TEST_EQUAL(flag, t.overlaps);                 errors += int(flag != t.overlaps);
        TRY(flag = t.lhs.touches(t.rhs));                                     TEST_EQUAL(flag, t.touches);                  errors += int(flag != t.touches);
        TRY(str = std::format("{}", t.lhs.envelope(t.rhs)));                  TEST_EQUAL(str, t.envelope);                  errors += int(str != t.envelope);
        TRY(str = std::format("{}", t.lhs.set_intersection(t.rhs)));          TEST_EQUAL(str, t.set_intersection);          errors += int(str != t.set_intersection);
        TRY(str = std::format("{}", t.lhs.set_union(t.rhs)));                 TEST_EQUAL(str, t.set_union);                 errors += int(str != t.set_union);
        TRY(str = std::format("{}", t.lhs.set_difference(t.rhs)));            TEST_EQUAL(str, t.set_difference);            errors += int(str != t.set_difference);
        TRY(str = std::format("{}", t.lhs.set_symmetric_difference(t.rhs)));  TEST_EQUAL(str, t.set_symmetric_difference);  errors += int(str != t.set_symmetric_difference);

        if (errors > 0) {
            std::println("... [{}] {} {}", t.line, t.lhs, t.rhs);
        }

    }

}

void test_rs_interval_stepwise_interval_hashing() {

    std::unordered_set<Itv> set;

    TRY(set.insert(Itv(1_sw)));
    TRY(set.insert(Itv(2_sw, 3_sw)));
    TRY(set.insert(Itv(4_sw, 4_sw, ">")));
    TEST_EQUAL(set.size(), 3u);

}
