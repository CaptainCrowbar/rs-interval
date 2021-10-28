#include "test/unit-test.hpp"

int main(int argc, char** argv) {

    RS::UnitTest::begin_tests(argc, argv);

    // interval-basic-test.cpp
    UNIT_TEST(rs_interval_basic_type_traits)
    UNIT_TEST(rs_interval_format_floating_point)

    // interval-continuous-test.cpp
    UNIT_TEST(rs_interval_continuous_basic_properties)
    UNIT_TEST(rs_interval_continuous_construction)
    UNIT_TEST(rs_interval_continuous_inverse)
    UNIT_TEST(rs_interval_continuous_binary_operations)

    // interval-continuous-arithmetic-test.cpp
    UNIT_TEST(rs_interval_continuous_arithmetic)

    // interval-continuous-set-test.cpp
    UNIT_TEST(rs_interval_continuous_set_construct_insert_erase)
    UNIT_TEST(rs_interval_continuous_set_operations)

    // interval-continuous-map-test.cpp
    UNIT_TEST(rs_interval_continuous_map)

    // interval-integral-test.cpp
    UNIT_TEST(rs_interval_integral_basic_properties)
    UNIT_TEST(rs_interval_integral_construction)
    UNIT_TEST(rs_interval_integral_iterators)
    UNIT_TEST(rs_interval_integral_inverse)
    UNIT_TEST(rs_interval_integral_binary_operations)

    // interval-integral-arithmetic-test.cpp
    UNIT_TEST(rs_interval_integral_arithmetic)

    // interval-integral-set-test.cpp
    UNIT_TEST(rs_interval_integral_set_construct_insert_erase)
    UNIT_TEST(rs_interval_integral_set_operations)

    // interval-integral-map-test.cpp
    UNIT_TEST(rs_interval_integral_map)

    // interval-ordered-test.cpp
    UNIT_TEST(rs_interval_ordered_basic_properties)
    UNIT_TEST(rs_interval_ordered_construction)
    UNIT_TEST(rs_interval_ordered_inverse)
    UNIT_TEST(rs_interval_ordered_binary_operations)

    // interval-ordered-set-test.cpp
    UNIT_TEST(rs_interval_ordered_set_construct_insert_erase)
    UNIT_TEST(rs_interval_ordered_set_operations)

    // interval-ordered-map-test.cpp
    UNIT_TEST(rs_interval_ordered_map)

    return RS::UnitTest::end_tests();

}
