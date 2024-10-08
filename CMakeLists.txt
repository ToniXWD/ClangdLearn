cmake_minimum_required(VERSION 3.10)
project(ClangdLearn)

find_package(fmt CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ClangdLearn main.cpp)
target_link_libraries(ClangdLearn PRIVATE fmt::fmt)

