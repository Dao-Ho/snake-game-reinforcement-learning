
cmake_minimum_required(VERSION 3.20)

# Project
project(SnakeGame CXX)

set(CMAKE_CXX_STANDARD 17)
enable_testing()
include(FetchContent)

FetchContent_Declare(
  abseil-cpp
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG 20260107.1
)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11
  GIT_TAG v3.0.4
)

set(ABSL_BUILD_TESTING OFF CACHE BOOL "Disable Abseil tests")
FetchContent_MakeAvailable(abseil-cpp)
FetchContent_MakeAvailable(pybind11)

add_library(snake_engine src/board.cc)
add_executable(snake_game snake_game.cpp)
target_link_libraries(snake_engine PUBLIC absl::status absl::statusor)
target_link_libraries(snake_game PRIVATE snake_engine ncurses)
target_include_directories(snake_engine PUBLIC include)

pybind11_add_module(snake_engine_py python/snake_env.cc)
target_link_libraries(snake_engine_py PRIVATE snake_engine)

add_subdirectory(tests)