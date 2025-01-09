#include "rs-interval/interval.hpp"
#include "rs-interval/set.hpp"
#include "rs-interval/types.hpp"
#include "test/unit-test.hpp"
#include <format>
#include <print>
#include <random>
#include <string>
#include <vector>

using namespace RS::Interval;

using Itv = Interval<int>;
using Set = IntervalSet<int>;

void test_rs_interval_integral_set_construct_insert_erase() {

    Set set, com;
    Set::iterator it;
    Itv in;
    std::string str;

    TEST(set.empty());
    TRY(set = 42);
    TEST_EQUAL(set.size(), 1u);
    TRY(it = set.begin());
    TEST_EQUAL(std::format("{}", *it), "42");
    TRY(++it);
    TEST(it == set.end());
    TRY((in = {5,10}));
    TRY(set = in);
    TEST_EQUAL(set.size(), 1u);
    TRY(it = set.begin());
    TEST_EQUAL(std::format("{}", *it), "[5,10]");
    TRY(++it);
    TEST(it == set.end());
    TRY((set = {{5,10},{15,20},{25,30}}));
    TRY(it = set.begin());
    TEST_EQUAL(std::format("{}", *it), "[5,10]");
    TRY(++it);
    TEST_EQUAL(std::format("{}", *it), "[15,20]");
    TRY(++it);
    TEST_EQUAL(std::format("{}", *it), "[25,30]");
    TRY(++it);
    TEST(it == set.end());

    TRY((set = {}));                                      TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{}");
    TRY((set = {{3,6,"()"}}));                            TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{[4,5]}");
    TRY((set = {{0,5,"()"},{10,15,"()"},{20,25,"()"}}));  TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{[1,4],[11,14],[21,24]}");

    TRY((set = {{3,6},{9,12},{15,18}}));
    TRY(com = set.complement());
    TRY(str = std::format("{}", set));
    TEST_EQUAL(str, "{[3,6],[9,12],[15,18]}");
    TRY(str = std::format("{}", com));
    TEST_EQUAL(str, "{<=2,[7,8],[13,14],>=19}");

    TEST(! set.contains(1));   TEST(com.contains(1));
    TEST(! set.contains(2));   TEST(com.contains(2));
    TEST(set.contains(3));     TEST(! com.contains(3));
    TEST(set.contains(4));     TEST(! com.contains(4));
    TEST(set.contains(5));     TEST(! com.contains(5));
    TEST(set.contains(6));     TEST(! com.contains(6));
    TEST(! set.contains(7));   TEST(com.contains(7));
    TEST(! set.contains(8));   TEST(com.contains(8));
    TEST(set.contains(9));     TEST(! com.contains(9));
    TEST(set.contains(10));    TEST(! com.contains(10));
    TEST(set.contains(11));    TEST(! com.contains(11));
    TEST(set.contains(12));    TEST(! com.contains(12));
    TEST(! set.contains(13));  TEST(com.contains(13));
    TEST(! set.contains(14));  TEST(com.contains(14));
    TEST(set.contains(15));    TEST(! com.contains(15));
    TEST(set.contains(16));    TEST(! com.contains(16));
    TEST(set.contains(17));    TEST(! com.contains(17));
    TEST(set.contains(18));    TEST(! com.contains(18));
    TEST(! set.contains(19));  TEST(com.contains(19));
    TEST(! set.contains(20));  TEST(com.contains(20));

    TRY(set.clear());
    TEST(set.empty());

    TRY((set.insert({10,20})));       TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{[10,20]}");
    TRY((set.insert({20,30,"()"})));  TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{[10,29]}");
    TRY((set.erase({5,10,"()"})));    TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{[10,29]}");
    TRY((set.erase({5,10})));         TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{[11,29]}");
    TRY((set.erase({12,14})));        TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{11,[15,29]}");
    TRY((set.erase({16,18,"()"})));   TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{11,[15,16],[18,29]}");
    TRY((set.insert({9,11})));        TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{[9,11],[15,16],[18,29]}");
    TRY((set.insert({29,31,"()"})));  TRY(str = std::format("{}", set));  TEST_EQUAL(str, "{[9,11],[15,16],[18,30]}");

}

void test_rs_interval_integral_set_formatting() {

    Set set;
    std::string str;

    TRY((set = {}));                                      TRY(str = std::format("{:04x}", set));  TEST_EQUAL(str, "{}");
    TRY((set = {{3,6,"()"}}));                            TRY(str = std::format("{:04x}", set));  TEST_EQUAL(str, "{[0004,0005]}");
    TRY((set = {{0,5,"()"},{10,15,"()"},{20,25,"()"}}));  TRY(str = std::format("{:04x}", set));  TEST_EQUAL(str, "{[0001,0004],[000b,000e],[0015,0018]}");

}

void test_rs_interval_integral_set_operations() {

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

                int a = random_int(1, max_value)(rng);
                int b = random_int(1, max_value)(rng);
                auto l = Bound(random_int(0, 3)(rng));
                auto r = Bound(random_int(0, 3)(rng));

                if ((l == Bound::empty) == (r == Bound::empty)) {
                    TRY(in = Itv(a, b, l, r));
                    TRY(set[j].insert(in));
                    vec[j].push_back(in);
                }

            }

        }

        TRY(i_set = set_intersection(set[0], set[1]));
        TRY(u_set = set_union(set[0], set[1]));
        TRY(d_set = set_difference(set[0], set[1]));
        TRY(sd_set = set_symmetric_difference(set[0], set[1]));

        for (int x = 0; x <= max_value + 1; ++x) {

            bool member[2];

            for (int j = 0; j < 2; ++j) {
                member[j] = false;
                for (const auto& item: vec[j]) {
                    member[j] |= item(x);
                }
            }

            auto i_expect = member[0] && member[1];
            auto u_expect = member[0] || member[1];
            auto d_expect = member[0] && ! member[1];
            auto sd_expect = member[0] != member[1];
            auto i_test = false;
            auto u_test = false;
            auto d_test = false;
            auto sd_test = false;

            TRY(i_test = i_set[x]);
            TRY(u_test = u_set[x]);
            TRY(d_test = d_set[x]);
            TRY(sd_test = sd_set[x]);
            TEST_EQUAL(i_test, i_expect);
            TEST_EQUAL(u_test, u_expect);
            TEST_EQUAL(d_test, d_expect);
            TEST_EQUAL(sd_test, sd_expect);

            if ((i_test != i_expect) || (u_test != u_expect) || (d_test != d_expect) || (sd_test != sd_expect)) {
                for (int j = 0; j < 2; ++j) {
                    std::println("... {} => {}", vec[j], set[j]);
                }
                std::println("... x={}", x);
            }

        }

        TRY(i_set = set[0]);
        TRY(u_set = set[0]);
        TRY(d_set = set[0]);
        TRY(sd_set = set[0]);
        TRY(i_set.apply_intersection(set[1]));
        TRY(u_set.apply_union(set[1]));
        TRY(d_set.apply_difference(set[1]));
        TRY(sd_set.apply_symmetric_difference(set[1]));

        for (int x = 0; x <= max_value + 1; ++x) {

            bool member[2];

            for (int j = 0; j < 2; ++j) {
                member[j] = false;
                for (const auto& item: vec[j]) {
                    member[j] |= item(x);
                }
            }

            auto i_expect = member[0] && member[1];
            auto u_expect = member[0] || member[1];
            auto d_expect = member[0] && ! member[1];
            auto sd_expect = member[0] != member[1];
            auto i_test = false;
            auto u_test = false;
            auto d_test = false;
            auto sd_test = false;

            TRY(i_test = i_set[x]);
            TRY(u_test = u_set[x]);
            TRY(d_test = d_set[x]);
            TRY(sd_test = sd_set[x]);
            TEST_EQUAL(i_test, i_expect);
            TEST_EQUAL(u_test, u_expect);
            TEST_EQUAL(d_test, d_expect);
            TEST_EQUAL(sd_test, sd_expect);

            if ((i_test != i_expect) || (u_test != u_expect) || (d_test != d_expect) || (sd_test != sd_expect)) {
                for (int j = 0; j < 2; ++j) {
                    std::println("... {} => {}", vec[j], set[j]);
                }
                std::println("... x={}", x);
            }

        }

    }

}
