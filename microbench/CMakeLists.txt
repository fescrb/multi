#        Copyright Fernando Escribano Macias 2022
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

list(APPEND sources ${CMAKE_CURRENT_SOURCE_DIR}/vector.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/select.cpp)

add_executable(microbench ${sources})

target_include_directories(tests PUBLIC ${GTest_INCLUDE_DIR} ${Benchmark_INCLUDE_DIR})
target_link_libraries(microbench benchmark::benchmark_main)

set_property(TARGET microbench PROPERTY CXX_STANDARD 20)