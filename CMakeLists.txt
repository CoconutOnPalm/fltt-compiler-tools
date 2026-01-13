cmake_minimum_required(VERSION 3.31)

project(fltt-compiler-tools)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

include(FetchContent)

FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG v6.1.9  # Replace with a version, tag, or commit hash
)

FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)

FetchContent_MakeAvailable(ftxui argparse)

BISON_TARGET(Parser debugger/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp)
FLEX_TARGET(Scanner debugger/lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

set(CXX_STANDARD 23)
set(FLAGS -std=c++23 -O3)
set(INCDIR include/)

set (
    DBGSRC 
    debugger/main.cpp  
    debugger/mw.cpp
    ${BISON_Parser_OUTPUTS} 
    ${FLEX_Scanner_OUTPUTS}
)

add_executable(debugger ${DBGSRC})

target_compile_options(debugger PRIVATE ${FLAGS})
target_include_directories(debugger PRIVATE 
    ${INCDIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/debugger
)
target_link_libraries(debugger 
    PRIVATE stdc++exp
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)