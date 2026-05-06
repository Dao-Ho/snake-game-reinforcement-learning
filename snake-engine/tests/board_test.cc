#include "snake/board.h"
#include <gtest/gtest.h>

using namespace snake_engine;

TEST(BoardTest, InitialStateNotGameOver) {
  Board board(10, 10);
  EXPECT_FALSE(board.IsGameOver());
}


TEST(BoardTest, InitialStateZeroScore) {
  Board board(10, 10);
  EXPECT_FALSE(board.GetScore(), 0);
}

