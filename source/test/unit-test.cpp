#include "test/unit-test.hpp"

int main(int argc, char** argv) {

    RS::UnitTest::begin_tests(argc, argv);

    // format-test.cpp
    UNIT_TEST(rs_format_interval_integer)
    UNIT_TEST(rs_format_interval_floating_point)

    // type-traits-test.cpp
    UNIT_TEST(rs_interval_enumeration)
    UNIT_TEST(rs_interval_type_traits)

    // ordered-test.cpp
    UNIT_TEST(rs_ordered_interval_basic_properties)
    UNIT_TEST(rs_ordered_interval_construction)
    UNIT_TEST(rs_ordered_interval_inverse)
    UNIT_TEST(rs_ordered_interval_binary_operations)

    // ordered-set-test.cpp
    UNIT_TEST(rs_ordered_interval_set_construct_insert_erase)
    UNIT_TEST(rs_ordered_interval_set_operations)

    // ordered-map-test.cpp
    UNIT_TEST(rs_ordered_interval_map)

    // stepwise-test.cpp
    UNIT_TEST(rs_stepwise_interval_basic_properties)
    UNIT_TEST(rs_stepwise_interval_construction)
    UNIT_TEST(rs_stepwise_interval_iterators)
    UNIT_TEST(rs_stepwise_interval_inverse)
    UNIT_TEST(rs_stepwise_interval_binary_operations)

    // stepwise-set-test.cpp
    UNIT_TEST(rs_stepwise_interval_set_construct_insert_erase)
    UNIT_TEST(rs_stepwise_interval_set_operations)

    // stepwise-map-test.cpp
    UNIT_TEST(rs_stepwise_interval_map)

    // integral-test.cpp
    UNIT_TEST(rs_integral_interval_basic_properties)
    UNIT_TEST(rs_integral_interval_construction)
    UNIT_TEST(rs_integral_interval_iterators)
    UNIT_TEST(rs_integral_interval_inverse)
    UNIT_TEST(rs_integral_interval_binary_operations)

    // integral-set-test.cpp
    UNIT_TEST(rs_integral_interval_set_construct_insert_erase)
    UNIT_TEST(rs_integral_interval_set_operations)

    // integral-map-test.cpp
    UNIT_TEST(rs_integral_interval_map)

    // integral-arithmetic-test.cpp
    UNIT_TEST(rs_integral_interval_arithmetic)

    // continuous-test.cpp
    UNIT_TEST(rs_continuous_interval_basic_properties)
    UNIT_TEST(rs_continuous_interval_construction)
    UNIT_TEST(rs_continuous_interval_inverse)
    UNIT_TEST(rs_continuous_interval_binary_operations)

    // continuous-set-test.cpp
    UNIT_TEST(rs_continuous_interval_set_construct_insert_erase)
    UNIT_TEST(rs_continuous_interval_set_operations)

    // continuous-map-test.cpp
    UNIT_TEST(rs_continuous_interval_map)

    // continuous-arithmetic-test.cpp
    UNIT_TEST(rs_continuous_interval_arithmetic)

    return RS::UnitTest::end_tests();

}
