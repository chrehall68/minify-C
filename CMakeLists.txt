project(golfC)

cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_library(boost_regex libboost_regex.a)
find_package(BISON)
BISON_TARGET(PG src/generator/grammar.y ${CMAKE_CURRENT_BINARY_DIR}/bison_outputs/pg.cpp DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/bison_outputs/pg.hpp)

# compile this stuff once
add_library(
    parser_lib
    STATIC
    src/generator/lexer.cpp
    src/generator/parser.cpp
    src/generator/ast.cpp
    src/generator/types.cpp
)
target_include_directories(parser_lib PRIVATE include)
target_link_libraries(parser_lib PUBLIC boost_regex)

# stage one - build the generator application
add_executable(generator 
    src/generator/main.cpp
    src/generator/context.cpp
    ${BISON_PG_OUTPUTS}
)
add_dependencies(generator parser_lib)
target_include_directories(generator PRIVATE include ${CMAKE_CURRENT_BINARY_DIR}/bison_outputs)
target_link_libraries(generator parser_lib)

# produce grammar
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lang/cgrammar.hpp
    COMMAND generator ${CMAKE_CURRENT_SOURCE_DIR}/src/lang/ebnf_grammar.g -o ${CMAKE_CURRENT_BINARY_DIR}/lang/cgrammar.hpp
    DEPENDS generator ${CMAKE_CURRENT_SOURCE_DIR}/src/lang/ebnf_grammar.g
)
add_custom_target(cgrammar DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/lang/cgrammar.hpp)

# make client applications
add_executable(
    minifier
    src/minifier/main.cpp
    src/lang/clexer.cpp
)
target_include_directories(minifier PRIVATE include ${CMAKE_CURRENT_BINARY_DIR}/lang)
add_dependencies(minifier cgrammar)
target_link_libraries(minifier parser_lib)