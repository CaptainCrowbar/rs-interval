cmake_minimum_required(VERSION 3.15)
# set(CMAKE_VERBOSE_MAKEFILE ON)

project(rs-interval-lib
    VERSION 1.0
    LANGUAGES CXX
)

include(compilers.cmake)

set(library rs-interval)
set(unittest ${library}-test)
include_directories(.)
find_package(Threads REQUIRED)

add_executable(${unittest}
    test/type-traits-test.cpp
    test/format-integer-test.cpp
    test/format-floating-point-test.cpp
    test/interval-boundary-comparison-test.cpp
    test/interval-boundary-addition-test.cpp
    test/interval-boundary-multiplication-test.cpp
    test/ordered-basic-test.cpp
    test/ordered-set-test.cpp
    test/ordered-map-test.cpp
    test/stepwise-basic-test.cpp
    test/stepwise-set-test.cpp
    test/stepwise-map-test.cpp
    test/integral-basic-test.cpp
    test/integral-set-test.cpp
    test/integral-map-test.cpp
    test/integral-arithmetic-test.cpp
    test/continuous-basic-test.cpp
    test/continuous-set-test.cpp
    test/continuous-map-test.cpp
    test/continuous-arithmetic-test.cpp
    test/unit-test.cpp
)

target_link_libraries(${unittest}
    PRIVATE Threads::Threads
)

file(GLOB subheaders "${library}/*.hpp")
install(FILES ${library}.hpp ${subheaders} DESTINATION include/${library})
