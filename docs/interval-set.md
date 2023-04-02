# Interval Set Class

_[Interval Library by Ross Smith](index.html)_

```c++
#include "rs-intervals/interval-set.hpp"
namespace RS::Intervals;
```

This header defines a class representing a set of disjoint intervals.

## Contents

* TOC
{:toc}

## Interval set class

```c++
template <IntervalCompatible T> class IntervalSet;
```

This class represents a subset of `T`'s domain as a set of intervals. This is
an ordered set analogous to `std::set`. The intervals in the set are
automatically updated when intervals are inserted or erased: an inserted
interval will be merged with any existing intervals that it touches; an
interval will be removed, reduced in size, or split into two if part of it is
erased.

### Member types

```c++
using IntervalSet::interval_type = Interval<T>;
using IntervalSet::value_type = T;
```

Type aliases.

```c++
class IntervalSet::iterator;
```

A bidirectional `const` iterator over the intervals that make up the set,
dereferencing to an `Interval<T>`.

### Member constants

```c++
static constexpr Category IntervalSet::category
    = interval_category<T>;
```

The underlying value type's interval category.

### Life cycle functions

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

When a set is constructed from a list of intervals or values, the intervals
are ordered lexicographically, and adjacent intervals are merged when they
touch or overlap.

### Comparison operators

```c++
std::strong_ordering operator<=>(const IntervalSet& a, const IntervalSet& b) noexcept;
bool operator==(const IntervalSet& a, const IntervalSet& b) noexcept;
bool operator!=(const IntervalSet& a, const IntervalSet& b) noexcept;
bool operator<(const IntervalSet& a, const IntervalSet& b) noexcept;
bool operator>(const IntervalSet& a, const IntervalSet& b) noexcept;
bool operator<=(const IntervalSet& a, const IntervalSet& b) noexcept;
bool operator>=(const IntervalSet& a, const IntervalSet& b) noexcept;
```

Lexicographical comparison operators. These call `T`'s equality and less-than
operators.

### Iterator functions

```c++
IntervalSet::iterator IntervalSet::begin() const noexcept;
IntervalSet::iterator IntervalSet::end() const noexcept;
```

Iterators over the intervals in the set.

### Query functions

```c++
bool IntervalSet::contains(const T& t) const;
bool IntervalSet::operator()(const T& t) const;
```

True if the value is an element of any of the intervals in the set.

```c++
bool IntervalSet::empty() const noexcept;
```

True if the set is empty.

```c++
std::size_t IntervalSet::size() const noexcept;
```

Returns the number of intervals in the set.

```c++
std::string IntervalSet::str() const;
template <Formatter<T> F>
    std::string IntervalSet::str(const F& f) const;
std::ostream& operator<<(std::ostream& out, const IntervalSet& set);
```

Format an interval set as a string. The format is `"{A,B,C...}"`, where `A`,
`B`, `C`, etc are intervals or values of `T`, formatted as described for
`Interval::str()`.

```c++
std::size_t IntervalSet::hash() const noexcept;
struct std::hash<IntervalSet>;
```

Hash function for an interval set.

### Modifying functions

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

Removes an interval from the set. This does not have to exactly match an
interval already in the set. Intervals in the set that overlap this interval
will be modified or removed as necessary. This will have no effect if this
interval does not overlap any existing interval in the set.

```c++
void IntervalSet::swap(IntervalSet& set) noexcept;
void swap(IntervalSet& a, IntervalSet& b) noexcept;
```

Swap two interval sets.

### Set operations

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

### Transformation functions

```c++
IntervalSet IntervalSet::complement() const;
IntervalSet IntervalSet::operator~() const;
```

Returns the complement of the set, i.e. a new set whose member intervals
contain every value of `T` that is not in this set.
