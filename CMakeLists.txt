cmake_minimum_required(VERSION 3.16)

project(tetris CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window audio)

include_directories("D:\\SFML-2.5.1\\include")

add_executable(tetris main.cpp tetris.cpp tetris.hpp)

target_link_libraries(tetris PRIVATE sfml-graphics sfml-window sfml-audio)

install(TARGETS tetris)
