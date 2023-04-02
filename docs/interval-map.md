# Interval Map Class

_[Interval Library by Ross Smith](index.html)_

```c++
#include "rs-intervals/interval-map.hpp"
namespace RS::Intervals;
```

This header defines a class representing a map from a set of disjoint
intervals to values of some other type.

## Contents

* TOC
{:toc}

## Interval map

```c++
template <IntervalCompatible K, std::regular T> class IntervalMap;
```

A map from a set of intervals over `K` to values of `T`. The `IntervalMap`
object also contains a default value of `T` that will be returned when a key
is not a member of any of the intervals in the map.

This is an ordered map analogous to `std::map`. The intervals in the map are
automatically updated when intervals are inserted or erased: an inserted
interval will erase any parts of existing intervals that it covers, or will
be merged with them if they have the same mapped value; an interval will be
removed, reduced in size, or split into two if part of it is erased.

### Member types

```c++
using IntervalMap::key_type = K;
using IntervalMap::interval_type = Interval<K>;
using IntervalMap::mapped_type = T;
using IntervalMap::value_type = std::pair<const Interval<K>, T>;
```

Type aliases.

```c++
class IntervalMap::iterator;
```

A bidirectional `const` iterator over the intervals that make up the set,
dereferencing to an `Interval<T>`.

### Member constants

```c++
static constexpr Category IntervalMap::category
    = interval_category<K>;
```

The underlying key type's interval category.

### Life cycle functions

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

An optional default value can be provided; if none is provided, a default
constructed value of `T` is used. When a map is constructed from a list of
`(interval,value)` pairs, the intervals are ordered lexicographically, and
adjacent intervals are merged when they touch or overlap and have the same
mapped value. When two intervals overlap but do not have the same mapped
value, later entries in the initializer list will overwrite earlier ones.

### Comparison operators

```c++
std::strong_ordering
    operator<=>(const IntervalMap& a, const IntervalMap& b) noexcept;
bool operator==(const IntervalMap& a, const IntervalMap& b) noexcept;
bool operator!=(const IntervalMap& a, const IntervalMap& b) noexcept;
bool operator<(const IntervalMap& a, const IntervalMap& b) noexcept;
bool operator>(const IntervalMap& a, const IntervalMap& b) noexcept;
bool operator<=(const IntervalMap& a, const IntervalMap& b) noexcept;
bool operator>=(const IntervalMap& a, const IntervalMap& b) noexcept;
```

Lexicographical comparison operators. These call `T`'s equality and less-than
operators.

### Element access functions

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

### Query functions

```c++
bool IntervalMap::contains(const K& key) const;
```

True if one of the intervals in the map contains the key.

```c++
const T& IntervalMap::default_value() const noexcept;
```

Returns the default value.

```c++
bool IntervalMap::empty() const noexcept;
```

True if the map is empty.

```c++
std::size_t IntervalMap::size() const noexcept;
```

Returns the number of intervals in the map.

```c++
std::string IntervalMap::str() const;
template <typename F>
    requires Formatter<F, K> && Formatter<F, T>
    std::string IntervalMap::str(const F& f) const;
template <Formatter<K> FK, Formatter<T> FT>
    std::string IntervalMap::str(const FK& fk, const FT& ft) const;
std::ostream& operator<<(std::ostream& out, const IntervalMap& set);
```

Format an interval map as a string. The format is `"{A:X,B:Y,C:Z,...}"`, where
`A`, `B`, `C`, etc are intervals or values of `K` (formatted as described for
`Interval::str()`), and `X`, `Y`, `Z`, etc are values of `T`. The two
formatter parameters (`fk` and `ft`) are used for the keys and values
respectively. The default value is not reported.

```c++
std::size_t IntervalMap::hash() const noexcept;
struct std::hash<IntervalMap>;
```

Hash function for an interval map.

### Modifying functions

```c++
void IntervalMap::default_value(const T& defval);
```

Changes the default value.

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

Removes an interval from the map. This does not have to exactly match an
interval already in the set. Intervals in the map that overlap this interval
will be modified or removed as necessary. This will have no effect if this
interval does not overlap any existing interval in the map.

```c++
void IntervalMap::swap(IntervalMap& map) noexcept;
void swap(IntervalMap& a, IntervalMap& b) noexcept;
```

Swap two interval maps.
