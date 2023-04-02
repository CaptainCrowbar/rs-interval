#include "rs-intervals/interval.hpp"
#include "rs-intervals/set.hpp"
#include "rs-intervals/types.hpp"
#include "test/stepwise-example.hpp"
#include "test/unit-test.hpp"
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace RS::Intervals;

using Itv = Interval<StepwiseType>;
using Set = IntervalSet<StepwiseType>;
using IB = IntervalBound;
using IC = IntervalCategory;
using IM = IntervalMatch;
using IO = IntervalOrder;

void test_rs_interval_stepwise_interval_set_construct_insert_erase() {

    Set set, com;
    Set::iterator it;
    Itv in;
    std::string str;

    TEST(set.empty());
    TRY(set = 42_sw);
    TEST_EQUAL(set.size(), 1u);
    TRY(it = set.begin());
    TEST_EQUAL(it->str(), "42");
    TRY(++it);
    TEST(it == set.end());
    TRY((in = {5_sw,10_sw}));
    TRY(set = in);
    TEST_EQUAL(set.size(), 1u);
    TRY(it = set.begin());
    TEST_EQUAL(it->str(), "[5,10]");
    TRY(++it);
    TEST(it == set.end());
    TRY((set = {{5_sw,10_sw},{15_sw,20_sw},{25_sw,30_sw}}));
    TRY(it = set.begin());
    TEST_EQUAL(it->str(), "[5,10]");
    TRY(++it);
    TEST_EQUAL(it->str(), "[15,20]");
    TRY(++it);
    TEST_EQUAL(it->str(), "[25,30]");
    TRY(++it);
    TEST(it == set.end());

    TRY((set = {}));                                                        TRY(str = set.str());  TEST_EQUAL(str, "{}");
    TRY((set = {{3_sw,6_sw,"()"}}));                                        TRY(str = set.str());  TEST_EQUAL(str, "{[4,5]}");
    TRY((set = {{0_sw,5_sw,"()"},{10_sw,15_sw,"()"},{20_sw,25_sw,"()"}}));  TRY(str = set.str());  TEST_EQUAL(str, "{[1,4],[11,14],[21,24]}");

    TRY((set = {{3_sw,6_sw},{9_sw,12_sw},{15_sw,18_sw}}));
    TRY(com = set.complement());
    TRY(str = set.str());
    TEST_EQUAL(str, "{[3,6],[9,12],[15,18]}");
    TRY(str = com.str());
    TEST_EQUAL(str, "{<=2,[7,8],[13,14],>=19}");

    TEST(! set.contains(1_sw));   TEST(com.contains(1_sw));
    TEST(! set.contains(2_sw));   TEST(com.contains(2_sw));
    TEST(set.contains(3_sw));     TEST(! com.contains(3_sw));
    TEST(set.contains(4_sw));     TEST(! com.contains(4_sw));
    TEST(set.contains(5_sw));     TEST(! com.contains(5_sw));
    TEST(set.contains(6_sw));     TEST(! com.contains(6_sw));
    TEST(! set.contains(7_sw));   TEST(com.contains(7_sw));
    TEST(! set.contains(8_sw));   TEST(com.contains(8_sw));
    TEST(set.contains(9_sw));     TEST(! com.contains(9_sw));
    TEST(set.contains(10_sw));    TEST(! com.contains(10_sw));
    TEST(set.contains(11_sw));    TEST(! com.contains(11_sw));
    TEST(set.contains(12_sw));    TEST(! com.contains(12_sw));
    TEST(! set.contains(13_sw));  TEST(com.contains(13_sw));
    TEST(! set.contains(14_sw));  TEST(com.contains(14_sw));
    TEST(set.contains(15_sw));    TEST(! com.contains(15_sw));
    TEST(set.contains(16_sw));    TEST(! com.contains(16_sw));
    TEST(set.contains(17_sw));    TEST(! com.contains(17_sw));
    TEST(set.contains(18_sw));    TEST(! com.contains(18_sw));
    TEST(! set.contains(19_sw));  TEST(com.contains(19_sw));
    TEST(! set.contains(20_sw));  TEST(com.contains(20_sw));

    TRY(set.clear());
    TEST(set.empty());

    TRY((set.insert({10_sw,20_sw})));       TRY(str = set.str());  TEST_EQUAL(str, "{[10,20]}");
    TRY((set.insert({20_sw,30_sw,"()"})));  TRY(str = set.str());  TEST_EQUAL(str, "{[10,29]}");
    TRY((set.erase({5_sw,10_sw,"()"})));    TRY(str = set.str());  TEST_EQUAL(str, "{[10,29]}");
    TRY((set.erase({5_sw,10_sw})));         TRY(str = set.str());  TEST_EQUAL(str, "{[11,29]}");
    TRY((set.erase({12_sw,14_sw})));        TRY(str = set.str());  TEST_EQUAL(str, "{11,[15,29]}");
    TRY((set.erase({16_sw,18_sw,"()"})));   TRY(str = set.str());  TEST_EQUAL(str, "{11,[15,16],[18,29]}");
    TRY((set.insert({9_sw,11_sw})));        TRY(str = set.str());  TEST_EQUAL(str, "{[9,11],[15,16],[18,29]}");
    TRY((set.insert({29_sw,31_sw,"()"})));  TRY(str = set.str());  TEST_EQUAL(str, "{[9,11],[15,16],[18,30]}");

}

void test_rs_interval_stepwise_interval_set_operations() {

    using random_int = std::uniform_int_distribution<int>;

    Set set[2], i_set, u_set, d_set, sd_set;
    std::vector<Itv> vec[2];
    Set::iterator it;
    Itv in;
    std::minstd_rand rng(42);

    static constexpr int iterations = 1000;
    static constexpr int max_size = 10;
    static constexpr int max_value = 50;

    for (int i = 0; i < iterations; ++i) {

        for (int j = 0; j < 2; ++j) {
            TRY(set[j].clear());
            vec[j].clear();
            int size = random_int(1, max_size)(rng);
            for (int k = 0; k < size; ++k) {
                StepwiseType a = random_int(1, max_value)(rng);
                StepwiseType b = random_int(1, max_value)(rng);
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

        for (int x = 0; x <= max_value + 1; ++x) {
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
            StepwiseType sx = x;
            TRY(i_test = i_set(sx));
            TRY(u_test = u_set(sx));
            TRY(d_test = d_set(sx));
            TRY(sd_test = sd_set(sx));
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
