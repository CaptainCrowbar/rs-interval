# Interval Class

_[Intervals Library by Ross Smith](index.html)_

```c++
#include "rs-interval/interval.hpp"
namespace RS::Intervals;
```

This header defines the basic `Interval` class at the core of the library.

## Contents

* TOC
{:toc}

## Interval class

```c++
template <typename T> class Interval;
```

A class representing an interval over the underlying type `T`. This template
is only defined for underlying types for which the interval category is not
`none`.

To be usable as the template parameter to `Interval` and related types, a type
must at the very least be default constructible, copyable, and totally
ordered (defining all six comparison operators).

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

An iterator over all values in the interval. This is defined only for
integer-like underlying types (i.e. types where the `interval_category` is
`integral`). This is a `const` iterator, and is random access if the
underlying type has addition and subtraction operators, otherwise
bidirectional.

### Member constants

```c++
static constexpr IntervalCategory Interval::category = interval_category<T>;
```

The interval category of the underlying type.

### Life cycle functions

```c++
Interval::Interval();
Interval::Interval(const T& t);
Interval::Interval(const T& t, IntervalBound l, IntervalBound r);
Interval::Interval(const T& min, const T& max,
    IntervalBound lr = IntervalBound::closed);
Interval::Interval(const T& min, const T& max,
    IntervalBound l, IntervalBound r);
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
static Interval Interval::all() noexcept;
```

Constructs a universal interval, i.e. an unbounded interval containing all
possible values of `T`.

```c++
template <typename T> Interval<T> make_interval(const T& t);
template <typename T> Interval<T> make_interval(const T& t,
    IntervalBound l, IntervalBound r);
template <typename T> Interval<T> make_interval(const T& min,
    const T& max, IntervalBound lr = IntervalBound::closed);
template <typename T> Interval<T> make_interval(const T& min,
    const T& max, IntervalBound l, IntervalBound r);
template <typename T> Interval<T> make_interval(const T& min,
    const T& max, std::string_view mode);
template <typename T> Interval<T> ordered_interval(T a, T b,
    IntervalBound lr = IntervalBound::closed);
template <typename T> Interval<T> ordered_interval(T a, T b,
    IntervalBound l, IntervalBound r);
```

Interval construction functions. These have the same behaviour as the
corresponding constructors. The `ordered_interval()` functions do the same
thing as the corresponding `make_interval()` functions, except that, if the
bounds are in the wrong order, they will be swapped (along with the
corresponding bound flags).

```c++
Interval::Interval(const Interval& in);
Interval::Interval(Interval&& in) noexcept;
Interval::~Interval() noexcept;
Interval& Interval::operator=(const Interval& in);
Interval& Interval::operator=(Interval&& in) noexcept;
```

Other life cycle functions.

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
IntervalOrder Interval::order(const Interval& b) const;
```

Determines the relationship between two intervals. See the description of the
`IntervalOrder` enumeration above for the interpretation of the result.

```c++
int Interval::compare(const Interval& b) const noexcept;
```

Performs a three-way lexicographical ordering of two intervals, according to
`T`'s less-than operator. The return value is a positive integer if
`*this>b`, a negative integer if `*this<b`, or zero if `*this==b`. An empty
interval compares less than any non-empty interval.

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
bool operator==(const Interval& a, const Interval& b) noexcept;
bool operator!=(const Interval& a, const Interval& b) noexcept;
bool operator<(const Interval& a, const Interval& b) noexcept;
bool operator>(const Interval& a, const Interval& b) noexcept;
bool operator<=(const Interval& a, const Interval& b) noexcept;
bool operator>=(const Interval& a, const Interval& b) noexcept;
```

Lexicographical comparison operators. These call `T`'s equality and less-than
operators. An empty interval compares less than any non-empty interval.

### Iterator functions

```c++
Interval::iterator Interval::begin() const;
Interval::iterator Interval::end() const;
```

Iterators over the values in an interval. These are defined only for
integer-like underlying types (interval category `integral`). If either of
the bounds does not exist (bound type `empty` or `unbound`), behaviour is
undefined if the corresponding `begin()` or `end()` function is called.

### Query functions

```c++
bool Interval::contains(const T& t) const;
IntervalMatch Interval::match(const T& t) const;
bool Interval::operator()(const T& t) const;
```

These determine the relationship between a specific value and an interval. The
`match()` function returns a value of the `IntervalMatch` enumeration
indicating the precise relationship, while `contains()` simply indicates
whether the value is an element of the interval. The function call operator
is equivalent to `contains()`.

```c++
const T& Interval::min() const noexcept;
const T& Interval::max() const noexcept;
IntervalBound Interval::left() const noexcept;
IntervalBound Interval::right() const noexcept;
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

| Property              | Description                                                                 |
| --------              | -----------                                                                 |
| `empty()`             | True if the interval is empty                                               |
| `is_single()`         | True if the interval contains exactly one value                             |
| `is_finite()`         | True if the interval is non-empty and bounded on both sides                 |
| `is_infinite()`       | True if the interval is non-empty and unbound on at least one side          |
| `is_universal()`      | True if the interval contains all values of the underlying type             |
| `is_left_bounded()`   | True if the interval is non-empty and bounded below                         |
| `is_left_closed()`    | True if the interval is left-bounded and includes its lower bound           |
| `is_left_open()`      | True if the interval is left-bounded and does not include its lower bound   |
| `is_right_bounded()`  | True if the interval is non-empty and bounded above                         |
| `is_right_closed()`   | True if the interval is right-bounded and includes its upper bound          |
| `is_right_open()`     | True if the interval is right-bounded and does not include its upper bound  |

```c++
[see below] Interval::size() const
```

Returns the length of the interval. For integral types, the return type is
`size_t`, and `size()` returns the number of values in the interval, or
`npos` if one or both bounds is `unbound`.

For continuous types, the return type is `T`, and `size()` returns the
difference between the upper and lower bounds, without regard to whether they
are open or closed bounds. Behaviour is undefined if one or both of the
bounds is `unbound`.

For ordered and stepwise types, the `size()` function is not defined.

```c++
std::string Interval::str(const FormatSpec& spec = {}) const;
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
size_t Interval::hash() const noexcept;
class std::hash<Interval>;
```

Hash function for an interval. This relies on the existence of a hash function
for `T`.

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
void swap(Interval& a, Interval& b);
```

Swap two intervals.

### Interval arithmetic operators

```c++
// Unary arithmetic operators
Interval<T> operator+(const Interval<T>& i);
Interval<T> operator-(const Interval<T>& i);
// Addition operators
Interval<T> operator+(const Interval<T>& a, const Interval<T>& b);
Interval<T> operator+(const Interval<T>& a, const T& b);
Interval<T> operator+(const T& a, const Interval<T>& b);
Interval<T>& operator+=(Interval<T>& a, const Interval<T>& b);
Interval<T>& operator+=(Interval<T>& a, const T& b);
// Subtraction operators
Interval<T> operator-(const Interval<T>& a, const Interval<T>& b);
Interval<T> operator-(const Interval<T>& a, const T& b);
Interval<T> operator-(const T& a, const Interval<T>& b);
Interval<T>& operator-=(Interval<T>& a, const Interval<T>& b);
Interval<T>& operator-=(Interval<T>& a, const T& b);
// Multiplication operators
Interval<T> operator*(const Interval<T>& a, const Interval<T>& b);
Interval<T> operator*(const Interval<T>& a, const T& b);
Interval<T> operator*(const T& a, const Interval<T>& b);
Interval<T>& operator*=(Interval<T>& a, const Interval<T>& b);
Interval<T>& operator*=(Interval<T>& a, const T& b);
// Division operators
IntervalSet<T> operator/(const Interval<T>& a, const Interval<T>& b);
IntervalSet<T> operator/(const Interval<T>& a, const T& b);
IntervalSet<T> operator/(const T& a, const Interval<T>& b);

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
