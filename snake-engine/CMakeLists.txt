
cmake_minimum_required(VERSION 3.20)

# Project
project(SnakeGame CXX)

set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

FetchContent_Declare(
  abseil-cpp
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG 20260107.1
)

add_executable(snake_game snake_game.cpp)
set(ABSL_BUILD_TESTING OFF CACHE BOOL "Disable Abseil tests")

FetchContent_MakeAvailable(abseil-cpp)
target_link_libraries(snake_game PRIVATE absl::status)

target_include_directories(snake_game PRIVATE include)