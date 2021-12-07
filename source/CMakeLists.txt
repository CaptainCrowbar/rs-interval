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
    test/format-test.cpp
    test/interval-basic-test.cpp
    test/interval-ordered-test.cpp
    test/interval-ordered-set-test.cpp
    test/interval-ordered-map-test.cpp
    test/interval-stepwise-test.cpp
    test/interval-stepwise-set-test.cpp
    test/interval-stepwise-map-test.cpp
    test/interval-integral-test.cpp
    test/interval-integral-set-test.cpp
    test/interval-integral-map-test.cpp
    test/interval-integral-arithmetic-test.cpp
    test/interval-continuous-test.cpp
    test/interval-continuous-set-test.cpp
    test/interval-continuous-map-test.cpp
    test/interval-continuous-arithmetic-test.cpp
    test/unit-test.cpp
)

target_link_libraries(${unittest}
    PRIVATE Threads::Threads
)

file(GLOB headers "${library}/*.hpp")
install(FILES ${headers} DESTINATION include/${library})
