#        Copyright Fernando Escribano Macias 2022
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.16)
project(multi CXX)

option(MULTI_ENABLE_TESTING "Build Unit Tests" ON)
option(MULTI_ENABLE_BENCHMARKING "Build Micro Benchmarks" ON)
option(MULTI_DOCS "Build Docs" ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(${MULTI_ENABLE_TESTING} OR ${MULTI_ENABLE_BENCHMARKING})
    find_package(GTest REQUIRED)
    include(CTest)
    enable_testing()
endif(${MULTI_ENABLE_TESTING} OR ${MULTI_ENABLE_BENCHMARKING})

if (${MULTI_ENABLE_TESTING})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
    add_test(NAME tests COMMAND tests/tests --gtest_output=xml:test_result.xml)
endif(${MULTI_ENABLE_TESTING})

if (${MULTI_ENABLE_BENCHMARKING})
    find_package(Benchmark REQUIRED)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/microbench)
    add_test(NAME microbench COMMAND microbench/microbench --benchmark_out_format=json --benchmark_out=benchmark_result.json)
endif(${MULTI_ENABLE_BENCHMARKING})

if (${MULTI_DOCS})
    find_package(Doxygen REQUIRED)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/docs)
endif(${MULTI_DOCS})