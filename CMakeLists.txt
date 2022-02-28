#        Copyright Fernando Escribano Macias 2022
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.16)
project(multi CXX)

option(BUILD_TESTS "Build Unit Tests" ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if (${BUILD_TESTS})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googletest)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googletest/googletest/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googletest/googlemock/include)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif(${BUILD_TESTS})