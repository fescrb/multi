#        Copyright Fernando Escribano Macias 2022
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

list(APPEND sources ${CMAKE_CURRENT_SOURCE_DIR}/details/max_alignof.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/details/packed_sizeof.cpp)

add_executable(tests ${sources})

target_link_libraries(tests gmock_main gmock gtest)

set_property(TARGET tests PROPERTY CXX_STANDARD 20)