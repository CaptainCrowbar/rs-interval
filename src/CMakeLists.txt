cmake_minimum_required(VERSION 3.22)
# set(CMAKE_VERBOSE_MAKEFILE ON)

project(IntervalsLibrary
    VERSION 0.3.7
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS=1)
    add_compile_options(/EHsc /Gy /MP /O2 /sdl /utf-8 /W4 /WX)
else()
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    add_compile_options(-fdiagnostics-color=always -finput-charset=UTF-8 -march=native -O2 -Wall -Wextra -Wpedantic -Werror)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-Wsuggest-override -Wsuggest-destructor-override)
    endif()
endif()

if(WIN32)
    add_compile_definitions(NOMINMAX=1 UNICODE=1 _UNICODE=1 WINVER=0xa00 _WIN32_WINNT=0xa00)
else()
    add_compile_definitions(_REENTRANT=1 _XOPEN_SOURCE=700)
    if(APPLE)
        add_compile_definitions(_DARWIN_C_SOURCE=1)
    endif()
endif()

find_package(Threads REQUIRED)
include_directories(.)
set(library rs-interval)
set(unittest test-${library})

add_executable(${unittest}
    test/types-test.cpp
    test/continuous-arithmetic-test.cpp
    test/continuous-basic-test.cpp
    test/continuous-boundary-addition-test.cpp
    test/continuous-boundary-basic-test.cpp
    test/continuous-boundary-comparison-test.cpp
    test/continuous-boundary-multiplication-test.cpp
    test/continuous-map-test.cpp
    test/continuous-set-test.cpp
    test/integral-arithmetic-test.cpp
    test/integral-basic-test.cpp
    test/integral-boundary-addition-test.cpp
    test/integral-boundary-basic-test.cpp
    test/integral-boundary-comparison-test.cpp
    test/integral-boundary-multiplication-test.cpp
    test/integral-map-test.cpp
    test/integral-set-test.cpp
    test/ordered-basic-test.cpp
    test/ordered-boundary-basic-test.cpp
    test/ordered-boundary-comparison-test.cpp
    test/ordered-map-test.cpp
    test/ordered-set-test.cpp
    test/stepwise-basic-test.cpp
    test/stepwise-boundary-basic-test.cpp
    test/stepwise-boundary-comparison-test.cpp
    test/stepwise-example.hpp
    test/stepwise-map-test.cpp
    test/stepwise-set-test.cpp
    test/version-test.cpp
    test/unit-test.cpp
)

target_link_libraries(${unittest}
    PRIVATE Threads::Threads
)

install(FILES ${library}.hpp DESTINATION include)
install(DIRECTORY ${library} DESTINATION include PATTERN "*.cpp" EXCLUDE)
