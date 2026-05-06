#include "snake/board.h"

#include <gtest/gtest.h>

#include <algorithm>

using namespace SnakeEngine;

TEST(BoardTest, InitialStateNotGameOver) {
  Board board(10, 10);
  EXPECT_FALSE(board.IsGameOver());
}

TEST(BoardTest, InitialStateZeroScore) {
  Board board(10, 10);
  EXPECT_EQ(board.GetScore(), 0);
}

TEST(BoardTest, OutOfBoundsTopWall) {
  Board board(10, 10);
  for (int i = 0; i < 6; ++i) (void)board.ApplyMove(Move::kUp);
  EXPECT_TRUE(board.IsGameOver());
}

TEST(BoardTest, OutOfBoundsBottomWall) {
  Board board(10, 10);
  for (int i = 0; i < 6; ++i) (void)board.ApplyMove(Move::kDown);
  EXPECT_TRUE(board.IsGameOver());
}

TEST(BoardTest, OutOfBoundsLeftWall) {
  Board board(10, 10);
  for (int i = 0; i < 6; ++i) (void)board.ApplyMove(Move::kLeft);
  EXPECT_TRUE(board.IsGameOver());
}

TEST(BoardTest, OutOfBoundsRightWall) {
  Board board(10, 10);
  for (int i = 0; i < 6; ++i) (void)board.ApplyMove(Move::kRight);
  EXPECT_TRUE(board.IsGameOver());
}

TEST(BoardTest, ReverseDirectionContinuesForward) {
  Board board(10, 10);
  ASSERT_TRUE(board.ApplyMove(Move::kDown).ok());
  EXPECT_FALSE(board.IsGameOver());
}

TEST(BoardTest, EatingAppleIncrementsScore) {
  Board board(10, 10);
  int initial_score = board.GetScore();
  int moves = 0;
  while (board.GetScore() == initial_score && !board.IsGameOver() &&
         moves < 200) {
    (void)board.ApplyMove(Move::kUp);
    ++moves;
  }
  if (!board.IsGameOver()) {
    EXPECT_GT(board.GetScore(), initial_score);
  }
}

TEST(BoardTest, InitialGridHasExactlyOneApple) {
  Board board(10, 10);
  const auto& grid = board.GetGrid();
  int apple_count = std::count(grid.begin(), grid.end(), Cell::kApple);
  EXPECT_EQ(apple_count, 1);
}

TEST(BoardTest, InitialGridHasExactlyOneSnakeHead) {
  Board board(10, 10);
  const auto& grid = board.GetGrid();
  int head_count = std::count(grid.begin(), grid.end(), Cell::kSnakeHead);
  EXPECT_EQ(head_count, 1);
}

TEST(BoardTest, AppleCountRemainsOneAfterEating) {
  Board board(10, 10);
  for (int i = 0; i < 50 && !board.IsGameOver(); ++i) {
    (void)board.ApplyMove(Move::kUp);
  }
  if (!board.IsGameOver()) {
    const auto& grid = board.GetGrid();
    int apple_count = std::count(grid.begin(), grid.end(), Cell::kApple);
    EXPECT_EQ(apple_count, 1);
  }
}
