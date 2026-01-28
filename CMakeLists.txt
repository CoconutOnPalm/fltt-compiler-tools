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

FetchContent_Declare (
   subprocess
   GIT_REPOSITORY https://github.com/benman64/subprocess.git
   GIT_TAG v0.6.0
)

FetchContent_Declare(
    json 
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_MakeAvailable(ftxui argparse subprocess json)

BISON_TARGET(Parser global/parser/parser.y ${CMAKE_CURRENT_SOURCE_DIR}/global/parser/parser.cpp DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/global/parser/parser.hpp)
FLEX_TARGET(Scanner global/parser/lexer.l  ${CMAKE_CURRENT_SOURCE_DIR}/global/parser/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

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

add_executable(debug ${DBGSRC})

target_compile_options(debug PRIVATE ${FLAGS})
target_include_directories(debug PRIVATE 
    ${INCDIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/debugger
)
target_link_libraries(debug 
    PRIVATE stdc++exp
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)


set (
    BENCHSRC
    benchmarker/src/main.cpp
    benchmarker/src/mw.cpp
    benchmarker/src/input/argparser.cpp
    benchmarker/src/input/jsonparser.cpp
    benchmarker/src/tui/benchmark_ui.cpp
    ${BISON_Parser_OUTPUTS} 
    ${FLEX_Scanner_OUTPUTS}
)


add_executable(benchmark ${BENCHSRC})

target_compile_options(benchmark PRIVATE ${FLAGS})
target_include_directories(benchmark PRIVATE 
    ${INCDIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/benchmarker
)
target_link_libraries(benchmark 
    PRIVATE stdc++exp
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE subprocess
)