#include "rs-unit-test.hpp"

int main(int argc, char** argv) {

    RS::UnitTest::begin_tests(argc, argv);

    // type-traits-test.cpp
    UNIT_TEST(rs_interval_type_traits)

    // interval-boundary-comparison-test.cpp
    UNIT_TEST(rs_interval_boundary_formatting)
    UNIT_TEST(rs_interval_boundary_comparison)

    // interval-boundary-addition-test.cpp
    UNIT_TEST(rs_interval_boundary_addition)
    UNIT_TEST(rs_interval_boundary_subtraction)

    // interval-boundary-multiplication-test.cpp
    UNIT_TEST(rs_interval_boundary_multiplication)

    // ordered-basic-test.cpp
    UNIT_TEST(rs_ordered_interval_basic_properties)
    UNIT_TEST(rs_ordered_interval_construction)
    UNIT_TEST(rs_ordered_interval_inverse)
    UNIT_TEST(rs_ordered_interval_binary_operations)
    UNIT_TEST(rs_ordered_interval_hashing)

    // ordered-set-test.cpp
    UNIT_TEST(rs_ordered_interval_set_construct_insert_erase)
    UNIT_TEST(rs_ordered_interval_set_operations)
    UNIT_TEST(rs_ordered_interval_set_hashing)

    // ordered-map-test.cpp
    UNIT_TEST(rs_ordered_interval_map)
    UNIT_TEST(rs_ordered_interval_map_hashing)

    // stepwise-basic-test.cpp
    UNIT_TEST(rs_stepwise_interval_basic_properties)
    UNIT_TEST(rs_stepwise_interval_construction)
    UNIT_TEST(rs_stepwise_interval_iterators)
    UNIT_TEST(rs_stepwise_interval_inverse)
    UNIT_TEST(rs_stepwise_interval_binary_operations)
    UNIT_TEST(rs_stepwise_interval_hashing)

    // stepwise-set-test.cpp
    UNIT_TEST(rs_stepwise_interval_set_construct_insert_erase)
    UNIT_TEST(rs_stepwise_interval_set_operations)
    UNIT_TEST(rs_stepwise_interval_set_hashing)

    // stepwise-map-test.cpp
    UNIT_TEST(rs_stepwise_interval_map)
    UNIT_TEST(rs_stepwise_interval_map_hashing)

    // integral-basic-test.cpp
    UNIT_TEST(rs_integral_interval_basic_properties)
    UNIT_TEST(rs_integral_interval_construction)
    UNIT_TEST(rs_integral_interval_iterators)
    UNIT_TEST(rs_integral_interval_inverse)
    UNIT_TEST(rs_integral_interval_binary_operations)
    UNIT_TEST(rs_integral_interval_hashing)

    // integral-set-test.cpp
    UNIT_TEST(rs_integral_interval_set_construct_insert_erase)
    UNIT_TEST(rs_integral_interval_set_operations)
    UNIT_TEST(rs_integral_interval_set_hashing)

    // integral-map-test.cpp
    UNIT_TEST(rs_integral_interval_map)
    UNIT_TEST(rs_integral_interval_map_hashing)

    // integral-arithmetic-test.cpp
    UNIT_TEST(rs_integral_interval_arithmetic)

    // continuous-basic-test.cpp
    UNIT_TEST(rs_continuous_interval_basic_properties)
    UNIT_TEST(rs_continuous_interval_construction)
    UNIT_TEST(rs_continuous_interval_inverse)
    UNIT_TEST(rs_continuous_interval_binary_operations)
    UNIT_TEST(rs_continuous_interval_hashing)

    // continuous-set-test.cpp
    UNIT_TEST(rs_continuous_interval_set_construct_insert_erase)
    UNIT_TEST(rs_continuous_interval_set_operations)
    UNIT_TEST(rs_continuous_interval_set_hashing)

    // continuous-map-test.cpp
    UNIT_TEST(rs_continuous_interval_map)
    UNIT_TEST(rs_continuous_interval_map_hashing)

    // continuous-arithmetic-test.cpp
    UNIT_TEST(rs_continuous_interval_arithmetic)

    // unit-test.cpp

    return RS::UnitTest::end_tests();

}
