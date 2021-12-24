#include "rs-unit-test.hpp"

int main(int argc, char** argv) {

    RS::UnitTest::begin_tests(argc, argv);

    // types-test.cpp
    UNIT_TEST(rs_interval_type_traits)

    // continuous-boundary-basic-test.cpp
    UNIT_TEST(rs_continuous_boundary_formatting)
    UNIT_TEST(rs_continuous_boundary_from_interval)
    UNIT_TEST(rs_continuous_boundary_to_interval)

    // integral-boundary-basic-test.cpp
    UNIT_TEST(rs_integral_boundary_formatting)
    UNIT_TEST(rs_integral_boundary_from_interval)
    UNIT_TEST(rs_integral_boundary_to_interval)

    // ordered-boundary-basic-test.cpp
    UNIT_TEST(rs_ordered_boundary_formatting)
    UNIT_TEST(rs_ordered_boundary_from_interval)
    UNIT_TEST(rs_ordered_boundary_to_interval)

    // stepwise-boundary-basic-test.cpp
    UNIT_TEST(rs_stepwise_boundary_formatting)
    UNIT_TEST(rs_stepwise_boundary_from_interval)
    UNIT_TEST(rs_stepwise_boundary_to_interval)

    // continuous-boundary-comparison-test.cpp
    UNIT_TEST(rs_continuous_boundary_adjacency)
    UNIT_TEST(rs_continuous_boundary_comparison)

    // integral-boundary-comparison-test.cpp
    UNIT_TEST(rs_integral_boundary_adjacency)
    UNIT_TEST(rs_integral_boundary_comparison)

    // ordered-boundary-comparison-test.cpp
    UNIT_TEST(rs_ordered_boundary_adjacency)
    UNIT_TEST(rs_ordered_boundary_comparison)

    // stepwise-boundary-comparison-test.cpp
    UNIT_TEST(rs_stepwise_boundary_adjacency)
    UNIT_TEST(rs_stepwise_boundary_comparison)

    // continuous-boundary-addition-test.cpp
    UNIT_TEST(rs_continuous_boundary_inversion)
    UNIT_TEST(rs_continuous_boundary_addition)
    UNIT_TEST(rs_continuous_boundary_subtraction)

    // integral-boundary-addition-test.cpp
    UNIT_TEST(rs_integral_boundary_inversion)
    UNIT_TEST(rs_integral_boundary_addition)
    UNIT_TEST(rs_integral_boundary_subtraction)

    // continuous-boundary-multiplication-test.cpp
    UNIT_TEST(rs_continuous_boundary_multiplication)

    // integral-boundary-multiplication-test.cpp
    UNIT_TEST(rs_integral_boundary_multiplication)

    // continuous-basic-test.cpp
    UNIT_TEST(rs_continuous_interval_basic_properties)
    UNIT_TEST(rs_continuous_interval_construction)
    UNIT_TEST(rs_continuous_interval_order)
    UNIT_TEST(rs_continuous_interval_complement)
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
    UNIT_TEST(rs_continuous_contains_zero)
    UNIT_TEST(rs_continuous_interval_arithmetic)

    // integral-basic-test.cpp
    UNIT_TEST(rs_integral_interval_basic_properties)
    UNIT_TEST(rs_integral_interval_construction)
    UNIT_TEST(rs_integral_interval_iterators)
    UNIT_TEST(rs_integral_interval_order)
    UNIT_TEST(rs_integral_interval_complement)
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
    UNIT_TEST(rs_integral_contains_zero)
    UNIT_TEST(rs_integral_interval_arithmetic)

    // ordered-basic-test.cpp
    UNIT_TEST(rs_ordered_interval_basic_properties)
    UNIT_TEST(rs_ordered_interval_construction)
    UNIT_TEST(rs_ordered_interval_order)
    UNIT_TEST(rs_ordered_interval_complement)
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
    UNIT_TEST(rs_stepwise_interval_order)
    UNIT_TEST(rs_stepwise_interval_complement)
    UNIT_TEST(rs_stepwise_interval_binary_operations)
    UNIT_TEST(rs_stepwise_interval_hashing)

    // stepwise-set-test.cpp
    UNIT_TEST(rs_stepwise_interval_set_construct_insert_erase)
    UNIT_TEST(rs_stepwise_interval_set_operations)
    UNIT_TEST(rs_stepwise_interval_set_hashing)

    // stepwise-map-test.cpp
    UNIT_TEST(rs_stepwise_interval_map)
    UNIT_TEST(rs_stepwise_interval_map_hashing)

    // unit-test.cpp

    return RS::UnitTest::end_tests();

}
