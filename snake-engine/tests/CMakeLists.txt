cmake_minimum_required(VERSION 3.20)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

add_executable(board_tests board_test.cc)
target_link_libraries(board_tests PRIVATE snake_engine GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(board_tests)