# Supporting Types

_[Intervals Library by Ross Smith](index.html)_

```c++
#include "rs-interval/types.hpp"
namespace RS::Intervals;
```

This header defines a number of supporting types used elsewhere in the
library. Normally there is no reason to include this header directly; it will
be included by anything that uses any of these types.

## Contents

* TOC
{:toc}

## Imported types

```c++
using RS::Format::FormatSpec;
```

Imported for convenience.

## Enumeration types

### Interval bounds

```c++
enum class IntervalBound;
    IntervalBound::empty = 0;    // Interval is empty
    IntervalBound::closed = 1;   // Interval includes the bound
    IntervalBound::open = 2;     // Interval does not include the bound
    IntervalBound::unbound = 3;  // Interval is unbounded in this direction
std::ostream& operator<<(std::ostream& out, IntervalBound ib);
```

Indicates what kind of bound the interval has. An `Interval` object contains
two of these values, one for each bound. Some combinations are impossible; if
one bound is `empty`, the other must also be `empty`.

### Value type categories

```c++
enum class IntervalCategory;
    // see below for the list of values
std::ostream& operator<<(std::ostream& out, IntervalCategory ic);
template <typename T> struct IntervalTraits;
    static constexpr IntervalCategory IntervalTraits::category;
template <typename T> constexpr IntervalCategory interval_category
    = IntervalTraits<T>::category;
```

Indicates the behaviour of a particular underlying type with respect to
intervals. To be usable as the template parameter to `Interval` and related
types, a type must at the very least be default constructible, copyable, and
totally ordered (defining all six comparison operators). Additional type
properties enable additional interval properties.

| IntervalCategory  | Description                          | Example        |
| ----------------  | -----------                          | -------        |
| `none`            | Not usable in an interval            | `bool`         |
| `ordered`         | Ordered but not an arithmetic type   | `std::string`  |
| `stepwise`        | Incrementable and decrementable      | `int*`         |
| `integral`        | Integer arithmetic operations        | `int`          |
| `continuous`      | Models a continuous arithmetic type  | `float`        |

The `IntervalTraits` or `interval_category` templates determine the category
of a given type, according to the following algorithm:

* _if `T` is `bool` or `T` is not default constructible, copyable, and totally ordered_
    * _category is `none`_
* _else if `is_floating_point_v<T>` is true, or `numeric_limits<T>` is defined and `is_integer` is false_
    * _category is `continuous`_
* _else if `is_integral_v<T>` is true, or `numeric_limits<T>` is defined and `is_integer` is true_
    * _category is `integral`_
* _else if `T` has unary `++ --` and binary `+ - * /`_
    * _category is `integral`_
* _else if `T` has `+ - * /` operators but not `++ --`_
    * _category is `continuous`_
* _else if `T` has `++ --` operators but not `+ - * /`_
    * _category is `stepwise`_
* _else_
    * _category is `ordered`_

The interval category for a user defined type can be controlled by
specializing `IntervalTraits` for that type, if the default algorithm does
not give the appropriate category.

### Relationship between a value and an interval

```c++
enum class IntervalMatch;
    IntervalMatch::low = -1;   // Value is less than the lower bound
    IntervalMatch::ok = 0;     // Value is an element of the interval
    IntervalMatch::high = 1;   // Value is greater than the upper bound
    IntervalMatch::empty = 2;  // Interval is empty
std::ostream& operator<<(std::ostream& out, IntervalMatch im);
```

The result of the `Interval::match()` method, indicating the relationship of a
specific value to the interval.

### Relationship between two intervals

```c++
enum class IntervalOrder;
    // see below for the list of values
std::ostream& operator<<(std::ostream& out, IntervalOrder io);
```

The result of the `Interval::order()` method, indicating the relationship of
one interval to another. In the table below, the values in the first column
are the possible results of calling `A.order(B)`, where `A` and `B` are
intervals. A negative value indicates that `A` will compare lexicographically
less than `B`; a positive value indicates that `A` will compare greater than
`B`.

In the "picture" layouts here:

* `AAA` = First interval
* `BBB` = Second interval
* `***` = Common to both intervals
* `...` = Gap between the two intervals

| IntervalOrder        | Value  | Picture      | Description                                                      |
| -------------        | -----  | -------      | -----------                                                      |
| `both_empty`         | -8     |              | Both intervals are empty                                         |
| `b_only`             | -7     | `BBB`        | `A` is empty, `B` is not                                         |
| `a_below_b`          | -6     | `AAA...BBB`  | Upper bound of `A` is less than lower bound of `B`, with a gap   |
| `a_touches_b`        | -5     | `AAABBB`     | Upper bound of `A` is less than lower bound of `B`, with no gap  |
| `a_overlaps_b`       | -4     | `AAA***BBB`  | Upper bound of `A` overlaps lower bound of `B`                   |
| `a_extends_below_b`  | -3     | `AAA***`     | `B` is a subset of `A`, with the same upper bound                |
| `a_encloses_b`       | -2     | `AAA***AAA`  | `B` is a subset of `A`, matching neither bound                   |
| `b_extends_above_a`  | -1     | `***BBB`     | `A` is a subset of `B`, with the same lower bound                |
| `equal`              | 0      | `***`        | `A` and `B` are the same                                         |
| `a_extends_above_b`  | 1      | `***AAA`     | `B` is a subset of `A`, with the same lower bound                |
| `b_encloses_a`       | 2      | `BBB***BBB`  | `A` is a subset of `B`, matching neither bound                   |
| `b_extends_below_a`  | 3      | `BBB***`     | `A` is a subset of `B`, with the same upper bound                |
| `b_overlaps_a`       | 4      | `BBB***AAA`  | Upper bound of `B` overlaps lower bound of `A`                   |
| `b_touches_a`        | 5      | `BBBAAA`     | Upper bound of `B` is less than lower bound of `A`, with no gap  |
| `b_below_a`          | 6      | `BBB...AAA`  | Upper bound of `B` is less than lower bound of `A`, with a gap   |
| `a_only`             | 7      | `AAA`        | `B` is empty, `A` is not                                         |
