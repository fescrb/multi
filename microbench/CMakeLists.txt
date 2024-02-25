#        Copyright Fernando Escribano Macias 2022
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

list(APPEND sources ${CMAKE_CURRENT_SOURCE_DIR}/src/elements.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/vector.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/select.cpp)

add_executable(microbench ${sources})

target_include_directories(microbench PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${GTest_INCLUDE_DIR} ${Benchmark_INCLUDE_DIR})
target_link_libraries(microbench benchmark::benchmark_main)

set_property(TARGET microbench PROPERTY CXX_STANDARD 20)