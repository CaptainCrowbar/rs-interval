# Intervals and Interval Based Containers

By Ross Smith

_[GitHub repository](https://github.com/CaptainCrowbar/rs-interval)_

## Overview

```c++
#include "rs-intervals/interval.hpp"
namespace RS::Intervals;
```

This is a headers-only library; there is nothing to build (unless you want to
run the unit tests), and no binaries to install. The CMake file includes an
`install` target that copies the headers into `/usr/local/include` or the
equivalent. Headers can be included individually as required, or the entire
library can be included using `"rs-interval.hpp"`.

Other libraries required (all header only):

[My formatting library](https://github.com/CaptainCrowbar/rs-format)
[My template library](https://github.com/CaptainCrowbar/rs-tl)
[My unit test library](https://github.com/CaptainCrowbar/rs-unit-test)

## Index

* [Version information](version.html)
* [Supporting types](types.html)
* [Interval class](interval.html)
* [Interval set class](set.html)
* [Interval map class](map.html)
