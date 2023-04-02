# Interval Class

_[Interval Library by Ross Smith](index.html)_

```c++
#include "rs-intervals/interval.hpp"
namespace RS::Intervals;
```

This header defines the basic `Interval` class at the core of the library.

## Contents

* TOC
{:toc}

## Constants

```c++
constexpr std::size_t npos = std::string::npos;
```

Defined for convenience.

## Supporting types

### Interval bounds

```c++
enum class Bound: std::uint8_t {
    empty,    // Interval is empty
    closed,   // Interval includes the bound
    open,     // Interval does not include the bound
    unbound,  // Interval is unbounded in this direction
};
```

Indicates what kind of bound the interval has. An `Interval` object contains
two of these values, one for each bound. Some combinations are impossible; if
one bound is `empty`, the other must also be `empty`.

### Value type categories

```c++
enum class Category: std::uint8_t {
    none,        // Not usable in an interval               (e.g. bool)
    continuous,  // Models a continuous arithmetic type     (e.g. float)
    integral,    // Supports integer arithmetic operations  (e.g. int)
    ordered,     // Ordered but not an arithmetic type      (e.g. std::string)
    stepwise,    // Incrementable and decrementable         (e.g. int*)
};
template <typename T> struct IntervalTraits {
    static constexpr Category category;
};
template <typename T> constexpr Category interval_category
    = IntervalTraits<T>::category;
```

Indicates the behaviour of a particular underlying type with respect to
intervals. To be usable as the template parameter to `Interval` and related
types, a type must at the very least be default constructible, copyable, and
totally ordered. Additional type properties enable additional interval
properties.

The `IntervalTraits` and `interval_category` templates determine the category
of a given type, according to the following table. The first matching row in
the table determines the type's interval category. If the default algorithm
does not give the appropriate category, the category for a user defined type
can be controlled by specializing `IntervalTraits` for that type.

| Condition                                                        | Category      |
| ---------                                                        | --------      |
| `IntervalTraits` is specialized                                  | From traits   |
| `T` is not default constructible, copyable, and totally ordered  | `none`        |
| `T` is `bool`                                                    | `none`        |
| `is_integral_v<T>` is true                                       | `integral`    |
| `is_floating_point_v<T>` is true                                 | `continuous`  |
| `numeric_limits<T>` is specialized and `is_integer` is true      | `integral`    |
| `numeric_limits<T>` is specialized and `is_integer` is false     | `continuous`  |
| `T` has unary `++ --` and binary `+ - * /` operators             | `integral`    |
| `T` has `+ - * /` but not `++ --`                                | `continuous`  |
| `T` has `++ --` but not `+ - * /`                                | `stepwise`    |
| Otherwise                                                        | `ordered`     |

The following concepts are defined to match the value type categories:

```c++
template <typename T> concept Continuous;
template <typename T> concept Integral;
template <typename T> concept Ordered;
template <typename T> concept Stepwise;
template <typename T> concept Arithmetic; // continuous or integral
template <typename T> concept IntervalCompatible; // any of the above
```

### Relationship between a value and an interval

```c++
enum class Match: std::int8_t {
    low = -1,  // Value is less than the lower bound
    ok,        // Value is an element of the interval
    high,      // Value is greater than the upper bound
    empty,     // Interval is empty
};
```

The result of the `Interval::match()` method, indicating the relationship of a
specific value to the interval.

### Relationship between two intervals

```c++
enum class Order: std::int8_t; // see below for the list of values
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

| Order                | Value  | Picture      | Description                                                      |
| -----                | -----  | -------      | -----------                                                      |
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

(The `equal` result includes the case where both intervals are empty.)

### Formatting helpers

```c++
template <typename F, typename T>
    concept Formatter = requires (const F& f, const T& t) {
        { f(t) } -> std::convertible_to<std::string>;
    };
```

Formatters can be passed to the string formatting functions of `Interval` and
related types, to format the interval bounds in a user defined way.

```c++
struct DefaultFormatter {
    template <typename T> std::string operator()(const T& t) const;
};
```

The default formatter that will be used if no custom formatter is supplied. It
uses the following simple rules:

* If `T` is an integer type, call `std::to_string()`.
* If `T` is explicitly or implicitly convertible to `std::string`, use that conversion.
* If a suitable output operator exists, format `T` using an `ostringstream`.
* Otherwise, `DefaultFormatter<T>` is not defined.

(Floating point types do not use `std::to_string()` because its output format
is poorly defined and likely to not be what the user expected.)

## Interval class

```c++
template <IntervalCompatible T> class Interval;
```

A class representing an interval over the underlying type `T`. This template
is only defined for underlying types for which the interval category is not
`none`.

The interval may be a finite interval with upper and lower bounds, bounded on
one side but unbounded on the other, an empty interval (containing no
values), or a universal interval (containing all values of the underlying
type). Depending on the properties of the value type, the interval class may
also distinguish between open, closed, and half-open intervals, indicating
whether or not the interval includes neither, both, or one of its bounds. For
integer-like types the open-closed distinction is not made, since an interval
specified using open bounds can be trivially turned into a closed interval by
incrementing or decrementing the bounds.

### Member types

```c++
using Interval::value_type = T;
```

The underlying domain type.

```c++
class Interval::iterator;
```

An iterator over all values in the interval. This is defined only for integral
or stepwise underlying types. This is a `const` iterator, and is random
access if the underlying type has addition and subtraction operators,
otherwise bidirectional.

### Member constants

```c++
static constexpr Category Interval::category = interval_category<T>;
```

The interval category of the underlying type.

### Life cycle functions

```c++
Interval::Interval();
Interval::Interval(const T& t);
Interval::Interval(const T& t, Bound l, Bound r);
Interval::Interval(const T& min, const T& max,
    Bound lr = Bound::closed);
Interval::Interval(const T& min, const T& max,
    Bound l, Bound r);
Interval::Interval(const T& min, const T& max, std::string_view mode);
```

Constructors. The default constructor creates an empty interval. The second
constructor creates an interval containing exactly one value, and provides an
implicit conversion from `T` to `Interval<T>`.

The next three constructors take the two boundary values and the two boundary
types; either can be reduced to a single value for both bounds. These will
automatically adjust the interval properties to produce a consistent result:
for integral types, open bounds will be turned into closed ones, and for all
types, the interval will be turned into an empty interval if the upper bound
is less than the lower bound (less than or equal if one or both is an open
bound). If one or both bounds is `empty` or `unbound`, the corresponding
`min` or `max` value is ignored. These constructors will throw
`std::invalid_argument` if one boundary type is `empty` but the other is
not.

The sixth constructor also takes explicit boundary value and boundary type
arguments, but the boundary types are specified using a string as shorthand:

| Mode    | Left bound  | Right bound  | Description                                                  |
| ----    | ----------  | -----------  | -----------                                                  |
| `"()"`  | `open`      | `open`       | Open interval                                                |
| `"(]"`  | `open`      | `closed`     | Half-open interval, closed on the right                      |
| `"[)"`  | `closed`    | `open`       | Half-open interval, closed on the left                       |
| `"[]"`  | `closed`    | `closed`     | Closed interval                                              |
| `"<"`   | `unbound`   | `open`       | Open interval, bounded above (first argument is ignored)     |
| `"<="`  | `unbound`   | `closed`     | Closed interval, bounded above (first argument is ignored)   |
| `">"`   | `open`      | `unbound`    | Open interval, bounded below (second argument is ignored)    |
| `">="`  | `closed`    | `unbound`    | Closed interval, bounded below (second argument is ignored)  |
| `"*"`   | `unbound`   | `unbound`    | Universal interval (first two arguments are ignored)         |

This constructor will throw `std::invalid_argument` if the mode string is not
one of these.

A universal interval (containing all possible values of `T`) can also be
constructed using the `all()` static member function.

```c++
Interval::Interval(const Interval& in);
Interval::Interval(Interval&& in) noexcept;
Interval::~Interval() noexcept;
Interval& Interval::operator=(const Interval& in);
Interval& Interval::operator=(Interval&& in) noexcept;
```

Other life cycle functions.

### Construction functions

```c++
static Interval Interval::all();
```

Constructs a universal interval, i.e. an unbounded interval containing all
possible values of `T`.

```c++
template <IntervalCompatible T>
    Interval<T> make_interval(const T& t);
template <IntervalCompatible T>
    Interval<T> make_interval(const T& t,
        Bound l, Bound r);
template <IntervalCompatible T>
    Interval<T> make_interval(const T& min, const T& max,
        Bound lr = Bound::closed);
template <IntervalCompatible T>
    Interval<T> make_interval(const T& min, const T& max,
        Bound l, Bound r);
template <IntervalCompatible T>
    Interval<T> make_interval(const T& min, const T& max,
        std::string_view mode);
template <IntervalCompatible T>
    Interval<T> ordered_interval(const T& a, const T& b,
        Bound lr = Bound::closed);
template <IntervalCompatible T>
    Interval<T> ordered_interval(const T& a, const T& b,
        Bound l, Bound r);
```

Interval construction functions. These have the same behaviour as the
corresponding constructors. The `ordered_interval()` functions do the same
thing as the corresponding `make_interval()` functions, except that, if the
bounds are in the wrong order, they will be swapped (along with the
corresponding bound flags).

### Type conversion functions

```c++
explicit Interval::operator bool() const noexcept;
```

Explicit conversion to `bool`; true if the interval is not empty.

```c++
static Interval Interval::from_string(const std::string& str);
```

Interprets a string as an interval. The value type `T` must have either a
constructor that takes a string or an input operator. This will not be
reliable for all value types because the characters that form part of the
interval syntax may also appear in the value type's string format.

Valid string formats are:

| Format      | Interpretation  | Format      | Interpretation  |
| ------      | --------------  | ------      | --------------  |
| `""`        | `{}`            | `"[A,B]"`   | `[A,B]`         |
| `"{}"`      | `{}`            | `"[A,B)"`   | `[A,B)`         |
| `"*"`       | `*`             | `"(A,B]"`   | `(A,B]`         |
| `"A"`       | `A`             | `"(A,B)"`   | `(A,B)`         |
| `">A"`      | `>A`            | `"A-B"`     | `[A,B]`         |
| `"<A"`      | `<A`            | `"A..B"`    | `[A,B]`         |
| `">=A"`     | `>=A`           | `"A...B"`   | `[A,B]`         |
| `"<=A"`     | `<=A`           | `"A..<B"`   | `[A,B)`         |
| `"A+"`      | `>=A`           | `"A<..B"`   | `(A,B]`         |
| `"A-"`      | `<=A`           | `"A<..<B"`  | `(A,B)`         |

### Comparison functions

```c++
Order Interval::order(const Interval& b) const;
```

Determines the relationship between two intervals. See the description of the
`Order` enumeration above for the interpretation of the result.

```c++
bool Interval::includes(const Interval& b) const;
bool Interval::overlaps(const Interval& b) const;
bool Interval::touches(const Interval& b) const;
```

Query specific relationships between two intervals. The `includes()` function
is true if `b` is a subset of `*this`; `overlaps()` is true if `*this` and
`b` have any values in common; `touches()` is true if there are no values
between the two intervals (this includes overlapping intervals).

```c++
std::strong_ordering
    operator<=>(const Interval& a, const Interval& b) noexcept;
bool operator==(const Interval& a, const Interval& b) noexcept;
bool operator!=(const Interval& a, const Interval& b) noexcept;
bool operator<(const Interval& a, const Interval& b) noexcept;
bool operator>(const Interval& a, const Interval& b) noexcept;
bool operator<=(const Interval& a, const Interval& b) noexcept;
bool operator>=(const Interval& a, const Interval& b) noexcept;
```

Lexicographical comparison operators. These call `T`'s comparison operators.
An empty interval compares less than any non-empty interval.

### Iterator functions

```c++
Interval::iterator Interval::begin() const;
Interval::iterator Interval::end() const;
```

Iterators over the values in an interval. These are defined only for integral
or stepwise underlying types. If either of the bounds does not exist
(bound type `empty` or `unbound`), behaviour is undefined if the
corresponding `begin()` or `end()` function is called.

### Query functions

```c++
bool Interval::contains(const T& t) const;
Match Interval::match(const T& t) const;
bool Interval::operator()(const T& t) const;
```

These determine the relationship between a specific value and an interval. The
`match()` function returns a value of the `Match` enumeration
indicating the precise relationship, while `contains()` simply indicates
whether the value is an element of the interval. The function call operator
is equivalent to `contains()`.

```c++
const T& Interval::min() const noexcept;
const T& Interval::max() const noexcept;
Bound Interval::left() const noexcept;
Bound Interval::right() const noexcept;
```

These return the boundary values and boundary types. If either end of the
interval is unbounded (boundary type `empty` or `unbound`), the value of the
corresponding `min()` or `max()` function is unspecified.

```c++
bool Interval::empty() const noexcept;
bool Interval::is_single() const noexcept;
bool Interval::is_finite() const noexcept;
bool Interval::is_infinite() const noexcept;
bool Interval::is_universal() const noexcept;
bool Interval::is_left_bounded() const noexcept;
bool Interval::is_left_closed() const noexcept;
bool Interval::is_left_open() const noexcept;
bool Interval::is_right_bounded() const noexcept;
bool Interval::is_right_closed() const noexcept;
bool Interval::is_right_open() const noexcept;
```

Interval properties.

| Property              | Description                                                     |
| --------              | -----------                                                     |
| `empty()`             | Interval is empty                                               |
| `is_single()`         | Interval contains exactly one value                             |
| `is_range()`          | Interval contains more than one value                           |
| `is_finite()`         | Interval is non-empty and bounded on both sides                 |
| `is_infinite()`       | Interval is non-empty and unbound on at least one side          |
| `is_universal()`      | Interval contains all values of the underlying type             |
| `is_left_bounded()`   | Interval is non-empty and bounded below                         |
| `is_left_closed()`    | Interval is left-bounded and includes its lower bound           |
| `is_left_open()`      | Interval is left-bounded and does not include its lower bound   |
| `is_right_bounded()`  | Interval is non-empty and bounded above                         |
| `is_right_closed()`   | Interval is right-bounded and includes its upper bound          |
| `is_right_open()`     | Interval is right-bounded and does not include its upper bound  |

```c++
[see below] Interval::size() const
```

Returns the length of the interval. For integral types, the return type is
`size_t`, and `size()` returns the number of values in the interval, or
`npos` if one or both bounds is `unbound`.

For continuous types, the return type is `T`, and `size()` returns the
difference between the upper and lower bounds, without regard to whether they
are open or closed bounds. If one or both of the bounds is `unbound`, `size()`
will return infinity if the type has a value for infinity; otherwise,
behaviour is undefined.

For ordered and stepwise types, the `size()` function is not defined.

```c++
std::string Interval::str() const;
template <Formatter<T> F> std::string Interval::str(const F& f) const;
std::ostream& operator<<(std::ostream& out, const Interval& in);
```

Format an interval as a string. The table below shows how intervals are
formatted (`A` and `B` represent formatted values of `T`).

| Format   | Description                                |
| ------   | -----------                                |
| `{}`     | Empty interval                             |
| `*`      | Universal interval (contains every value)  |
| `A`      | Single value                               |
| `(A,B)`  | Open interval                              |
| `(A,B]`  | Half-open interval, closed on the right    |
| `[A,B)`  | Half-open interval, closed on the left     |
| `[A,B]`  | Closed interval                            |
| `<A`     | Open interval, bounded above               |
| `<=A`    | Closed interval, bounded above             |
| `>A`     | Open interval, bounded below               |
| `>=A`    | Closed interval, bounded below             |

```c++
std::size_t Interval::hash() const noexcept;
struct std::hash<Interval>;
```

Hash function for an interval. Defined only if `std::hash<T>` is defined.

### Transformation functions

```c++
IntervalSet<T> Interval::complement() const;
IntervalSet<T> Interval::operator~() const;
```

Return the complementary interval (the set of all values that are not in the
interval). The complement may in general consist of more than one interval,
so the return type is `IntervalSet`, not `Interval`.

### Operations on two intervals

```c++
Interval Interval::envelope(const Interval& b) const;
```

Returns the smallest interval that contains both argument intervals
(`*this` and `b`). This is similar to `set_union()`, except that if the
intervals are both non-empty and are disjoint, this will also contain the
interval between them.

```c++
Interval Interval::set_intersection(const Interval& b) const;
IntervalSet<T> Interval::set_union(const Interval& b) const;
IntervalSet<T> Interval::set_difference(const Interval& b) const;
IntervalSet<T> Interval::set_symmetric_difference(const Interval& b) const;
```

Set operations on two intervals. In general the result of a set operation may
consist of more than one interval, so most of these return an `IntervalSet`
rather than an `Interval`.

```c++
void Interval::swap(Interval& in) noexcept;
void swap(Interval& a, Interval& b) noexcept;
```

Swap two intervals.

### Interval arithmetic operators

```c++
// Unary arithmetic operators
Interval operator+(const Interval& i);
Interval operator-(const Interval& i);
// Addition operators
Interval operator+(const Interval& a, const Interval& b);
Interval operator+(const Interval& a, const T& b);
Interval operator+(const T& a, const Interval& b);
Interval& operator+=(Interval& a, const Interval& b);
Interval& operator+=(Interval& a, const T& b);
// Subtraction operators
Interval operator-(const Interval& a, const Interval& b);
Interval operator-(const Interval& a, const T& b);
Interval operator-(const T& a, const Interval& b);
Interval& operator-=(Interval& a, const Interval& b);
Interval& operator-=(Interval& a, const T& b);
// Multiplication operators
Interval operator*(const Interval& a, const Interval& b);
Interval operator*(const Interval& a, const T& b);
Interval operator*(const T& a, const Interval& b);
Interval& operator*=(Interval& a, const Interval& b);
Interval& operator*=(Interval& a, const T& b);
// Division operators
IntervalSet operator/(const Interval& a, const Interval& b);
IntervalSet operator/(const Interval& a, const T& b);
IntervalSet operator/(const T& a, const Interval& b);

```

These return the interval containing all possible results of applying the
given operation to elements of the argument intervals. The result will always
be empty if any argument is empty. Most of these are defined only for
integral and continuous interval categories (division is a special case).

The unary plus operator simply returns its argument and is included only for
equivalence with the ordinary arithmetic operators.

The division operators are only defined for continuous intervals, because the
discontinuous nature of integer division in C++ makes it hard to define
integer interval division unambiguously.

The division operators return a set rather than an interval because division
may result in two disjoint intervals if the divisor includes values on both
sides of zero. The set will be empty if either argument is empty.

Division by zero is assumed never to happen (i.e. zero is excluded from the
divisor interval if it is present). The result will be an empty set if the
divisor contains only the single value zero.
