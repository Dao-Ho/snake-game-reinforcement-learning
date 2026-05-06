#include "snake/board.h"
#include <gtest/gtest.h>

using namespace SnakeEngine;

TEST(BoardTest, InitialStateNotGameOver) {
  Board board(10, 10);
  EXPECT_FALSE(board.IsGameOver());
}

TEST(BoardTest, InitialStateZeroScore) {
  Board board(10, 10);
  EXPECT_EQ(board.GetScore(), 0);
}

TEST(BoardTest, OutofBoundsCorrect) {
  const int size = 5;
  const int moves_until_out_of_bounds = (size / 2) + 1;
  Board board(size, size);
  for (int i = 0; i < moves_until_out_of_bounds; ++i) {
    board.ApplyMove(Move::kUp);
  }
  EXPECT_TRUE(board.IsGameOver());
}
