# Intervals Library

By Ross Smith

_[GitHub repository](https://github.com/CaptainCrowbar/rs-intervals)_

## Overview

```c++
#include "rs-intervals.hpp"
namespace RS::Intervals;
```

This library provides classes for the manipulation of intervals within an
ordered type; for example, a range of integers or floating point values.
Other ordered but non-arithmetic types, such as strings, are also supported.
In addition to the basic interval type, it also supports sets of intervals,
and maps from a series of intervals to a value type.

Some simple examples:

```c++
Interval<int> ii(10, 20);
    // Contains all integers from 10 to 20
Interval<double> di(0, 1, Bound::open);
    // Contains all values from 0 to 1, excluding 0 and 1
Interval<std::string> si("A", "B", Bound::closed, Bound::open);
    // Contains all strings starting with A
```

The library is header-only; there are no binaries to build or install. A CMake
file is supplied to build the tests, but this is not required if you only
want to use the library and not run the tests. Installing it simply requires
copying the `"rs-intervals"` directory and `"rs-intervals.hpp"` header
(these are under `"source"`) to somewhere on your include search path. The
`install` option in the CMake file will do this (it may not work on Windows
because of the lack of a standardized search path).

Although this library provides arithmetic operators on intervals where they
make sense, it is not intended to be a full interval arithmetic library in
the sense used in scientific computing, since it does not include the special
handling of floating point rounding issues required by such a library.

You can include `"rs-intervals.hpp"` to import the whole library, or a
selection from the following headers if you don't need all of the library's
features:

* `"rs-intervals/interval.hpp"` -- [Interval class](interval.html)
* `"rs-intervals/interval-map.hpp"` -- [Interval map class](interval-map.html)
* `"rs-intervals/interval-set.hpp"` -- [Interval set class](interval-set.html)
* `"rs-intervals/version.hpp"` -- [Version information](version.html)

The following principal classes are defined (in addition to a number of
supporting infrastructure types):

```c++
template <IntervalCompatible T> class Interval;
```

A class representing an interval over the underlying type `T`. The
`IntervalCompatible` concept, indicating a valid underlying value type for an
interval, is also defined in the `"interval.hpp"` header.

The interval may be a finite interval with upper and lower bounds, bounded on
one side but unbounded on the other, an empty interval (containing no
values), or a universal interval (containing all values of the underlying
type). Depending on the properties of the value type, the interval class may
also distinguish between open, closed, and half-open intervals, indicating
whether or not the interval includes neither, both, or one of its bounds.

```c++
template <IntervalCompatible T> class IntervalSet;
```

This class represents an arbitrary, possibly discontinuous, subset of the
domain of `T` as a set of intervals. This is an ordered set analogous to
`std::set`. The intervals in the set are automatically updated when intervals
are inserted or erased: an inserted interval will be merged with any existing
intervals that it touches; an interval will be removed, reduced in size, or
split into two if part of it is erased.

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
