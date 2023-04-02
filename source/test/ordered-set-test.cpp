#include "rs-intervals/interval.hpp"
#include "rs-intervals/set.hpp"
#include "rs-intervals/types.hpp"
#include "test/unit-test.hpp"
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace RS::Intervals;
using namespace std::literals;

using Itv = Interval<std::string>;
using Set = IntervalSet<std::string>;
using IB = IntervalBound;
using IC = IntervalCategory;
using IM = IntervalMatch;
using IO = IntervalOrder;

void test_rs_interval_ordered_interval_set_construct_insert_erase() {

    Set set, com;
    Set::iterator it;
    Itv in;
    std::string str;

    TEST(set.empty());
    TRY(set = "hello"s);
    TEST_EQUAL(set.size(), 1u);
    TRY(it = set.begin());
    TEST_EQUAL(it->str(), "hello");
    TRY(++it);
    TEST(it == set.end());
    TRY((in = {"hello","world"}));
    TRY(set = in);
    TEST_EQUAL(set.size(), 1u);
    TRY(it = set.begin());
    TEST_EQUAL(it->str(), "[hello,world]");
    TRY(++it);
    TEST(it == set.end());
    TRY((set = {{"abc","def"},{"ghi","jkl"},{"mno","pqr"}}));
    TRY(it = set.begin());
    TEST_EQUAL(it->str(), "[abc,def]");
    TRY(++it);
    TEST_EQUAL(it->str(), "[ghi,jkl]");
    TRY(++it);
    TEST_EQUAL(it->str(), "[mno,pqr]");
    TRY(++it);
    TEST(it == set.end());

    TRY((set = {}));                                              TRY(str = set.str());  TEST_EQUAL(str, "{}");
    TRY((set = {{"a","b","()"}}));                                TRY(str = set.str());  TEST_EQUAL(str, "{(a,b)}");
    TRY((set = {{"a","b","()"},{"c","d","()"},{"e","f","()"}}));  TRY(str = set.str());  TEST_EQUAL(str, "{(a,b),(c,d),(e,f)}");
    TRY((set = {{"a","b","[]"},{"c","d","[]"},{"e","f","[]"}}));  TRY(str = set.str());  TEST_EQUAL(str, "{[a,b],[c,d],[e,f]}");

    TRY((set = {{"a","b","[)"},{"c","d","(]"},{"e","f","()"}}));
    TRY(com = set.complement());
    TRY(str = set.str());
    TEST_EQUAL(str, "{[a,b),(c,d],(e,f)}");
    TRY(str = com.str());
    TEST_EQUAL(str, "{<a,[b,c],(d,e],>=f}");

    TEST(! set.contains("@x"));  TEST(com.contains("@x"));
    TEST(set.contains("a"));     TEST(! com.contains("a"));
    TEST(set.contains("ax"));    TEST(! com.contains("ax"));
    TEST(! set.contains("b"));   TEST(com.contains("b"));
    TEST(! set.contains("bx"));  TEST(com.contains("bx"));
    TEST(! set.contains("c"));   TEST(com.contains("c"));
    TEST(set.contains("cx"));    TEST(! com.contains("cx"));
    TEST(set.contains("d"));     TEST(! com.contains("d"));
    TEST(! set.contains("dx"));  TEST(com.contains("dx"));
    TEST(! set.contains("e"));   TEST(com.contains("e"));
    TEST(set.contains("ex"));    TEST(! com.contains("ex"));
    TEST(! set.contains("f"));   TEST(com.contains("f"));
    TEST(! set.contains("fx"));  TEST(com.contains("fx"));
    TEST(! set.contains("g"));   TEST(com.contains("g"));

    TRY(set.clear());
    TEST(set.empty());

    TRY((set.insert({"jj","tt"})));       TRY(str = set.str());  TEST_EQUAL(str, "{[jj,tt]}");
    TRY((set.insert({"tt","xx","()"})));  TRY(str = set.str());  TEST_EQUAL(str, "{[jj,xx)}");
    TRY((set.erase({"ee","jj","()"})));   TRY(str = set.str());  TEST_EQUAL(str, "{[jj,xx)}");
    TRY((set.erase({"ee","jj"})));        TRY(str = set.str());  TEST_EQUAL(str, "{(jj,xx)}");
    TRY((set.erase({"ll","nn"})));        TRY(str = set.str());  TEST_EQUAL(str, "{(jj,ll),(nn,xx)}");
    TRY((set.erase({"pp","rr","()"})));   TRY(str = set.str());  TEST_EQUAL(str, "{(jj,ll),(nn,pp],[rr,xx)}");
    TRY((set.insert({"ii","kk"})));       TRY(str = set.str());  TEST_EQUAL(str, "{[ii,ll),(nn,pp],[rr,xx)}");
    TRY((set.insert({"ww","yy","()"})));  TRY(str = set.str());  TEST_EQUAL(str, "{[ii,ll),(nn,pp],[rr,yy)}");

}

void test_rs_interval_ordered_interval_set_operations() {

    using random_int = std::uniform_int_distribution<int>;

    Set set[2], i_set, u_set, d_set, sd_set;
    std::vector<Itv> vec[2];
    Set::iterator it;
    Itv in;
    std::minstd_rand rng(42);
    std::string x;

    static constexpr int iterations = 1000;
    static constexpr int max_size = 10;

    for (int i = 0; i < iterations; ++i) {

        for (int j = 0; j < 2; ++j) {
            TRY(set[j].clear());
            vec[j].clear();
            int size = random_int(1, max_size)(rng);
            for (int k = 0; k < size; ++k) {
                auto a = std::string(2, char(random_int('a', 'z')(rng)));
                auto b = std::string(2, char(random_int('a', 'z')(rng)));
                auto l = IB(random_int(0, 3)(rng));
                auto r = IB(random_int(0, 3)(rng));
                if ((l == IB::empty) == (r == IB::empty)) {
                    TRY(in = Itv(a, b, l, r));
                    TRY(set[j].insert(in));
                    vec[j].push_back(in);
                }
            }
        }

        TRY(i_set = set[0] & set[1]);
        TRY(u_set = set[0] | set[1]);
        TRY(d_set = set[0] - set[1]);
        TRY(sd_set = set[0] ^ set[1]);

        for (char y = 'a'; y <= 'z'; ++y) {
            for (char z = 'a'; z <= 'z'; ++z) {
                x = {y,z};
                bool member[2];
                for (int j = 0; j < 2; ++j) {
                    member[j] = false;
                    for (auto& item: vec[j])
                        member[j] |= item(x);
                }
                bool i_expect = member[0] && member[1], i_test = false;
                bool u_expect = member[0] || member[1], u_test = false;
                bool d_expect = member[0] && ! member[1], d_test = false;
                bool sd_expect = member[0] != member[1], sd_test = false;
                TRY(i_test = i_set(x));
                TRY(u_test = u_set(x));
                TRY(d_test = d_set(x));
                TRY(sd_test = sd_set(x));
                TEST_EQUAL(i_test, i_expect);
                TEST_EQUAL(u_test, u_expect);
                TEST_EQUAL(d_test, d_expect);
                TEST_EQUAL(sd_test, sd_expect);
                if ((i_test != i_expect) || (u_test != u_expect) || (d_test != d_expect) || (sd_test != sd_expect)) {
                    for (int j = 0; j < 2; ++j)
                        std::cout << "... " << RS::UnitTest::format_range(vec[j]) << " => " << set[j] << "\n";
                    std::cout << "... x=" << x << "\n";
                }
            }
        }

    }

}
