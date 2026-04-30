
cmake_minimum_required(VERSION 3.20)

# Project
project(SnakeGame CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(snake_game snake_game.cpp)

target_include_directories(snake_game PRIVATE include)