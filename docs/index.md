# Intervals and Interval Based Containers

By Ross Smith

```c++
#include "rs-intervals/interval.hpp"
namespace RS::Intervals;
```

## Contents

* TOC
{:toc}

## Supporting types

```c++
enum class IntervalBound
    IntervalBound::empty = 0    // interval is empty
    IntervalBound::closed = 1   // interval includes the bound
    IntervalBound::open = 2     // interval does not include the bound
    IntervalBound::unbound = 3  // interval is unbounded in this direction
std::ostream& operator<<(std::ostream& out, IntervalBound ib)
```

Indicates what kind of bound the interval has. An `Interval` object contains
two of these values, one for each bound. Some combinations are impossible; if
one bound is `empty`, the other must also be `empty`.

```c++
enum class IntervalCategory;
    // see below for the list of values
std::ostream& operator<<(std::ostream& out, IntervalCategory ic);
template <typename T> struct IntervalTraits
    static constexpr IntervalCategory IntervalTraits::category;
template <typename T> constexpr IntervalCategory interval_category
    = IntervalTraits<T>::category;
```

Indicates the behaviour of a particular underlying type with respect to
intervals. To be usable as the template parameter to `Interval` and related
types, a type must at the very least be default constructible, copyable, and
totally ordered (defining all six comparison operators). Additional type
properties enable additional interval properties.

IntervalCategory  | Value  | Description                          | Example
----------------  | -----  | -----------                          | -------
`none`            | 0      | Not usable in an interval            | `bool`
`ordered`         | 1      | Ordered but not an arithmetic type   | `std::string`
`integral`        | 2      | Incrementable and not continuous     | `int`
`continuous`      | 3      | Models a continuous arithmetic type  | `float`

The `IntervalTraits` or `interval_category` templates determine the category
of a given type, according to the following algorithm:

* _if `T` is `bool`_
    * _category is `none`_
* _else if `T` is not default constructible, copyable, and totally ordered_
    * _category is `none`_
* _else if `numeric_limits<T>` is defined and `is_integer` is true_
    * _category is `integral`_
* _else if `numeric_limits<T>` is defined and `is_integer` is false_
    * _category is `continuous`_
* _else if `T` has binary `+ - * /` operators but not `%`_
    * _category is `continuous`_
* _else if `T` has unary `++` and `--` operators_
    * _category is `integral`_
* _else_
    * _category is `ordered`_

The interval category for a user defined type can be controlled by
specializing `IntervalCategory` for that type, if the default algorithm does
not give the appropriate category.

```c++
enum class IntervalMatch;
    IntervalMatch::low = -1;   // value is less than the lower bound
    IntervalMatch::ok = 0;     // value is an element of the interval
    IntervalMatch::high = 1;   // value is greater than the upper bound
    IntervalMatch::empty = 2;  // interval is empty
std::ostream& operator<<(std::ostream& out, IntervalMatch im);
```

The result of the `Interval::match()` method, indicating the relationship of a
specific value to the interval.

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

* `A` = left hand operand
* `B` = right hand operand
* `*` = included in both intervals
* `.` = gap between the two intervals

IntervalOrder        | Value  | Picture      | Description
-------------        | -----  | -------      | -----------
`b_only`             | -7     | `BBB`        | `A` is empty, `B` is not
`a_below_b`          | -6     | `AAA...BBB`  | Upper bound of `A` is less than lower bound of `B`, with a gap
`a_touches_b`        | -5     | `AAABBB`     | Upper bound of `A` is less than lower bound of `B`, with no gap
`a_overlaps_b`       | -4     | `AAA***BBB`  | Upper bound of `A` overlaps lower bound of `B`
`a_extends_below_b`  | -3     | `AAA***`     | `B` is a subset of `A`, with the same upper bound
`a_encloses_b`       | -2     | `AAA***AAA`  | `B` is a subset of `A`, matching neither bound
`b_extends_above_a`  | -1     | `***BBB`     | `A` is a subset of `B`, with the same lower bound
`equal`              | 0      | `***`        | `A` and `B` are the same (this includes the case where both are empty)
`a_extends_above_b`  | 1      | `***AAA`     | `B` is a subset of `A`, with the same lower bound
`b_encloses_a`       | 2      | `BBB***BBB`  | `A` is a subset of `B`, matching neither bound
`b_extends_below_a`  | 3      | `BBB***`     | `A` is a subset of `B`, with the same upper bound
`b_overlaps_a`       | 4      | `BBB***AAA`  | Upper bound of `B` overlaps lower bound of `A`
`b_touches_a`        | 5      | `BBBAAA`     | Upper bound of `B` is less than lower bound of `A`, with no gap
`b_below_a`          | 6      | `BBB...AAA`  | Upper bound of `B` is less than lower bound of `A`, with a gap
`a_only`             | 7      | `AAA`        | `B` is empty, `A` is not

## Interval class

```c++
template <typename T> class Interval;
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

```c++
static constexpr IntervalCategory Interval::category = interval_category<T>;
```

The interval category of the underlying type.

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

Mode    | Left bound  | Right bound  | Description
----    | ----------  | -----------  | -----------
`"()"`  | `open`      | `open`       | Open interval
`"(]"`  | `open`      | `closed`     | Half-open interval, closed on the right
`"[)"`  | `closed`    | `open`       | Half-open interval, closed on the left
`"[]"`  | `closed`    | `closed`     | Closed interval
`"<"`   | `unbound`   | `open`       | Open interval, bounded above (first argument is ignored)
`"<="`  | `unbound`   | `closed`     | Closed interval, bounded above (first argument is ignored)
`">"`   | `open`      | `unbound`    | Open interval, bounded below (second argument is ignored)
`">="`  | `closed`    | `unbound`    | Closed interval, bounded below (second argument is ignored)
`"*"`   | `unbound`   | `unbound`    | Universal interval (first two arguments are ignored)

This constructor will throw `std::invalid_argument` if the mode string is not
one of these.

A universal interval (containing all possible values of `T`) can also be
constructed using the `all()` static member function (see below).

```c++
Interval::Interval(const Interval& in);
Interval::Interval(Interval&& in) noexcept;
Interval::~Interval() noexcept;
Interval& Interval::operator=(const Interval& in);
Interval& Interval::operator=(Interval&& in) noexcept;
```

Other life cycle functions.

```c++
explicit Interval::operator bool() const noexcept;
```

Explicit conversion to `bool`; true if the interval is not empty.

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

Property              | Description
--------              | -----------
`empty()`             | True if the interval is empty
`is_single()`         | True if the interval contains exactly one value
`is_finite()`         | True if the interval is non-empty and bounded on both sides
`is_infinite()`       | True if the interval is non-empty and unbound on at least one side
`is_universal()`      | True if the interval contains all values of the underlying type
`is_left_bounded()`   | True if the interval is non-empty and bounded below
`is_left_closed()`    | True if the interval is left-bounded and includes its lower bound
`is_left_open()`      | True if the interval is left-bounded and does not include its lower bound
`is_right_bounded()`  | True if the interval is non-empty and bounded above
`is_right_closed()`   | True if the interval is right-bounded and includes its upper bound
`is_right_open()`     | True if the interval is right-bounded and does not include its upper bound

```c++
Interval::iterator Interval::begin() const;
Interval::iterator Interval::end() const;
```

Iterators over the values in an interval. These are defined only for
integer-like underlying types (interval category `integral`). If either of
the bounds does not exist (bound type `empty` or `unbound`), behaviour is
undefined if the corresponding `begin()` or `end()` function is called.

```c++
[see below] Interval::size() const
```

Returns the length of the interval. For `integral` types, the return type is
`size_t`, and `size()` returns the number of values in the interval, or
`npos` if one or both bounds is `unbound`.

For `continuous` types, the return type is `T`, and `size()` returns the
difference between the upper and lower bounds, without regard to whether they
are open or closed bounds. Behaviour is undefined if one or both of the
bounds is `unbound`.

For `ordered` types, the `size()` function is not defined.

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
IntervalSet<T> Interval::inverse() const;
```

Returns the inverse interval (the set of all values that are not in the
interval). The inverse may in general consist of more than one interval, so
the return type is `IntervalSet`, not `Interval`.

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
std::string Interval::str(char mode = 0, int prec = -1) const;
std::ostream& operator<<(std::ostream& out, const Interval& in);
```

Format an interval as a string. The table below shows how intervals are
formatted (`A` and `B` represent formatted values of `T`).

Format   Description
------   -----------
`{}`     Empty interval
`*`      Universal interval (contains every value)
`A`      Single value
`(A,B)`  Open interval
`(A,B]`  Half-open interval, closed on the right
`[A,B)`  Half-open interval, closed on the left
`[A,B]`  Closed interval
`<A`     Open interval, bounded above
`<=A`    Closed interval, bounded above
`>A`     Open interval, bounded below
`>=A`    Closed interval, bounded below

The `mode` and `prec` arguments will only be used if the value type is a
floating point arithmetic type; `mode` must be one of `[EeFfGg]`, and `prec`
has its usual meaning. Other types are formatted according to the following
rules:

* _if `T` is `char`_
    * _format as a literal character_
* _else if `T` is an integer arithmetic type_
    * _format as a decimal integer_
* _else if `T` is a floating point arithmetic type_
    * _format as described above_
* _else if `T` is implicitly or explicitly convertible to `std::string`_
    * _convert directly to a string_
* _else if a `to_string(T)` or `std::to_string(T)` function exists_
    * _format by calling that function_
* _else if `T` has an output operator_
    * _format using `std::ostringstream`_
* _else_
    * _fail to compile_

```c++
size_t Interval::hash() const noexcept;
class std::hash<Interval>;
```

Hash function for an interval. This relies on the existence of a hash function
for `T`.

```c++
void Interval::swap(Interval& in) noexcept;
void swap(Interval& a, Interval& b);
```

Swap two intervals.

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
bool operator==(const Interval& a, const Interval& b) noexcept;
bool operator!=(const Interval& a, const Interval& b) noexcept;
bool operator<(const Interval& a, const Interval& b) noexcept;
bool operator>(const Interval& a, const Interval& b) noexcept;
bool operator<=(const Interval& a, const Interval& b) noexcept;
bool operator>=(const Interval& a, const Interval& b) noexcept;
```

Lexicographical comparison operators. These call `T`'s equality and less-than
operators. An empty interval compares less than any non-empty interval.

## Interval set

```c++
template <typename T> class IntervalSet;
```

This class represents a subset of `T`'s domain as a set of intervals.

```c++
using IntervalSet::value_type = T;
using IntervalSet::interval_type = Interval<T>;
class IntervalSet::iterator; // Bidirectional const iterator

Member types.

```c++
static constexpr IntervalCategory IntervalSet::category
    = interval_category<T>;
```

The underlying value type's interval category.

```c++
IntervalSet::IntervalSet();
IntervalSet::IntervalSet(const T& t);
IntervalSet::IntervalSet(const Interval& in);
IntervalSet::IntervalSet(std::initializer_list<Interval> list);
IntervalSet::IntervalSet(const IntervalSet& set);
IntervalSet::IntervalSet(IntervalSet&& set) noexcept;
IntervalSet::~IntervalSet() noexcept;
IntervalSet& IntervalSet::operator=(const IntervalSet& set);
IntervalSet& IntervalSet::operator=(IntervalSet&& set) noexcept;
```

Life cycle functions. When a set is constructed from a list of intervals or
values, the intervals are ordered lexicographically, and adjacent intervals
are merged when they touch or overlap.

```c++
IntervalSet::iterator IntervalSet::begin() const noexcept;
IntervalSet::iterator IntervalSet::end() const noexcept;
```

Iterators over the intervals in the set.

```c++
bool IntervalSet::empty() const noexcept;
```

True if the set is empty.

```c++
size_t IntervalSet::size() const noexcept;
```

Returns the number of intervals in the set.

```c++
bool IntervalSet::contains(const T& t) const;
bool IntervalSet::operator()(const T& t) const;
```

True if the value is an element of any of the intervals in the set.

```c++
void IntervalSet::clear() noexcept;
```

Clears all intervals from the set, leaving it empty.

```c++
void IntervalSet::insert(const Interval& in);
```

Adds a new interval to the set. Adjacent intervals are merged when they touch
or overlap.

```c++
void IntervalSet::erase(const Interval& in);
```

Removes an interval from the set. Intervals in the set that overlap this
interval will be modified or removed as necessary. This will have no effect if
this interval does not overlap any existing interval in the set.

```c++
IntervalSet IntervalSet::inverse() const;
IntervalSet IntervalSet::operator~() const;
```

Returns the complement of the set, i.e. a new set whose member intervals
contain every value of `T` that is not in this set.

```c++
// Set intersection
    IntervalSet IntervalSet::set_intersection(const IntervalSet& b) const;
    IntervalSet& IntervalSet::operator&=(const IntervalSet& b);
    IntervalSet operator&(const IntervalSet& a, const IntervalSet& b);
    IntervalSet operator&(const IntervalSet& a, const Interval& b);
    IntervalSet operator&(const Interval& a, const IntervalSet& b);
    IntervalSet operator&(const IntervalSet& a, const T& b);
    IntervalSet operator&(const T& a, const IntervalSet& b);
// Set union
    IntervalSet IntervalSet::set_union(const IntervalSet& b) const;
    IntervalSet& IntervalSet::operator|=(const IntervalSet& b);
    IntervalSet operator|(const IntervalSet& a, const IntervalSet& b);
    IntervalSet operator|(const IntervalSet& a, const Interval& b);
    IntervalSet operator|(const Interval& a, const IntervalSet& b);
    IntervalSet operator|(const IntervalSet& a, const T& b);
    IntervalSet operator|(const T& a, const IntervalSet& b);
// Set difference
    IntervalSet IntervalSet::set_difference(const IntervalSet& b) const;
    IntervalSet& IntervalSet::operator-=(const IntervalSet& b);
    IntervalSet operator-(const IntervalSet& a, const IntervalSet& b);
    IntervalSet operator-(const IntervalSet& a, const Interval& b);
    IntervalSet operator-(const Interval& a, const IntervalSet& b);
    IntervalSet operator-(const IntervalSet& a, const T& b);
    IntervalSet operator-(const T& a, const IntervalSet& b);
// Set symmetric difference
    IntervalSet IntervalSet::set_symmetric_difference(const IntervalSet& b) const;
    IntervalSet& IntervalSet::operator^=(const IntervalSet& b);
    IntervalSet operator^(const IntervalSet& a, const IntervalSet& b);
    IntervalSet operator^(const IntervalSet& a, const Interval& b);
    IntervalSet operator^(const Interval& a, const IntervalSet& b);
    IntervalSet operator^(const IntervalSet& a, const T& b);
    IntervalSet operator^(const T& a, const IntervalSet& b);
```

Set operations.

```c++
std::string IntervalSet::str(char mode = 0, int prec = -1) const;
std::ostream& operator<<(std::ostream& out, const IntervalSet& set);
```

Format an interval set as a string. The format is `"{A,B,C...}"`, where `A`,
`B`, `C`, etc are intervals or values of `T`, formatted as described for
`Interval::str()` above.

```c++
size_t IntervalSet::hash() const noexcept;
class std::hash<IntervalSet>;
```

Hash function for an interval set.

```c++
void IntervalSet::swap(IntervalSet& set) noexcept;
void swap(IntervalSet& a, IntervalSet& b);
```

Swap two interval sets.

```c++
bool operator==(const IntervalSet& a, const IntervalSet& b) noexcept;
bool operator!=(const IntervalSet& a, const IntervalSet& b) noexcept;
bool operator<(const IntervalSet& a, const IntervalSet& b) noexcept;
bool operator>(const IntervalSet& a, const IntervalSet& b) noexcept;
bool operator<=(const IntervalSet& a, const IntervalSet& b) noexcept;
bool operator>=(const IntervalSet& a, const IntervalSet& b) noexcept;
```

Lexicographical comparison operators. These call `T`'s equality and less-than
operators.

## Interval map

```c++
template <typename K, typename T> class IntervalMap;
```

A map from a set of intervals over `K` to values of `T`. The `IntervalMap`
object also contains a default value of `T` that will be returned when a key
is not a member of any of the intervals in the map.

```c++
using IntervalMap::key_type = K;
using IntervalMap::interval_type = Interval<K>;
using IntervalMap::mapped_type = T;
using IntervalMap::value_type = std::pair<const Interval<K>, T>;
class IntervalMap::iterator; // Bidirectional const iterator

Member types.

```c++
static constexpr IntervalCategory IntervalMap::category
    = interval_category<K>;
```

The underlying type's interval category.

```c++
IntervalMap::IntervalMap();
explicit IntervalMap::IntervalMap(const T& defval);
IntervalMap::IntervalMap(std::initializer_list<value_type> list);
IntervalMap::IntervalMap(const IntervalMap& map);
IntervalMap::IntervalMap(IntervalMap&& map) noexcept;
IntervalMap::~IntervalMap() noexcept;
IntervalMap& IntervalMap::operator=(const IntervalMap& map);
IntervalMap& IntervalMap::operator=(IntervalMap&& map) noexcept;
```

Life cycle functions. An optional default value can be provided; if none is
provided, `T`'s default constructor is used. When a map is constructed from a
list of `(interval,value)` pairs, the intervals are ordered
lexicographically, and adjacent intervals are merged when they touch or
overlap and have the same mapped value. When two intervals overlap but do not
have the same mapped value, later entries in the initializer list will
overwrite earlier ones.

```c++
const T& IntervalMap::operator[](const K& key) const;
```

Returns the mapped value corresponding to the interval containing the given
key, or the default value if no interval contains the key.

```c++
IntervalMap::iterator IntervalMap::begin() const noexcept;
IntervalMap::iterator IntervalMap::end() const noexcept;
```

Iterators over the map's list of `(interval,value)` pairs.

```c++
bool IntervalMap::empty() const noexcept;
```

True if the map is empty.

```c++
size_t IntervalMap::size() const noexcept;
```

Returns the number of intervals in the map.

```c++
const T& IntervalMap::default_value() const noexcept;
void IntervalMap::default_value(const T& defval);
```

Query or set the default value.

```c++
bool IntervalMap::contains(const K& key) const;
```

True if one of the intervals in the map contains the key.

```c++
IntervalMap::iterator IntervalMap::find(const K& key) const;
```

Returns an iterator pointing to the interval containing the given key, or
`end()` if no such interval exists.

```c++
IntervalMap::iterator IntervalMap::lower_bound(const K& key) const;
IntervalMap::iterator IntervalMap::upper_bound(const K& key) const;
```

If the key is contained in one of the intervals in the map, `lower_bound()`
returns an iterator pointing to that interval, and `upper_bound()` returns the
next iterator. If not, both functions return the iterator pointing to the
first interval after the given key, or `end()` if no such interval exists.

```c++
void IntervalMap::clear() noexcept;
void IntervalMap::reset(const T& defval = T());
```

These clear all `(interval,value)` pairs from the map. The `clear()` function
does not change the default value, while `reset()` also changes it.

```c++
void IntervalMap::insert(const Interval<K>& in, const T& t);
void IntervalMap::insert(const value_type& v);
```

Adds a new `(interval,value)` pair to the map. Intervals in the map that
overlap this interval will be modified or removed as necessary.

```c++
void IntervalMap::erase(const Interval<K>& in);
```

Removes an interval from the map. Intervals in the map that overlap this
interval will be modified or removed as necessary. This will have no effect if
this interval does not overlap any existing interval in the map.

```c++
std::string IntervalMap::str() const;
std::string IntervalMap::str(char kmode, int kprec,
    char vmode, int vprec) const;
std::ostream& operator<<(std::ostream& out, const IntervalMap& set);
```

Format an interval set as a string. The format is `"{A:X,B:Y,C:Z,...}"`, where
`A`, `B`, `C`, etc are intervals or values of `K` (formatted as described for
`Interval::str()` above), and `X`, `Y`, `Z`, etc are values of `T`. The two
sets of formatting parameters are used for the keys and values respectively.
The default value is not reported.

```c++
size_t IntervalMap::hash() const noexcept;
class std::hash<IntervalMap>;
```

Hash function for an interval map.

```c++
void IntervalMap::swap(IntervalMap& map) noexcept;
void swap(IntervalMap& a, IntervalMap& b);
```

Swap two interval maps.

```c++
bool operator==(const IntervalMap& a, const IntervalMap& b) noexcept;
bool operator!=(const IntervalMap& a, const IntervalMap& b) noexcept;
bool operator<(const IntervalMap& a, const IntervalMap& b) noexcept;
bool operator>(const IntervalMap& a, const IntervalMap& b) noexcept;
bool operator<=(const IntervalMap& a, const IntervalMap& b) noexcept;
bool operator>=(const IntervalMap& a, const IntervalMap& b) noexcept;
```

Lexicographical comparison operators. These call `T`'s equality and less-than
operators.
