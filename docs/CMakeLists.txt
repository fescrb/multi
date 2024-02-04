#        Copyright Fernando Escribano Macias 2024
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

file(GLOB_RECURSE MULTI_PUBLIC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../include/*/*.hpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile @ONLY)

message("${MULTI_PUBLIC_HEADERS}")

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/html/index.html
                   DEPENDS ${MULTI_PUBLIC_HEADERS}
                   COMMAND Doxygen::doxygen ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)

add_custom_target(docs ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/html/index.html)