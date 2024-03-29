#        Copyright Fernando Escribano Macias 2022
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

list(APPEND sources ${CMAKE_CURRENT_SOURCE_DIR}/details/max_alignof.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/details/multiple.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/details/packed_sizeof.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/details/type_sequence.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/array.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/vector.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/vector_iterator.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/vector_const_iterator.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/select.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/reference_tuple.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/tuple_element.cpp)

add_executable(tests ${sources})

target_include_directories(tests PUBLIC ${GTest_INCLUDE_DIR})
target_link_libraries(tests GTest::gmock_main GTest::gmock GTest::gtest)

set_property(TARGET tests PROPERTY CXX_STANDARD 20)