cmake_minimum_required(VERSION 3.20)
project(BitQueue)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(tokens)
add_subdirectory(ast)
add_subdirectory(interpret)

add_executable(BitQueue bitqueue.cpp)

target_link_libraries(BitQueue PRIVATE Tokenizer Parser Interpreter)