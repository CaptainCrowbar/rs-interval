cmake_minimum_required(VERSION 3.18)
# set(CMAKE_VERBOSE_MAKEFILE ON)

project(rs-interval-lib
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
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
    add_compile_definitions(NOMINMAX=1 UNICODE=1 _UNICODE=1 WINVER=0x601 _WIN32_WINNT=0x601)
else()
    add_compile_definitions(_REENTRANT=1 _XOPEN_SOURCE=700)
    if(APPLE)
        add_compile_definitions(_DARWIN_C_SOURCE=1)
    endif()
endif()

function(check_for_library required library header repo)
    find_file(found ${header})
    if(NOT found)
        if(required)
            message(SEND_ERROR "Required library not found: ${library}\nDownload from: ${repo}\n")
        else()
            message(WARNING "Optional library not found: ${library}\nDownload from: ${repo}\n")
        endif()
    endif()
endfunction()

check_for_library(yes rs-format rs-format.hpp https://github.com/CaptainCrowbar/rs-format)
check_for_library(no rs-unit-test rs-unit-test.hpp https://github.com/CaptainCrowbar/rs-unit-test)

set(library rs-interval)
set(unittest ${library}-test)
include_directories(.)
find_package(Threads REQUIRED)

add_executable(${unittest}
    test/type-traits-test.cpp
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

install(DIRECTORY ${library} DESTINATION include)
install(FILES ${library}.hpp DESTINATION include)
