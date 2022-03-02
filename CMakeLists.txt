#        Copyright Fernando Escribano Macias 2022
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.16)
project(multi CXX)

option(MULTI_ENABLE_TESTING "Build Unit Tests" ON)
option(MULTI_ENABLE_BENCHMARKING "Build Micro Benchmarks" ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(${MULTI_ENABLE_TESTING} OR ${MULTI_ENABLE_BENCHMARKING})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googletest)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googletest/googletest/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googletest/googlemock/include)
endif(${MULTI_ENABLE_TESTING} OR ${MULTI_ENABLE_BENCHMARKING})

if (${MULTI_ENABLE_TESTING})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif(${MULTI_ENABLE_TESTING})

if (${MULTI_ENABLE_BENCHMARKING})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/benchmark)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/benchmark/include)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/microbench)
endif(${MULTI_ENABLE_BENCHMARKING})